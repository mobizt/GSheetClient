/**
 * Created May 30, 2024
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
#ifndef GSHEET_CORE_JWT_H
#define GSHEET_CORE_JWT_H

#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/Memory.h"
#include "./core/AuthConfig.h"
#include "./core/Base64.h"
#include "./core/URL.h"
#include "./core/JSON.h"
#include "./core/Error.h"
#include "./core/Core.h"
#include "./core/Timer.h"

#if defined(GSHEET_ENABLE_JWT)

#if __has_include(<ESP_SSLClient.h>)
#include <ESP_SSLClient.h>
#else
#include "./client/SSLClient/ESP_SSLClient.h"
#endif

using namespace gsheet;

namespace gsheet
{
    static void jwt_add_gapis_host(String &str, PGM_P sub)
    {
        str += sub;
        if (str[str.length() - 1] != '.')
            str += ".";
        str += FPSTR("googleapis.com");
    }
    static void jwt_add_sp(String &buf) { buf += ' '; }

    struct jwt_token_data_t
    {
    public:
        String token;
        int err_code = 0;
        String msg;
        String pk;
        char *hash = nullptr;               // SHA256 size (256 bits or 32 bytes)
        unsigned char *signature = nullptr; // 256 bytes
    };

    class GSheetJWTClass
    {
        friend class GSheetApp;
        friend class GSheetClient;

    private:
        GSheetMemory mem;
        GSheetStringUtil sut;
        GSheetBase64Util but;
        GSheetURLUtil uut;
        String payload;
        GSheetJSONUtil json;
        jwt_token_data_t jwt_data;
        GSheetTimer err_timer;
        auth_data_t *auth_data = nullptr;
        bool processing = false;
        gsheet_app_debug_t *app_debug = nullptr;

        bool exit(bool ret)
        {
            processing = false;
            if (jwt_data.signature)
                mem.release(&jwt_data.signature);
            if (jwt_data.hash && !ret)
                mem.release(&jwt_data.hash);
            return ret;
        }

        bool begin(auth_data_t *auth_data);
        bool create();
        void sendErrCB(GSheetAsyncResultCallback cb, GSheetAsyncResult *aResult = nullptr);
        void sendErrResult(GSheetAsyncResult *refResult);
        void setAppDebug(gsheet_app_debug_t *app_debug);
        const char *token();
        bool ready();
        void clear();

    public:
        GSheetJWTClass();
        ~GSheetJWTClass();

        /**
         * Process the JWT token generation and returns the status.
         *
         * @return boolean of JWT processor result.
         */
        bool loop(auth_data_t *auth_data);
    };

}

#endif

#endif