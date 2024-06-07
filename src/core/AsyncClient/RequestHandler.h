/**
 * Created May 31, 2024
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
#ifndef GSHEET_ASYNC_REQUEST_HANDLER_H
#define GSHEET_ASYNC_REQUEST_HANDLER_H
#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/FileConfig.h"
#include "./core/Timer.h"
#include "Client.h"
#include "./core/AuthConfig.h"

#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)
#include "./core/AsyncTCPConfig.h"
#endif

#define GSHEET_TCP_WRITE_TIMEOUT_SEC 30

#define GSHEET_RECONNECTION_TIMEOUT_MSEC 5000

#define GSHEET_AUTH_PLACEHOLDER (const char *)FPSTR("<auth_token>")

#if !defined(GSHEET_ASYNC_QUEUE_LIMIT)
#if defined(ESP8266)
#define GSHEET_ASYNC_QUEUE_LIMIT 10
#elif defined(ESP32) || defined(ARDUINO_PICO_MODULE)
#define GSHEET_ASYNC_QUEUE_LIMIT 20
#else
#define GSHEET_ASYNC_QUEUE_LIMIT 10
#endif
#endif

typedef void (*GSheetNetworkStatus)(bool &status);
typedef void (*GSheetNetworkReconnect)(void);

using namespace gsheet;

namespace gsheet_req_hndlr_ns
{
    enum data_item_type_t
    {
        url,
        path,
        etag,
        header,
        payload,
        max_type
    };
}

struct gsheet_async_request_handler_t
{
public:
    enum tcp_client_type
    {
        tcp_client_type_sync,
        tcp_client_type_async
    };

    enum http_request_method
    {
        http_undefined,
        http_put,
        http_post,
        http_get,
        http_patch,
        http_delete,
    };

    String val[gsheet_req_hndlr_ns::max_type];
    String location;
    gsheet_app_token_t *app_token = nullptr;
    uint16_t port = 443;
    uint8_t *data = nullptr;
    gsheet_file_config_data file_data;
    bool base64 = false;
    bool ota = false;
    uint32_t payloadLen = 0;
    uint32_t dataLen = 0;
    uint32_t payloadIndex = 0;
    uint16_t dataIndex = 0;
    int8_t b64Pad = 0;
    int16_t ota_error = 0;
    http_request_method method = http_undefined;
    GSheetTimer send_timer;

    gsheet_async_request_handler_t()
    {
    }

    void clear()
    {
         
        for (size_t i = 0; i < gsheet_req_hndlr_ns::max_type; i++)
            val[i].remove(0, val[i].length());
        port = 443;
        if (data)
            delete data;
        data = nullptr;
        file_data.clear();
        base64 = false;
        ota = false;
        payloadLen = 0;
        dataLen = 0;
        payloadIndex = 0;
        dataIndex = 0;
        b64Pad = 0;
        ota_error = 0;
        method = http_undefined;
    }

    void addNewLine()
    {
       val[gsheet_req_hndlr_ns::header] += "\r\n";
    }

    void addGAPIsHost(String &str, PGM_P sub)
    {
        str += sub;
        if (str[str.length() - 1] != '.')
            str += ".";
        str += FPSTR("googleapis.com");
    }

    void addGAPIsHostHeader(PGM_P sub)
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR("Host: ");
        addGAPIsHost(val[gsheet_req_hndlr_ns::header], sub);
        addNewLine();
    }

    void addHostHeader(PGM_P host)
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR("Host: ");
        val[gsheet_req_hndlr_ns::header] += host;
        addNewLine();
    }

    void addContentTypeHeader(PGM_P v)
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR("Content-Type: ");
        val[gsheet_req_hndlr_ns::header] += v;
        addNewLine();
    }

    void addContentLengthHeader(size_t len)
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR("Content-Length: ");
        val[gsheet_req_hndlr_ns::header] += len;
        addNewLine();
    }

    void addUAHeader()
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR("User-Agent: ESP");
        addNewLine();
    }

    void addConnectionHeader(bool keepAlive)
    {
        val[gsheet_req_hndlr_ns::header] += keepAlive ? FPSTR("Connection: keep-alive") : FPSTR("Connection: close");
        addNewLine();
    }

    /* Append the string with first request line (HTTP method) */
    bool addRequestHeaderFirst(gsheet_async_request_handler_t::http_request_method method)
    {
        bool post = false;
        switch (method)
        {
        case gsheet_async_request_handler_t::http_get:
            val[gsheet_req_hndlr_ns::header] += FPSTR("GET");
            break;
        case gsheet_async_request_handler_t::http_post:
            val[gsheet_req_hndlr_ns::header] += FPSTR("POST");
            post = true;
            break;

        case gsheet_async_request_handler_t::http_patch:
            val[gsheet_req_hndlr_ns::header] += FPSTR("PATCH");
            post = true;
            break;

        case gsheet_async_request_handler_t::http_delete:
            val[gsheet_req_hndlr_ns::header] += FPSTR("DELETE");
            break;

        case gsheet_async_request_handler_t::http_put:
            val[gsheet_req_hndlr_ns::header] += FPSTR("PUT");
            break;

        default:
            break;
        }

        if (method == gsheet_async_request_handler_t::http_get || method == gsheet_async_request_handler_t::http_post || method == gsheet_async_request_handler_t::http_patch || method == gsheet_async_request_handler_t::http_delete || method == gsheet_async_request_handler_t::http_put)
            val[gsheet_req_hndlr_ns::header] += FPSTR(" ");

        return post;
    }

    /* Append the string with last request line (HTTP version) */
    void addRequestHeaderLast()
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR(" HTTP/1.1\r\n");
    }

    /* Append the string with first part of Authorization header */
    void addAuthHeaderFirst(gsheet_auth_token_type type)
    {
        val[gsheet_req_hndlr_ns::header] += FPSTR("Authorization: ");
        if (type == gsheet_auth_access_token || type == gsheet_auth_sa_access_token)
            val[gsheet_req_hndlr_ns::header] += FPSTR("Bearer ");
        else
            val[gsheet_req_hndlr_ns::header] += FPSTR("key=");
    }

    void feedTimer(int interval = -1)
    {
        send_timer.feed(interval == -1 ? GSHEET_TCP_WRITE_TIMEOUT_SEC : interval);
    }

    size_t tcpWrite(gsheet_async_request_handler_t::tcp_client_type client_type, Client *client, void *atcp_config, uint8_t *data, size_t size)
    {
        if (client_type == tcp_client_type_sync)
            return client ? client->write(data, size) : 0;
        else
        {

#if defined(GSHEET_ENABLE_ASYNC_TCP_CLIENT)

            GSheetAsyncTCPConfig *async_tcp_config = reinterpret_cast<GSheetAsyncTCPConfig *>(atcp_config);
            if (!async_tcp_config && !async_tcp_config->tcpSend)
                return 0;

            uint32_t sent = 0;
            async_tcp_config->tcpSend(data, size, sent);
            return sent;
#endif
        }
        return 0;
    }
};

#endif