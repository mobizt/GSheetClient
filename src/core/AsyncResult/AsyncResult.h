/**
 * Created March 26, 2024
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

namespace gsheet
{
    struct app_event_t
    {

    private:
        String ev_msg;
        int ev_code = 0;

    public:
        String message() { return ev_msg; }
        int code() { return ev_code; }
        void setEvent(int code, const String &msg)
        {
            ev_code = code;
            ev_msg = msg;
        }
    };
}

class GSheetAsyncResult
{
    friend class GSheetAsyncClientClass;
    friend class GSheetApp;
    friend class gsheet_async_data_item_t;


private:
    uint32_t addr = 0;
    String val[gsheet_ares_ns::max_type];
    bool debug_info_available = false;


    void setPayload(const String &data)
    {
        if (data.length())
        {
            data_available = true;
            val[gsheet_ares_ns::data_payload] = data;
        }
    }

public:
    bool data_available = false, error_available = false;
    app_event_t app_event;
    GSheetError lastError;

    void setDebug(const String &debug)
    {
        // Keeping old message in case unread.
        if (debug_info_available && val[gsheet_ares_ns::debug_info].length() < 200)
        {
            if (val[gsheet_ares_ns::debug_info].indexOf(debug) == -1)
            {
                val[gsheet_ares_ns::debug_info] += " >> ";
                val[gsheet_ares_ns::debug_info] += debug;
            }
        }
        else
            val[gsheet_ares_ns::debug_info] = debug;
        if (debug.length())
            debug_info_available = true;
    }

    GSheetAsyncResult()
    {
        addr = reinterpret_cast<uint32_t>(this);
        GSheetList vec;
        vec.addRemoveList(rVec, addr, true);
    };
    ~GSheetAsyncResult()
    {
        GSheetList vec;
        vec.addRemoveList(rVec, addr, false);
    };
    const char *c_str() { return val[gsheet_ares_ns::data_payload].c_str(); }
    String payload() const { return val[gsheet_ares_ns::data_payload].c_str(); }
    String uid() const { return val[gsheet_ares_ns::res_uid].c_str(); }
    String debug() const { return val[gsheet_ares_ns::debug_info].c_str(); }
    void clear()
    {
        for (size_t i = 0; i < gsheet_ares_ns::max_type; i++)
            val[i].remove(0, val[i].length());
        debug_info_available = false;
        lastError.setLastError(0, "");
        app_event.setEvent(0, "");
        data_available = false;
    }
 
    int available()
    {
        bool ret = data_available;
        data_available = false;
        return ret ? val[gsheet_ares_ns::data_payload].length() : 0;
    }

    app_event_t appEvent() const { return app_event; }

    bool isError()
    {
        bool err = lastError.code() != 0 && lastError.code() != GSHEET_ERROR_HTTP_CODE_OK;
        if (error_available)
        {
            error_available = false;
            return err;
        }
        return false;
    }

    bool isDebug()
    {
        bool dbg = val[gsheet_ares_ns::debug_info].length() > 0;
        if (debug_info_available)
        {
            debug_info_available = false;
            return dbg;
        }
        return false;
    }

    GSheetError error() const { return lastError; }
};

typedef void (*GSheetAsyncResultCallback)(GSheetAsyncResult &aResult);

#endif