/**
 * Created March 26, 2024
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

#ifndef GSHEET_CLIENT_H
#define GSHEET_CLIENT_H

#if defined(GSHEET_CLIENT_VERSION)
#undef GSHEET_CLIENT_VERSION
#endif

#define GSHEET_CLIENT_VERSION "0.0.1"

#include <Arduino.h>
#include "./core/GSheetApp.h"
#include "./core/AsyncClient/AsyncClient.h"
#include "./spreadsheets/GSheetBase.h"

using namespace gsheet;

namespace gsheet
{
    class GSheetClient: public GSheetBase
    {
    private:
        void configApp(GSheetAsyncClientClass &aClient, GSheetApp &app, user_auth_data &auth, core_auth_task_type task_type = core_auth_task_type_undefined)
        {
            app.aClient = &aClient;
            app.aclient_addr = reinterpret_cast<uint32_t>(&aClient);
            app.auth_data.user_auth.copy(auth);

            app.auth_data.app_token.clear();
            app.auth_data.app_token.auth_type = app.auth_data.user_auth.auth_type;
            app.auth_data.app_token.auth_data_type = app.auth_data.user_auth.auth_data_type;

            app.auth_data.app_token.authenticated = false;
            app.auth_data.user_auth.task_type = task_type;
        }

        void resetTimer(GSheetApp &app, bool start, int interval = 0, int exp = -1)
        {
            app.err_timer.feed(0);
            app.auth_timer.stop();
            app.auth_timer.setInterval(interval);
            app.expire = exp == -1 ? interval : exp;
            if (start)
                app.auth_timer.start();
        }

    public:
        GSheetClient(){};
        ~GSheetClient(){};

        void initializeApp(GSheetAsyncClientClass &aClient, GSheetApp &app, user_auth_data &auth)
        {
            configApp(aClient, app, auth);

            if (app.auth_data.user_auth.auth_type == auth_access_token)
            {
                if (app.auth_data.user_auth.auth_type == auth_access_token)
                {
#if defined(GSHEET_ENABLE_ACCESS_TOKEN)

                    app.auth_data.app_token.expire = app.auth_data.user_auth.access_token.expire;

                    if (app.auth_data.user_auth.access_token.val[gsheet_access_tk_ns::token].length())
                    {
                        app.auth_data.app_token.val[gsheet_app_tk_ns::token] = app.auth_data.user_auth.access_token.val[gsheet_access_tk_ns::token];
                        app.auth_data.app_token.val[gsheet_app_tk_ns::refresh] = app.auth_data.user_auth.access_token.val[gsheet_access_tk_ns::refresh];
                        app.auth_data.app_token.authenticated = app.auth_data.user_auth.initialized;
                    }
                    else
                    {
                        app.auth_data.app_token.val[gsheet_app_tk_ns::refresh] = app.auth_data.user_auth.access_token.val[gsheet_access_tk_ns::refresh];
                        app.auth_data.app_token.authenticated = false;
                    }

                    resetTimer(app, true, app.auth_data.user_auth.access_token.expire);

#endif
                }
            }
            else if (app.auth_data.user_auth.auth_type == auth_sa_access_token)
            {
                app.auth_data.app_token.authenticated = false;
                uint32_t exp = app.auth_data.user_auth.sa.expire;
                resetTimer(app, true, 0, exp);
            }
        }

        void printf(const char *format, ...)
        {
            int size = 2048;
            char s[size];
            va_list va;
            va_start(va, format);
            vsnprintf(s, size, format, va);
            va_end(va);
#if defined(GSHEET_PRINTF_PORT)
            GSHEET_PRINTF_PORT.print(s);
#else
            Serial.print(s);
#endif
        }
    };

}

static GSheetClient GSheet;

namespace GSHEET
{
    template <typename T>
    static user_auth_data &getAuth(T &auth) { return auth.get(); }
    static void initializeApp(GSheetAsyncClientClass &aClient, GSheetApp &app, user_auth_data &auth) { GSheet.initializeApp(aClient, app, auth); }
}

#endif