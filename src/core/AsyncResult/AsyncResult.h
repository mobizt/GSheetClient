/**
 * Created May 30, 2024
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *4
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
#ifndef GSHEET_ASYNC_RESULT_H
#define GSHEET_ASYNC_RESULT_H
#include "Value.h"
#include "./core/Error.h"
#include "./core/List.h"
#include "./core/Timer.h"
#include "./core/StringUtil.h"
#include "./core/AsyncResult/AppEvent.h"
#include "./core/AsyncResult/AppDebug.h"
#include "./core/AsyncResult/ResultBase.h"
#include "./core/AsyncResult/AppData.h"

#define GSHEET_CHUNK_SIZE 2048
#define GSHEET_BASE64_CHUNK_SIZE 1026
#define GSHEET_SSE_TIMEOUT 40 * 1000

using namespace gsheet;

namespace gsheet_ares_ns
{
    enum data_item_type_t
    {
        res_uid,
        data_path,
        data_payload,
        debug_info,
        max_type
    };
}

class GSheetAsyncResult : public GSheetResultBase
{
    friend class GSheetAsyncClientClass;
    friend class GSheetAppBase;
    friend class gsheet_async_data_item_t;

private:
    uint32_t addr = 0;
    uint32_t rvec_addr = 0;
    String val[gsheet_ares_ns::max_type];

    void setPayload(const String &data)
    {
        if (data.length())
        {
            app_data.setData();
            val[gsheet_ares_ns::data_payload] = data;
        }
    }

    void updateData()
    {
        app_data.update();
    }

    void setDebug(const String &msg)
    {
        if (app_debug)
            setDebugBase(*app_debug, msg);
    }

    void setUID(const String &uid = "")
    {
        if (uid.length())
            val[gsheet_ares_ns::res_uid] = uid;
        else
        {
            val[gsheet_ares_ns::res_uid] = FPSTR("task_");
            val[gsheet_ares_ns::res_uid] += String(millis());
        }
    }

    gsheet_app_debug_t *app_debug = nullptr;
    gsheet_app_event_t *app_event = nullptr;

    // This required by appEvent() that returns the reference to the app_event object which is not yet initialized.
    gsheet_app_event_t ev;
    GSheetError lastError;
    gsheet_app_data_t app_data;

public:
    GSheetAsyncResult()
    {
        addr = reinterpret_cast<uint32_t>(this);
        setUID();
    };

    ~GSheetAsyncResult()
    {
        if (rvec_addr > 0)
        {
            std::vector<uint32_t> *rVec = reinterpret_cast<std::vector<uint32_t> *>(rvec_addr);
            if (rVec)
            {
                GSheetList vec;
                addr = reinterpret_cast<uint32_t>(this);
                vec.addRemoveList(*rVec, addr, false);
            }
        }
    };

    /**
     * Get the pointer to the internal response payload string buffer.
     *
     * @return const char * The pointer to internal response payload string.
     */
    const char *c_str() { return val[gsheet_ares_ns::data_payload].c_str(); }

    /**
     * Get the copy of server response payload string.
     *
     * @return String The copy of payload string.
     */
    String payload() const { return val[gsheet_ares_ns::data_payload].c_str(); }

    /**
     * Get the path of the resource of the request.
     *
     * @return String The path of the resource of the request.
     */
    String path() const { return val[gsheet_ares_ns::data_path].c_str(); }

    /**
     * Get the unique identifier of async task.
     *
     * @return String The UID of async task.
     */
    String uid() const { return val[gsheet_ares_ns::res_uid].c_str(); }

    /**
     * Get the debug information.
     *
     * @return String The debug information.
     */
    String debug()
    {
        if (app_debug)
            return app_debug->message();
        return "";
    }

    /**
     * Clear the async result.
     */
    void clear()
    {
        for (size_t i = 0; i < gsheet_ares_ns::max_type; i++)
            val[i].remove(0, val[i].length());

        lastError.reset();

        if (app_debug)
            resetDebug(*app_debug);

        if (app_event)
            resetEvent(*app_event);

        app_data.reset();
    }

    /**
     * Get the number of bytes of available response payload.
     * @return int The number of bytes available.
     */
    int available()
    {
        if (app_data.isData())
            return val[gsheet_ares_ns::data_payload].length();
        return 0;
    }

    /**
     * Get the reference of internal app event information.
     *
     * @return app_event_t The reference of internal app event.
     */
    gsheet_app_event_t &appEvent()
    {
        if (app_event)
            return *app_event;
        return ev;
    }

    /**
     * Check if the error occurred in async task.
     *
     * @return bool Returns true if error occurred.
     */
    bool isError() { return lastError.isError() && lastError.code() != 0 && lastError.code() != GSHEET_ERROR_HTTP_CODE_OK; }

    /**
     * Check if the debug information in available.
     *
     * @return bool Returns true if debug information in available.
     */
    bool isDebug()
    {
        if (app_debug)
            return isDebugBase(*app_debug);
        return false;
    }

    /**
     * Check if the app event information in available.
     *
     * @return bool Returns true if app event information in available.
     */
    bool isEvent()
    {
        if (app_event)
            return isEventBase(*app_event);
        return false;
    }

    /**
     * Get the reference of internal GSheetError object.
     *
     * @return GSheetError The internal GSheetError object.
     */
    GSheetError &error() { return lastError; }
};

typedef void (*GSheetAsyncResultCallback)(GSheetAsyncResult &aResult);

#endif