/**
 * Created March 26, 2024
 *
 * For MCU build target (CORE_ARDUINO_XXXX), see Options.h.
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GSHEET_ASYNC_CLIENT_H
#define GSHEET_ASYNC_CLIENT_H
#include <vector>
#include "./core/AsyncClient/RequestHandler.h"
#include "./core/AsyncClient/ResponseHandler.h"
#include "./core/NetConfig.h"
#include "./core/Memory.h"
#include "./core/FileConfig.h"
#include "./core/Base64.h"
#include "./core/Error.h"
#include "./core/OTA.h"
#include "./core/AsyncResult/AsyncResult.h"
#include "./core/AuthConfig.h"
#include "./core/List.h"
#include "./core/Core.h"
#include "./core/URL.h"

#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)
#include "./core/AsyncTCPConfig.h"
#endif

using namespace gsheet;

enum gsheet_async_state
{
    gsheet_async_state_undefined,
    gsheet_async_state_send_header,
    gsheet_async_state_send_payload,
    gsheet_async_state_read_response,
    gsheet_async_state_complete
};

enum gsheet_function_return_type
{
    gsheet_function_return_type_undefined = -2,
    gsheet_function_return_type_failure = -1,
    gsheet_function_return_type_continue = 0,
    gsheet_function_return_type_complete = 1,
    gsheet_function_return_type_retry = 2
};

struct gsheet_async_data_item_t
{
    friend class GSheetApp;

public:
    struct async_error_t
    {
        gsheet_async_state state = gsheet_async_state_undefined;
        int code = 0;
    };

    gsheet_async_state state = gsheet_async_state_undefined;
    gsheet_function_return_type return_type = gsheet_function_return_type_undefined;
    gsheet_async_request_handler_t request;
    gsheet_async_response_handler_t response;
    async_error_t error;
    bool to_remove = false;
    bool auth_used = false;
    bool complete = false;
    bool async = false;
    bool cancel = false;
    uint32_t auth_ts = 0;
    uint32_t addr = 0;
    GSheetAsyncResult aResult;
    GSheetAsyncResult *refResult = nullptr;
    uint32_t ref_result_addr = 0;
    GSheetAsyncResultCallback cb = NULL;
    GSheetTimer err_timer;
    gsheet_async_data_item_t()
    {
        addr = reinterpret_cast<uint32_t>(this);
        err_timer.feed(0);
    }

    void setRefResult(GSheetAsyncResult *refResult)
    {
        this->refResult = refResult;
        ref_result_addr = refResult->addr;
    }

    void reset()
    {
        state = gsheet_async_state_undefined;
        return_type = gsheet_function_return_type_undefined;
        request.clear();
        response.clear();
        error.code = 0;
        error.state = gsheet_async_state_undefined;
        to_remove = false;
        auth_used = false;
        complete = false;
        async = false;
        cancel = false;
        cb = NULL;
        err_timer.reset();
    }
};

struct gsheet_slot_options_t
{
public:
    bool auth_used = false;
    bool async = false;
    bool ota = false;
    bool auth_param = false;
    app_token_t *app_token = nullptr;
    gsheet_slot_options_t() {}
    gsheet_slot_options_t(bool auth_used, bool sse, bool async, bool sv, bool ota, bool no_etag, bool auth_param = false)
    {
        this->auth_used = auth_used;
        this->async = async;
        this->ota = ota;
        this->auth_param = auth_param;
    }
};

class GSheetAsyncClientClass
{
    friend class GSheetApp;

private:
    GSheetError lastErr;
    String header;
    int netErrState = 0;
    uint32_t auth_ts = 0;
    Client *client = nullptr;
#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)
    GSheetAsyncTCPConfig *async_tcp_config = nullptr;
#else
    void *async_tcp_config = nullptr;
#endif
    gsheet_async_request_handler_t::tcp_client_type client_type = gsheet_async_request_handler_t::tcp_client_type_sync;
    std::vector<uint32_t> sVec;
    GSheetMemory mem;
    GSheetBase64Util but;
    gsheet_network_config_data net;
    uint32_t addr = 0;
    bool inProcess = false;
    bool inStopAsync = false;

    void closeFile(gsheet_async_data_item_t *sData)
    {
#if defined(GSHEET_ENABLE_FS)
        if (sData->request.file_data.file && sData->request.file_data.file_status == gsheet_file_config_data::gsheet_file_status_opened)
        {
            sData->request.file_data.file_size = 0;
            sData->request.payloadIndex = 0;
            sData->request.dataIndex = 0;
            sData->request.file_data.file_status = gsheet_file_config_data::gsheet_file_status_closed;
            sData->request.file_data.file.close();
        }
#endif
    }

    bool openFile(gsheet_async_data_item_t *sData, gsheet_file_operating_mode mode)
    {
#if defined(GSHEET_ENABLE_FS)
        sData->request.file_data.cb(sData->request.file_data.file, sData->request.file_data.filename.c_str(), mode);
        if (!sData->request.file_data.file)
            return false;
#else
        return false;
#endif
        sData->request.file_data.file_status = gsheet_file_config_data::gsheet_file_status_opened;
        return true;
    }

    gsheet_function_return_type sendHeader(gsheet_async_data_item_t *sData, const char *data)
    {
        return send(sData, (uint8_t *)data, data ? strlen(data) : 0, data ? strlen(data) : 0, gsheet_async_state_send_header);
    }

    gsheet_function_return_type sendHeader(gsheet_async_data_item_t *sData, uint8_t *data, size_t len)
    {
        return send(sData, data, len, len, gsheet_async_state_send_header);
    }

    gsheet_function_return_type sendBuff(gsheet_async_data_item_t *sData, gsheet_async_state state = gsheet_async_state_send_payload)
    {
        gsheet_function_return_type ret = gsheet_function_return_type_continue;

#if defined(GSHEET_ENABLE_FS)

        size_t totalLen = sData->request.file_data.file_size;
        bool fopen = sData->request.payloadIndex == 0;

        if (fopen)
        {
            if (sData->request.file_data.filename.length() > 0)
            {
                if (sData->request.file_data.file_status == gsheet_file_config_data::gsheet_file_status_closed)
                {
                    if (!openFile(sData, gsheet_file_mode_open_read))
                    {
                        setAsyncError(sData, state, GSHEET_ERROR_OPEN_FILE, true, true);
                        return gsheet_function_return_type_failure;
                    }
                }
            }

            if (sData->request.base64)
            {
                ret = send(sData, (uint8_t *)"\"", 1, totalLen, gsheet_async_state_send_payload);
                if (ret != gsheet_function_return_type_continue)
                    return ret;
            }
        }

        uint8_t *buf = nullptr;
        int toSend = 0;
        if (sData->request.file_data.filename.length() > 0 ? sData->request.file_data.file.available() : sData->request.file_data.data_pos < sData->request.file_data.data_size)
        {
            if (sData->request.base64)
            {

                toSend = GSHEET_BASE64_CHUNK_SIZE;

                if (sData->request.file_data.filename.length() > 0)
                {
                    if (sData->request.file_data.file.available() < toSend)
                        toSend = sData->request.file_data.file.available();
                }
                else
                {
                    if ((int)(sData->request.file_data.data_size - sData->request.file_data.data_pos) < toSend)
                        toSend = sData->request.file_data.data_size - sData->request.file_data.data_pos;
                }

                buf = reinterpret_cast<uint8_t *>(mem.alloc(toSend));
                if (sData->request.file_data.filename.length() > 0)
                {
                    toSend = sData->request.file_data.file.read(buf, toSend);
                    if (toSend == 0)
                    {
                        setAsyncError(sData, state, GSHEET_ERROR_FILE_READ, true, true);
                        ret = gsheet_function_return_type_failure;
                        goto exit;
                    }
                }
                else if (sData->request.file_data.data)
                {
                    memcpy(buf, sData->request.file_data.data + sData->request.file_data.data_pos, toSend);
                    sData->request.file_data.data_pos += toSend;
                }

                uint8_t *temp = (uint8_t *)but.encodeToChars(mem, buf, toSend);
                mem.release(&buf);
                toSend = strlen((char *)temp);
                buf = temp;
            }
            else
            {

                toSend = totalLen - sData->request.file_data.data_pos < GSHEET_CHUNK_SIZE ? totalLen - sData->request.file_data.data_pos : GSHEET_CHUNK_SIZE;

                buf = reinterpret_cast<uint8_t *>(mem.alloc(toSend));

                if (sData->request.file_data.filename.length() > 0)
                {
                    toSend = sData->request.file_data.file.read(buf, toSend);
                    if (toSend == 0)
                    {
                        setAsyncError(sData, state, GSHEET_ERROR_FILE_READ, true, true);
                        ret = gsheet_function_return_type_failure;
                        goto exit;
                    }
                }
                else if (sData->request.file_data.data)
                {
                    memcpy(buf, sData->request.file_data.data + sData->request.file_data.data_pos, toSend);
                }

                sData->request.file_data.data_pos += toSend;
            }

            ret = send(sData, buf, toSend, totalLen, gsheet_async_state_send_payload);
        }
        else if (sData->request.base64)
            ret = send(sData, (uint8_t *)"\"", 1, totalLen, gsheet_async_state_send_payload);

    exit:

        if (buf)
            mem.release(&buf);
#endif

        return ret;
    }

    gsheet_function_return_type send(gsheet_async_data_item_t *sData, const char *data, gsheet_async_state state = gsheet_async_state_send_payload)
    {
        return send(sData, (uint8_t *)data, data ? strlen(data) : 0, data ? strlen(data) : 0, state);
    }

    gsheet_function_return_type send(gsheet_async_data_item_t *sData, uint8_t *data, size_t len, size_t size, gsheet_async_state state = gsheet_async_state_send_payload)
    {
        sData->state = state;

        if (data && len && this->client)
        {
            uint16_t toSend = len - sData->request.dataIndex > GSHEET_CHUNK_SIZE ? GSHEET_CHUNK_SIZE : len - sData->request.dataIndex;

            size_t sent = sData->request.tcpWrite(client_type, client, async_tcp_config, data + sData->request.dataIndex, toSend);
            gsheet_sys_idle();

            if (sent == toSend)
            {
                sData->request.dataIndex += toSend;
                sData->request.payloadIndex += toSend;

                if (sData->request.dataIndex == len)
                    sData->request.dataIndex = 0;

                if (sData->request.payloadIndex < size)
                {
                    sData->return_type = gsheet_function_return_type_continue;
                    return sData->return_type;
                }
            }
        }

        sData->return_type = sData->request.payloadIndex == size && size > 0 ? gsheet_function_return_type_complete : gsheet_function_return_type_failure;

        if (sData->return_type == gsheet_function_return_type_failure)
            setAsyncError(sData, state, GSHEET_ERROR_TCP_SEND, true, false);

        sData->request.payloadIndex = 0;
        sData->request.dataIndex = 0;
        sData->request.file_data.data_pos = 0;

        if (sData->return_type == gsheet_function_return_type_complete)
        {
            if (state == gsheet_async_state_send_header)
            {
                if (sData->request.val[gsheet_req_hndlr_ns::header].indexOf("Content-Length: 0\r\n") > -1)
                    sData->state = gsheet_async_state_read_response;
                else
                    sData->state = gsheet_async_state_send_payload;
            }
            else if (state == gsheet_async_state_send_payload)
                sData->state = gsheet_async_state_read_response;

            if (sData->state != gsheet_async_state_read_response)
                sData->response.clear();
        }

        return sData->return_type;
    }

    gsheet_function_return_type send(gsheet_async_data_item_t *sData)
    {
        if (!sData || !netConnect(sData))
        {
            setAsyncError(sData, sData->state, GSHEET_ERROR_TCP_DISCONNECTED, true, false);
            return gsheet_function_return_type_failure;
        }

        gsheet_function_return_type ret = gsheet_function_return_type_continue;

        if (sData->state == gsheet_async_state_undefined || sData->state == gsheet_async_state_send_header)
        {
            if (sData->auth_used && sData->state == gsheet_async_state_undefined)
                stop(sData);

            if ((client_type == gsheet_async_request_handler_t::tcp_client_type_sync && !client->connected()) || client_type == gsheet_async_request_handler_t::tcp_client_type_async)
            {
                ret = connect(sData, getHost(sData, true).c_str(), sData->request.port);

                // allow non-blocking async tcp connection
                if (ret == gsheet_function_return_type_continue)
                    return ret;

                if (ret != gsheet_function_return_type_complete)
                    return connErrorHandler(sData, sData->state);

                sData->auth_ts = auth_ts;
            }

            if (sData->request.app_token && sData->request.app_token->auth_data_type != user_auth_data_undefined)
            {
                if (sData->request.app_token->val[gsheet_app_tk_ns::token].length() == 0)
                {
                    setAsyncError(sData, sData->state, GSHEET_ERROR_UNAUTHENTICATE, true, false);
                    return gsheet_function_return_type_failure;
                }

                header = sData->request.val[gsheet_req_hndlr_ns::header];
                header.replace(GSHEET_AUTH_PLACEHOLDER, sData->request.app_token->val[gsheet_app_tk_ns::token]);
                ret = sendHeader(sData, header.c_str());
                header.remove(0, header.length());
                return ret;
            }
            return sendHeader(sData, sData->request.val[gsheet_req_hndlr_ns::header].c_str());
        }
        else if (sData->state == gsheet_async_state_send_payload)
        {

            if (sData->request.method == gsheet_async_request_handler_t::http_get || sData->request.method == gsheet_async_request_handler_t::http_delete)
                sData->state = gsheet_async_state_read_response;
            else
            {
                if (sData->request.val[gsheet_req_hndlr_ns::payload].length())
                    ret = send(sData, sData->request.val[gsheet_req_hndlr_ns::payload].c_str());
            }
        }
        return ret;
    }

    gsheet_function_return_type receive(gsheet_async_data_item_t *sData)
    {

        if (!sData || !netConnect(sData))
        {
            setAsyncError(sData, sData->state, GSHEET_ERROR_TCP_DISCONNECTED, true, false);
            return gsheet_function_return_type_failure;
        }

        if (!readResponse(sData))
        {
            setAsyncError(sData, sData->state, sData->response.httpCode > 0 ? sData->response.httpCode : GSHEET_ERROR_TCP_RECEIVE_TIMEOUT, true, false);
            return gsheet_function_return_type_failure;
        }

        if (sData->response.httpCode == 0)
            return gsheet_function_return_type_continue;

        if (sData->response.val[gsheet_res_hndlr_ns::location].length() && !sData->response.flags.header_remaining && !sData->response.flags.payload_remaining)
        {
            String ext;
            String host = getHost(sData, false, &ext);
            if (client)
                client->stop();
            if (connect(sData, host.c_str(), sData->request.port) > gsheet_function_return_type_failure)
            {
                GSheetURLUtil uut;
                uut.relocate(sData->request.val[gsheet_req_hndlr_ns::header], host, ext);
                sData->request.val[gsheet_req_hndlr_ns::payload].remove(0, sData->request.val[gsheet_req_hndlr_ns::payload].length());
                sData->state = gsheet_async_state_send_header;
                return gsheet_function_return_type_continue;
            }

            return connErrorHandler(sData, sData->state);
        }

        if (sData->response.httpCode > 0 && !sData->response.flags.header_remaining && !sData->response.flags.payload_remaining)
        {
            sData->state = gsheet_async_state_undefined;
            return gsheet_function_return_type_complete;
        }

        return gsheet_function_return_type_continue;
    }

    gsheet_function_return_type connErrorHandler(gsheet_async_data_item_t *sData, gsheet_async_state state)
    {
        setAsyncError(sData, state, GSHEET_ERROR_TCP_CONNECTION, true, false);
        return gsheet_function_return_type_failure;
    }

    void setAsyncError(gsheet_async_data_item_t *sData, gsheet_async_state state, int code, bool toRemove, bool toCloseFile)
    {
        sData->error.state = state;
        sData->error.code = code;

        if (toRemove)
            sData->to_remove = toRemove;

        if (toCloseFile)
            closeFile(sData);

        setLastError(sData);
    }

    gsheet_async_data_item_t *getData(uint8_t slot)
    {
        if (slot < sVec.size())
            return reinterpret_cast<gsheet_async_data_item_t *>(sVec[slot]);
        return nullptr;
    }

    gsheet_async_data_item_t *addSlot(int index = -1)
    {
        gsheet_async_data_item_t *sData = new gsheet_async_data_item_t();
        if (index > -1)
            sVec.insert(sVec.begin() + index, sData->addr);
        else
            sVec.push_back(sData->addr);

        return sData;
    }

    void returnResult(gsheet_async_data_item_t *sData, bool setData)
    {

        bool error_notify_timeout = false;
        if (sData->err_timer.remaining() == 0)
        {
            sData->err_timer.feed(5000);
            error_notify_timeout = true;
        }

        if (sData->refResult)
        {
            if (setData || error_notify_timeout)
            {
                *sData->refResult = sData->aResult;

                if (setData)
                    sData->refResult->setPayload(sData->aResult.val[gsheet_ares_ns::data_payload]);
            }
        }

        if (sData->cb && (setData || error_notify_timeout))
        {
            if (!sData->auth_used)
                sData->cb(sData->aResult);
        }
    }

    void setLastError(gsheet_async_data_item_t *sData)
    {
        if (!sData->aResult.error_available)
        {
            if (sData->error.code < 0)
            {
                sData->aResult.lastError.setClientError(sData->error.code);
                lastErr.setClientError(sData->error.code);
                sData->aResult.error_available = true;
                sData->aResult.data_available = false;
            }
            else if (sData->response.httpCode > 0 && sData->response.httpCode >= GSHEET_ERROR_HTTP_CODE_BAD_REQUEST)
            {
                sData->aResult.lastError.setResponseError(sData->response.val[gsheet_res_hndlr_ns::payload], sData->response.httpCode);
                lastErr.setResponseError(sData->response.val[gsheet_res_hndlr_ns::payload], sData->response.httpCode);
                sData->aResult.error_available = true;
                sData->aResult.data_available = false;
            }
        }
    }

    int readLine(gsheet_async_data_item_t *sData, String &buf)
    {
        int p = 0;

        while (sData->response.tcpAvailable(client_type, client, async_tcp_config))
        {
            int res = sData->response.tcpRead(client_type, client, async_tcp_config);
            if (res > -1)
            {
                buf += (char)res;
                p++;
                if (res == '\n')
                    return p;
            }
        }
        return p;
    }

    uint32_t hex2int(const char *hex)
    {
        uint32_t val = 0;
        while (*hex)
        {
            // get current character then increment
            uint8_t byte = *hex++;
            // transform hex character to the 4bit equivalent number, using the ascii table indexes
            if (byte >= '0' && byte <= '9')
                byte = byte - '0';
            else if (byte >= 'a' && byte <= 'f')
                byte = byte - 'a' + 10;
            else if (byte >= 'A' && byte <= 'F')
                byte = byte - 'A' + 10;
            // shift 4 to make space for new digit, and add the 4 bits of the new digit
            val = (val << 4) | (byte & 0xF);
        }
        return val;
    }

    void clear(String &str) { str.remove(0, str.length()); }

    bool readResponse(gsheet_async_data_item_t *sData)
    {
        if (!netConnect(sData) || !client || !sData)
            return false;

        if (sData->response.tcpAvailable(client_type, client, async_tcp_config) > 0)
        {
            // status line or data?
            if (!readStatusLine(sData))
            {
                // remaining headers to read?
                if (sData->response.flags.header_remaining)
                    readHeader(sData);
                // read payload
                else if (sData->response.flags.payload_remaining || sData->response.flags.sse)
                {
                    if (!readPayload(sData))
                        return false;

                    if (sData->response.flags.sse || !sData->response.flags.payload_remaining)
                    {
                        if (!sData->auth_used)
                        {
                            sData->aResult.setPayload(sData->response.val[gsheet_res_hndlr_ns::payload]);
                        }
                    }
                }
            }
        }

        return true;
    }

    int getStatusCode(const String &header)
    {
        String out;
        int p1 = header.indexOf("HTTP/1.");
        if (p1 > -1)
        {
            out = header.substring(p1 + 9, header.indexOf(' ', p1 + 9));
            return atoi(out.c_str());
        }
        return 0;
    }

    bool readStatusLine(gsheet_async_data_item_t *sData)
    {
        if (sData->response.httpCode > 0)
            return false;

        sData->response.val[gsheet_res_hndlr_ns::header].reserve(1024);

        // the first chunk (line) can be http response status or already connected stream payload
        readLine(sData, sData->response.val[gsheet_res_hndlr_ns::header]);
        int status = getStatusCode(sData->response.val[gsheet_res_hndlr_ns::header]);
        if (status > 0)
        {
            // http response status
            sData->response.flags.header_remaining = true;
            sData->response.httpCode = status;
        }
        return true;
    }

    void readHeader(gsheet_async_data_item_t *sData)
    {
        if (sData->response.flags.header_remaining)
        {
            int read = readLine(sData, sData->response.val[gsheet_res_hndlr_ns::header]);
            if ((read == 1 && sData->response.val[gsheet_res_hndlr_ns::header][sData->response.val[gsheet_res_hndlr_ns::header].length() - 1] == '\r') ||
                (read == 2 && sData->response.val[gsheet_res_hndlr_ns::header][sData->response.val[gsheet_res_hndlr_ns::header].length() - 2] == '\r' && sData->response.val[gsheet_res_hndlr_ns::header][sData->response.val[gsheet_res_hndlr_ns::header].length() - 1] == '\n'))
            {
                String temp[5];
                parseRespHeader(sData, sData->response.val[gsheet_res_hndlr_ns::header], sData->response.val[gsheet_res_hndlr_ns::location], "Location");

                sData->aResult.val[gsheet_ares_ns::data_path] = sData->request.val[gsheet_req_hndlr_ns::path];

                parseRespHeader(sData, sData->response.val[gsheet_res_hndlr_ns::header], temp[0], "Content-Length");

                sData->response.payloadLen = atoi(temp[0].c_str());

                parseRespHeader(sData, sData->response.val[gsheet_res_hndlr_ns::header], temp[1], "Connection");
                sData->response.flags.keep_alive = temp[1].length() && temp[1].indexOf("keep-alive") > -1;

                parseRespHeader(sData, sData->response.val[gsheet_res_hndlr_ns::header], temp[2], "Transfer-Encoding");
                sData->response.flags.chunks = temp[2].length() && temp[2].indexOf("chunked") > -1;

                parseRespHeader(sData, sData->response.val[gsheet_res_hndlr_ns::header], temp[3], "Content-Type");
                sData->response.flags.sse = temp[3].length() && temp[3].indexOf("text/event-stream") > -1;

                clear(sData);

                for (size_t i = 0; i < 5; i++)
                    temp[i].remove(0, temp[i].length());

                if (sData->response.httpCode > 0 && sData->response.httpCode != GSHEET_ERROR_HTTP_CODE_NO_CONTENT)
                    sData->response.flags.payload_remaining = true;

                if ((sData->response.httpCode == GSHEET_ERROR_HTTP_CODE_OK || sData->response.httpCode == GSHEET_ERROR_HTTP_CODE_PERMANENT_REDIRECT || sData->response.httpCode == GSHEET_ERROR_HTTP_CODE_NO_CONTENT) && !sData->response.flags.chunks && sData->response.payloadLen == 0)
                    sData->response.flags.payload_remaining = false;

                if (sData->request.method == gsheet_async_request_handler_t::http_delete && sData->response.httpCode == GSHEET_ERROR_HTTP_CODE_NO_CONTENT)
                    sData->aResult.setDebug(FPSTR("Delete operation complete"));
            }
        }
    }

    void parseRespHeader(gsheet_async_data_item_t *sData, const String &src, String &out, const char *header)
    {
        if (sData->response.httpCode > 0)
        {
            int p1 = -1, p2 = -1, p3 = -1;
            p1 = src.indexOf(header);
            if (p1 > -1)
                p2 = src.indexOf(':', p1);

            if (p2 > -1)
                p3 = src.indexOf("\r\n", p2);

            if (p2 > -1 && p3 > -1)
                out = src.substring(p2 + 1, p3);

            out.trim();
        }
    }

    int getChunkSize(gsheet_async_data_item_t *sData, Client *client)
    {
        String line;
        readLine(sData, line);
        int p = line.indexOf(";");
        if (p == -1)
            p = line.indexOf("\r\n");
        if (p != -1)
            sData->response.chunkInfo.chunkSize = hex2int(line.substring(0, p).c_str());

        return sData->response.chunkInfo.chunkSize;
    }

    // Returns -1 when complete
    int decodeChunks(gsheet_async_data_item_t *sData, Client *client, String *out)
    {
        if (!client || !sData || !out)
            return 0;
        int res = 0;

        // read chunk-size, chunk-extension (if any) and CRLF
        if (sData->response.chunkInfo.phase == gsheet_async_response_handler_t::READ_CHUNK_SIZE)
        {
            sData->response.chunkInfo.phase = gsheet_async_response_handler_t::READ_CHUNK_DATA;
            sData->response.chunkInfo.chunkSize = -1;
            sData->response.chunkInfo.dataLen = 0;
            res = getChunkSize(sData, client);
            sData->response.payloadLen += res > -1 ? res : 0;
        }
        // read chunk-data and CRLF
        // append chunk-data to entity-body
        else
        {
            if (sData->response.chunkInfo.chunkSize > -1)
            {
                String chunk;
                int read = readLine(sData, chunk);
                if (read && chunk[0] != '\r')
                    *out += chunk;
                if (read)
                {
                    sData->response.chunkInfo.dataLen += read;
                    sData->response.payloadRead += read;
                    // chunk may contain trailing
                    if (sData->response.chunkInfo.dataLen - 2 >= sData->response.chunkInfo.chunkSize)
                    {
                        sData->response.chunkInfo.dataLen = sData->response.chunkInfo.chunkSize;
                        sData->response.chunkInfo.phase = gsheet_async_response_handler_t::READ_CHUNK_SIZE;
                    }
                }
                // if all chunks read, returns -1
                else if (sData->response.chunkInfo.dataLen == sData->response.chunkInfo.chunkSize)
                    res = -1;
            }
        }

        return res;
    }

    bool readPayload(gsheet_async_data_item_t *sData)
    {
        uint8_t *buf = nullptr;
        GSheetMemory mem;

        if (sData->response.flags.payload_remaining)
        {
            sData->response.feedTimer();

            // the next chunk data is the payload
            if (sData->response.httpCode != GSHEET_ERROR_HTTP_CODE_NO_CONTENT)
            {

                if (sData->response.flags.chunks)
                {
                    if (decodeChunks(sData, client, &sData->response.val[gsheet_res_hndlr_ns::payload]) == -1)
                        sData->response.flags.payload_remaining = false;
                }
                else
                {
                    sData->response.payloadRead += readLine(sData, sData->response.val[gsheet_res_hndlr_ns::payload]);
                }
            }
        }

        if (buf)
            mem.release(&buf);

        if (sData->response.payloadLen > 0 && sData->response.payloadRead >= sData->response.payloadLen && sData->response.tcpAvailable(client_type, client, async_tcp_config) == 0)
        {
            // Async payload and header data collision workaround from session reusage.
            if (!sData->response.flags.chunks && sData->response.payloadRead > sData->response.payloadLen)
            {
                sData->response.val[gsheet_res_hndlr_ns::header] = sData->response.val[gsheet_res_hndlr_ns::payload].substring(sData->response.payloadRead - sData->response.payloadLen);
                sData->response.val[gsheet_res_hndlr_ns::payload].remove(0, sData->response.payloadLen);
                sData->return_type = gsheet_function_return_type_continue;
                sData->state = gsheet_async_state_read_response;
                sData->response.flags.header_remaining = true;
            }

            if (sData->response.flags.chunks && sData->auth_used)
                stop(sData);

            if (sData->response.httpCode >= GSHEET_ERROR_HTTP_CODE_BAD_REQUEST)
            {
                setAsyncError(sData, sData->state, sData->response.httpCode, true, true);
                sData->return_type = gsheet_function_return_type_failure;
                returnResult(sData, false);
            }

            sData->response.flags.payload_remaining = false;
            closeFile(sData);

            if (sData->auth_used)
                sData->response.auth_data_available = true;
        }

        return sData->error.code == 0;
    }

    // non-block memory buffer for collecting the multiple of 4 data prepared for base64 decoding
    uint8_t *asyncBase64Buffer(gsheet_async_data_item_t *sData, GSheetMemory &mem, int &toRead, int &read)
    {
        uint8_t *buf = nullptr;

        if (sData->request.base64)
        {
            if (sData->response.toFill && sData->response.toFillLen)
            {
                int currentRead = sData->response.tcpRead(client_type, client, async_tcp_config, sData->response.toFill + sData->response.toFillIndex, sData->response.toFillLen);
                if (currentRead == sData->response.toFillLen)
                {
                    buf = reinterpret_cast<uint8_t *>(mem.alloc(sData->response.toFillIndex + sData->response.toFillLen));
                    memcpy(buf, sData->response.toFill, sData->response.toFillIndex + sData->response.toFillLen);
                    mem.release(&sData->response.toFill);
                    read = sData->response.toFillLen + sData->response.toFillIndex;
                    toRead = read;
                    sData->response.toFillIndex = 0;
                    sData->response.toFillLen = 0;
                }
                else
                {
                    sData->response.toFillIndex += currentRead;
                    sData->response.toFillLen -= currentRead;
                }
            }
        }

        return buf;
    }

    void clear(gsheet_async_data_item_t *sData)
    {
        clear(sData->response.val[gsheet_res_hndlr_ns::header]);
        if (!sData->auth_used)
            clear(sData->response.val[gsheet_res_hndlr_ns::payload]);
        sData->response.flags.header_remaining = false;
        sData->response.flags.payload_remaining = false;
        sData->response.payloadRead = 0;
        sData->response.error.resp_code = 0;
        clear(sData->response.error.string);
        sData->response.chunkInfo.chunkSize = 0;
        sData->response.chunkInfo.dataLen = 0;
        sData->response.chunkInfo.phase = gsheet_async_response_handler_t::READ_CHUNK_SIZE;
    }

    void reset(gsheet_async_data_item_t *sData, bool disconnect)
    {
        if (disconnect)
            stop(sData);
        sData->response.httpCode = 0;
        sData->error.code = 0;
        sData->response.flags.reset();
        sData->state = gsheet_async_state_undefined;
        sData->return_type = gsheet_function_return_type_undefined;
        clear(sData);
    }

    gsheet_function_return_type connect(gsheet_async_data_item_t *sData, const char *host, uint16_t port)
    {
        sData->aResult.lastError.clearError();
        lastErr.clearError();

        if (client && !client->connected() && !sData->auth_used) // This info is already show in auth task
            sData->aResult.setDebug(FPSTR("Connecting to server..."));

        if (client && !client->connected() && client_type == gsheet_async_request_handler_t::tcp_client_type_sync)
            sData->return_type = client->connect(host, port) > 0 ? gsheet_function_return_type_complete : gsheet_function_return_type_failure;
        else if (client_type == gsheet_async_request_handler_t::tcp_client_type_async)
        {

#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)
            if (async_tcp_config && async_tcp_config->tcpStatus && async_tcp_config->tcpConnect)
            {
                bool status = false;
                if (async_tcp_config->tcpStatus)
                    async_tcp_config->tcpStatus(status);

                if (!status)
                {
                    if (async_tcp_config->tcpConnect)
                        async_tcp_config->tcpConnect(host, port);

                    if (async_tcp_config->tcpStatus)
                        async_tcp_config->tcpStatus(status);
                }

                sData->return_type = status ? gsheet_function_return_type_complete : gsheet_function_return_type_continue;
            }
#endif
        }

        return sData->return_type;
    }

    /**
     * Get the ethernet link status.
     * @return true for link up or false for link down.
     */
    bool ethLinkUp()
    {
        bool ret = false;

#if defined(GSHEET_ENABLE_ETHERNET_NETWORK)

#if defined(GSHEET_ETH_IS_AVAILABLE)

#if defined(ESP32)
        if (validIP(ETH.localIP()))
        {
            ETH.linkUp();
            ret = true;
        }
#elif defined(ESP8266) || defined(CORE_ARDUINO_PICO)

        if (!net.eth)
            return false;

#if defined(ESP8266) && defined(ESP8266_CORE_SDK_V3_X_X)

#if defined(GSHEET_INC_ENC28J60_LWIP)
        if (net.eth->enc28j60)
        {
            ret = net.eth->enc28j60->status() == WL_CONNECTED;
            goto ex;
        }
#endif
#if defined(GSHEET_INC_W5100_LWIP)
        if (net.eth->w5100)
        {
            ret = net.eth->w5100->status() == WL_CONNECTED;
            goto ex;
        }
#endif
#if defined(GSHEET_INC_W5500_LWIP)
        if (net.eth->w5500)
        {
            ret = net.eth->w5500->status() == WL_CONNECTED;
            goto ex;
        }
#endif

#elif defined(CORE_ARDUINO_PICO)

#endif

#endif

        return ret;

#if defined(GSHEET_INC_ENC28J60_LWIP) || defined(GSHEET_INC_W5100_LWIP) || defined(GSHEET_INC_W5500_LWIP)
    ex:
#endif

        // workaround for ESP8266 Ethernet
        delayMicroseconds(0);

        return ret;
#endif

#endif

        return ret;
    }

    /**
     * Checking for valid IP.
     * @return true for valid.
     */
    bool validIP(IPAddress ip)
    {
        char buf[16];
        sprintf(buf, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
        return strcmp(buf, "0.0.0.0") != 0;
    }

    bool gprsConnect(gsheet_async_data_item_t *sData)
    {
#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE)
        TinyGsm *gsmModem = (TinyGsm *)net.gsm.modem;
        if (gsmModem)
        {
            // Unlock your SIM card with a PIN if needed
            if (net.gsm.pin.length() && gsmModem->getSimStatus() != 3)
                gsmModem->simUnlock(net.gsm.pin.c_str());

#if defined(TINY_GSM_MODEM_XBEE)
            // The XBee must run the gprsConnect function BEFORE waiting for network!
            gsmModem->gprsConnect(_apn.c_str(), _user.c_str(), _password.c_str());
#endif
            if (netErrState == 0 && sData)
                sData->aResult.setDebug(FPSTR("Waiting for network..."));
            if (!gsmModem->waitForNetwork())
            {
                if (netErrState == 0 && sData)
                    sData->aResult.setDebug(FPSTR("Network connection failed"));
                netErrState = 1;
                net.network_status = false;
                return false;
            }

            if (netErrState == 0 && sData)
                sData->aResult.setDebug(FPSTR("Network connected"));

            if (gsmModem->isNetworkConnected())
            {

                if (netErrState == 0 && sData)
                {
                    String debug = FPSTR("Connecting to ");
                    debug += net.gsm.apn.c_str();
                    sData->aResult.setDebug(debug);
                }

                net.network_status = gsmModem->gprsConnect(net.gsm.apn.c_str(), net.gsm.user.c_str(), net.gsm.password.c_str()) &&
                                     gsmModem->isGprsConnected();

                if (netErrState == 0 && sData)
                {
                    if (net.network_status)
                        sData->aResult.setDebug(FPSTR("GPRS/EPS connected"));
                    else
                        sData->aResult.setDebug(FPSTR("GPRS/EPS connection failed"));
                }
            }

            if (!net.network_status)
                netErrState = 1;

            return net.network_status;
        }

#endif
        return false;
    }

    bool gprsConnected()
    {
#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE)
        TinyGsm *gsmModem = (TinyGsm *)net.gsm.modem;
        net.network_status = gsmModem && gsmModem->isGprsConnected();
#endif
        return net.network_status;
    }

    bool gprsDisconnect()
    {
#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE)
        TinyGsm *gsmModem = (TinyGsm *)net.gsm.modem;
        net.network_status = gsmModem && gsmModem->gprsDisconnect();
#endif
        return !net.network_status;
    }

    bool ethernetConnect(gsheet_async_data_item_t *sData)
    {
        bool ret = false;

#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(ENABLE_ETHERNET_NETWORK)

        if (net.ethernet.ethernet_cs_pin > -1)
            GSHEET_ETH_MODULE_CLASS.init(net.ethernet.ethernet_cs_pin);

        if (net.ethernet.ethernet_reset_pin > -1)
        {
            if (sData)
                sData->aResult.setDebug(FPSTR("Resetting Ethernet Board..."));

            pinMode(net.ethernet.ethernet_reset_pin, OUTPUT);
            digitalWrite(net.ethernet.ethernet_reset_pin, HIGH);
            delay(200);
            digitalWrite(net.ethernet.ethernet_reset_pin, LOW);
            delay(50);
            digitalWrite(net.ethernet.ethernet_reset_pin, HIGH);
            delay(200);
        }

        if (sData)
            sData->aResult.setDebug(FPSTR("Starting Ethernet connection..."));

        if (net.ethernet.static_ip)
        {

            if (net.ethernet.static_ip->optional == false)
                GSHEET_ETH_MODULE_CLASS.begin(net.ethernet.ethernet_mac, net.ethernet.static_ip->ipAddress, net.ethernet.static_ip->dnsServer, net.ethernet.static_ip->defaultGateway, net.ethernet.static_ip->netMask);
            else if (!GSHEET_ETH_MODULE_CLASS.begin(net.ethernet.ethernet_mac))
            {
                GSHEET_ETH_MODULE_CLASS.begin(net.ethernet.ethernet_mac, net.ethernet.static_ip->ipAddress, net.ethernet.static_ip->dnsServer, net.ethernet.static_ip->defaultGateway, net.ethernet.static_ip->netMask);
            }
        }
        else
            GSHEET_ETH_MODULE_CLASS.begin(net.ethernet.ethernet_mac);

        net.eth_timer.feed(GSHEET_ETHERNET_MODULE_TIMEOUT);

        while (GSHEET_ETH_MODULE_CLASS.linkStatus() == LinkOFF && net.eth_timer.remaining() > 0)
        {
            delay(100);
        }

        ret = ethernetConnected();

        if (ret && sData)
        {
            String debug = FPSTR("Starting Ethernet connection...");
            debug += GSHEET_ETH_MODULE_CLASS.localIP();
            sData->aResult.setDebug(debug);
        }

        if (!ret && sData)
            sData->aResult.setDebug(FPSTR("Can't connect to network"));

#endif

        return ret;
    }

    bool ethernetConnected()
    {
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE)
        net.network_status = GSHEET_ETH_MODULE_CLASS.linkStatus() == LinkON && validIP(GSHEET_ETH_MODULE_CLASS.localIP());
        if (!net.network_status)
        {
            delay(GSHEET_ETHERNET_MODULE_TIMEOUT);
            net.network_status = GSHEET_ETH_MODULE_CLASS.linkStatus() == LinkON && validIP(GSHEET_ETH_MODULE_CLASS.localIP());
        }
#endif
        return net.network_status;
    }

    bool netConnect(gsheet_async_data_item_t *sData)
    {
        if (!netStatus(sData))
        {
            bool recon = net.reconnect;

            if (net.wifi && net.net_timer.feedCount() == 0)
                recon = true;

            if (recon && (net.net_timer.remaining() == 0))
            {
                net.net_timer.feed(GSHEET_NET_RECONNECT_TIMEOUT_SEC);

                if (sData)
                    sData->aResult.setDebug(FPSTR("Reconnecting to network..."));

                if (net.network_data_type == gsheet_network_data_generic_network)
                {
#if defined(GSHEET_HAS_WIFI_DISCONNECT)
                    // We can reconnect WiFi when device connected via built-in WiFi that supports reconnect
                    if (GSHEET_WIFI_CONNECTED)
                    {
                        WiFi.reconnect();
                        return netStatus(sData);
                    }
#endif
                    if (net.generic.net_con_cb)
                        net.generic.net_con_cb();
                }
                else if (net.network_data_type == gsheet_network_data_gsm_network)
                {
                    gprsDisconnect();
                    gprsConnect(sData);
                }
                else if (net.network_data_type == gsheet_network_data_ethernet_network)
                {
                    ethernetConnect(sData);
                }
                else if (net.network_data_type == gsheet_network_data_default_network)
                {

#if defined(GSHEET_WIFI_IS_AVAILABLE)
#if defined(ESP32) || defined(ESP8266)
                    if (net.wifi && net.wifi->credentials.size())
                        net.wifi->reconnect();
                    else
                        WiFi.reconnect();
#else
                    if (net.wifi && net.wifi->credentials.size())
                        net.wifi->reconnect();
#endif
#endif
                }
            }
        }

        return netStatus(sData);
    }

    bool netStatus(gsheet_async_data_item_t *sData)
    {
        // We will not invoke the network status request when device has built-in WiFi or Ethernet and it is connected.
        if (net.network_data_type == gsheet_network_data_gsm_network)
        {
            net.network_status = gprsConnected();
            if (!net.network_status)
                gprsConnect(sData);
        }
        else if (net.network_data_type == gsheet_network_data_ethernet_network)
        {
            if (!ethernetConnected())
                ethernetConnect(sData);
        }
        // also check the native network before calling external cb
        else if (net.network_data_type == gsheet_network_data_default_network || GSHEET_WIFI_CONNECTED || ethLinkUp())
            net.network_status = GSHEET_WIFI_CONNECTED || ethLinkUp();
        else if (net.network_data_type == gsheet_network_data_generic_network)
        {
            if (!net.generic.net_status_cb)
                netErrState = 1;
            else
                net.generic.net_status_cb(net.network_status);
        }
        else
            net.network_status = false;

        return net.network_status;
    }

    int sIndex(gsheet_slot_options_t &options)
    {
        int slot = -1;
        if (options.auth_used)
            slot = 0;
        else
        {
            int auth_index = -1;
            for (size_t i = 0; i < sVec.size(); i++)
            {
                if (getData(i))
                {
                    if (getData(i)->auth_used)
                        auth_index = i;
                }
            }

            if (auth_index > -1)
                slot = auth_index + 1;

            // Multiple SSE modes
            if (sVec.size() >= GSHEET_ASYNC_QUEUE_LIMIT)
                slot = -2;

            if (slot >= (int)sVec.size())
                slot = -1;
        }

        return slot;
    }

    void setContentType(gsheet_async_data_item_t *sData, const String &type)
    {
        sData->request.addContentTypeHeader(type.c_str());
    }

    void setFileContentLength(gsheet_async_data_item_t *sData, int headerLen = 0, const String &customHeader = "")
    {
#if defined(GSHEET_ENABLE_FS)
        if ((sData->request.file_data.cb && sData->request.file_data.filename.length()) || (sData->request.file_data.data_size && sData->request.file_data.data))
        {
            GSheetBase64Util but;
            size_t sz = 0;
            if (sData->request.file_data.cb)
            {
                sData->request.file_data.cb(sData->request.file_data.file, sData->request.file_data.filename.c_str(), gsheet_file_mode_open_read);
                sz = sData->request.file_data.file.size();
            }
            else
                sz = sData->request.file_data.data_size;

            sData->request.file_data.file_size = sData->request.base64 ? 2 + but.getBase64Len(sz) : sz;
            if (customHeader.length())
            {
                sData->request.val[gsheet_req_hndlr_ns::header] += customHeader;
                sData->request.val[gsheet_req_hndlr_ns::header] += ":";
                sData->request.val[gsheet_req_hndlr_ns::header] += sData->request.file_data.file_size + headerLen;
                sData->request.val[gsheet_req_hndlr_ns::header] += "\r\n";
            }
            else
                setContentLength(sData, sData->request.file_data.file_size);

            closeFile(sData);
        }
#endif
    }

    uint8_t slotCount() const { return sVec.size(); }

    bool processLocked()
    {
        if (inProcess)
            return true;
        inProcess = true;
        return false;
    }

    bool handleSendTimeout(gsheet_async_data_item_t *sData)
    {
        if (sData->request.send_timer.remaining() == 0 || sData->cancel)
        {
            setAsyncError(sData, sData->state, GSHEET_ERROR_TCP_SEND, true, false);
            sData->return_type = gsheet_function_return_type_failure;
            return true;
        }
        return false;
    }

    bool handleReadTimeout(gsheet_async_data_item_t *sData)
    {
        if (sData->response.read_timer.remaining() == 0 || sData->cancel)
        {
            setAsyncError(sData, sData->state, GSHEET_ERROR_TCP_RECEIVE_TIMEOUT, true, false);
            sData->return_type = gsheet_function_return_type_failure;
            return true;
        }
        return false;
    }

    void handleProcessFailure(gsheet_async_data_item_t *sData)
    {
        if (sData->return_type == gsheet_function_return_type_failure)
        {
            if (sData->async)
                returnResult(sData, false);
            reset(sData, false);
        }
    }

    String getHost(gsheet_async_data_item_t *sData, bool fromReq, String *ext = nullptr)
    {
        String url = fromReq ? sData->request.val[gsheet_req_hndlr_ns::url] : sData->response.val[gsheet_res_hndlr_ns::location];

        GSheetURLUtil uut;
        return uut.getHost(url, ext);
    }

    void stopAsyncImpl(bool all = false, const String &uid = "")
    {
        if (inStopAsync)
            return;

        inStopAsync = true;
        size_t size = slotCount();
        if (size)
        {
            for (int i = size - 1; i >= 0; i--)
            {
                gsheet_sys_idle();
                gsheet_async_data_item_t *sData = getData(i);
                if (sData && sData->async && !sData->auth_used && !sData->cancel)
                {
                    if (uid.length())
                    {
                        if (strcmp(sData->aResult.uid().c_str(), uid.c_str()) == 0)
                            sData->cancel = true;
                    }
                    else
                    {
                        sData->cancel = true;
                        if (!all)
                            break;
                    }
                }
            }
        }

        inStopAsync = false;
    }

public:
    GSheetAsyncClientClass(Client &client, gsheet_network_config_data &net) : client(&client)
    {
        this->net.copy(net);
        this->addr = reinterpret_cast<uint32_t>(this);
        GSheetList list;
        client_type = gsheet_async_request_handler_t::tcp_client_type_sync;
        list.addRemoveList(cVec, addr, true);
    }

#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)
    GSheetAsyncClientClass(GSheetAsyncTCPConfig &tcpClientConfig, gsheet_network_config_data &net) : async_tcp_config(&tcpClientConfig)
    {
        this->net.copy(net);
        this->addr = reinterpret_cast<uint32_t>(this);
        List list;
        client_type = gsheet_async_request_handler_t::tcp_client_type_async;
        list.addRemoveList(cVec, addr, true);
    }
#endif

    ~GSheetAsyncClientClass()
    {
        stop(nullptr);

        for (size_t i = 0; i < sVec.size(); i++)
        {
            reset(getData(i), true);
            gsheet_async_data_item_t *sData = getData(i);
            if (!sData->auth_used)
                delete sData;
            sData = nullptr;
        }

        GSheetList list;
        list.addRemoveList(cVec, addr, false);
    }

    bool networkStatus() { return netStatus(nullptr); }

    void stopAsync(bool all = false) { stopAsyncImpl(all); }
    void stopAsync(const String &uid) { stopAsyncImpl(false, uid); }

    void stop(gsheet_async_data_item_t *sData)
    {
        if (sData)
            sData->aResult.setDebug(FPSTR("Terminating the server connection..."));
        if (client_type == gsheet_async_request_handler_t::tcp_client_type_sync)
        {
            if (client)
                client->stop();
        }
        else
        {
#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)
            if (async_tcp_config && async_tcp_config->tcpStop)
                async_tcp_config->tcpStop();
#endif
        }
    }

    GSheetError lastError() const { return lastErr; }

    gsheet_async_data_item_t *createSlot(gsheet_slot_options_t &options)
    {
        int slot_index = sIndex(options);

        gsheet_async_data_item_t *sData = addSlot(slot_index);
        sData->reset();
        return sData;
    }

    void newRequest(gsheet_async_data_item_t *sData, const String &url, const String &path, const String &extras, gsheet_async_request_handler_t::http_request_method method, gsheet_slot_options_t &options, const String &uid)
    {
        sData->async = options.async;
        sData->request.val[gsheet_req_hndlr_ns::url] = url;
        sData->request.val[gsheet_req_hndlr_ns::path] = path;
        sData->request.method = method;

        sData->aResult.val[gsheet_ares_ns::res_uid] = uid;
        clear(sData->request.val[gsheet_req_hndlr_ns::header]);
        sData->request.addRequestHeaderFirst(method);
        if (path.length() == 0)
            sData->request.val[gsheet_req_hndlr_ns::header] += '/';
        else if (path.length() && path[0] != '/')
            sData->request.val[gsheet_req_hndlr_ns::header] += '/';
        sData->request.val[gsheet_req_hndlr_ns::header] += path;
        sData->request.val[gsheet_req_hndlr_ns::header] += extras;
        sData->request.addRequestHeaderLast();
        sData->request.addHostHeader(getHost(sData, true).c_str());

        sData->auth_used = options.auth_used;

        if (!options.auth_used)
        {
            sData->request.app_token = options.app_token;
            if (options.app_token && !options.auth_param && (options.app_token->auth_type == auth_access_token || options.app_token->auth_type == auth_sa_access_token))
            {
                sData->request.addAuthHeaderFirst(options.app_token->auth_type);
                sData->request.val[gsheet_req_hndlr_ns::header] += GSHEET_AUTH_PLACEHOLDER;
                sData->request.addNewLine();
            }

            sData->request.val[gsheet_req_hndlr_ns::header] += FPSTR("Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0");
            sData->request.addNewLine();
            sData->request.addConnectionHeader(true);

            if (sData->request.val[gsheet_req_hndlr_ns::etag].length() > 0 && (method == gsheet_async_request_handler_t::http_put || method == gsheet_async_request_handler_t::http_delete))
            {
                sData->request.val[gsheet_req_hndlr_ns::header] += FPSTR("if-match: ");
                sData->request.val[gsheet_req_hndlr_ns::header] += sData->request.val[gsheet_req_hndlr_ns::etag];
                sData->request.addNewLine();
            }
        }

        if (method == gsheet_async_request_handler_t::http_get || method == gsheet_async_request_handler_t::http_delete)
            sData->request.addNewLine();
    }

    void setAuthTs(uint32_t ts) { auth_ts = ts; }

    void setContentLength(gsheet_async_data_item_t *sData, size_t len)
    {
        if (sData->request.method == gsheet_async_request_handler_t::http_post || sData->request.method == gsheet_async_request_handler_t::http_put || sData->request.method == gsheet_async_request_handler_t::http_patch)
        {
            sData->request.addContentLengthHeader(len);
            sData->request.addNewLine();
        }
    }

    void process(bool async)
    {
        if (processLocked())
            return;

        if (slotCount())
        {
            size_t slot = 0;
            gsheet_async_data_item_t *sData = getData(slot);

            if (!sData)
            {
                inProcess = false;
                return;
            }

            if (!netConnect(sData))
            {
                setAsyncError(sData, sData->state, GSHEET_ERROR_TCP_DISCONNECTED, true, false);
                if (sData->async)
                {
                    returnResult(sData, false);
                    reset(sData, true);
                }
                inProcess = false;
                return;
            }

            if (sData->async && !async)
            {
                inProcess = false;
                return;
            }

            bool sending = false;
            if (sData->state == gsheet_async_state_undefined || sData->state == gsheet_async_state_send_header || sData->state == gsheet_async_state_send_payload)
            {
                sData->response.clear();
                sData->request.feedTimer();
                sending = true;
                sData->return_type = send(sData);

                while (sData->state == gsheet_async_state_send_header || sData->state == gsheet_async_state_send_payload)
                {
                    sData->return_type = send(sData);
                    sData->response.feedTimer();
                    handleSendTimeout(sData);
                    if (sData->async || sData->return_type == gsheet_function_return_type_failure)
                        break;
                }
            }

            if (sending)
            {
                handleSendTimeout(sData);
                if (sData->async && sData->return_type == gsheet_function_return_type_continue)
                {
                    inProcess = false;
                    return;
                }
            }

            gsheet_sys_idle();

            if (sData->state == gsheet_async_state_read_response)
            {
                // if (!sData->download && !sData->upload)
                //    sData->request.clear();

                // it can be complete response from payload sending
                if (sData->return_type == gsheet_function_return_type_complete)
                    sData->return_type = gsheet_function_return_type_continue;

                if (sData->async && !sData->response.tcpAvailable(client_type, client, async_tcp_config))
                {

                    handleReadTimeout(sData);
                    inProcess = false;
                    return;
                }
                else if (!sData->async) // wait for non async
                {
                    while (!sData->response.tcpAvailable(client_type, client, async_tcp_config) && netConnect(sData))
                    {
                        gsheet_sys_idle();
                        if (handleReadTimeout(sData))
                            break;
                    }
                }
            }

            // Read until status code > 0, header finished and payload read complete
            if (sData->state == gsheet_async_state_read_response)
            {
                sData->error.code = 0;
                while (sData->return_type == gsheet_function_return_type_continue && (sData->response.httpCode == 0 || sData->response.flags.header_remaining || sData->response.flags.payload_remaining))
                {
                    sData->response.feedTimer();
                    sData->return_type = receive(sData);

                    handleReadTimeout(sData);

                    bool allRead = sData->response.httpCode > 0 && sData->response.httpCode != GSHEET_ERROR_HTTP_CODE_OK && !sData->response.flags.header_remaining && !sData->response.flags.payload_remaining;
                    if (allRead && sData->response.httpCode >= GSHEET_ERROR_HTTP_CODE_BAD_REQUEST)
                    {
                        sData->return_type = gsheet_function_return_type_failure;
                    }

                    if (sData->async || allRead || sData->return_type == gsheet_function_return_type_failure)
                        break;
                }
            }

            handleProcessFailure(sData);

            setAsyncError(sData, sData->state, 0, sData->return_type == gsheet_function_return_type_complete, false);

            if (sData->to_remove)
                removeSlot(slot);
        }

        inProcess = false;
    }
    void handleRemove()
    {
        for (size_t slot = 0; slot < slotCount(); slot++)
        {
            gsheet_async_data_item_t *sData = getData(slot);
            if (sData && sData->to_remove)
                removeSlot(slot);
        }
    }

    size_t slotCount() { return sVec.size(); }

    void removeSlot(uint8_t slot, bool sse = true)
    {
        gsheet_async_data_item_t *sData = getData(slot);

        if (!sData)
            return;

        closeFile(sData);
        setLastError(sData);
        // data available from sync and asyn request
        returnResult(sData, true);
        reset(sData, sData->auth_used);
        if (!sData->auth_used)
            delete sData;
        sData = nullptr;
        sVec.erase(sVec.begin() + slot);
    }
};

#endif