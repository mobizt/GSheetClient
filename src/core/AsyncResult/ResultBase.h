/**
 * Created May 18, 2024
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
#ifndef GSHEET_RESULT_BASE_H_
#define GSHEET_RESULT_BASE_H_

#define GSHEET_CHUNK_SIZE 2048
#define GSHEET_BASE64_CHUNK_SIZE 1026

#include <Arduino.h>
#include "./core/AsyncResult/AppEvent.h"
#include "./core/AsyncResult/AppDebug.h"
#include "./core/AsyncResult/AppData.h"

namespace gsheet
{

    class GSheetResultBase : public gsheet_app_data_t
    {

    public:
        GSheetResultBase() {}

    protected:

        void updateEvent(gsheet_app_event_t &app_event) { app_event.update(); }

        void setEventBase(gsheet_app_event_t &app_event, int code, const String &msg) { app_event.setEvent(code, msg); }

        void resetEvent(gsheet_app_event_t &app_event) { app_event.reset(); }

        void updateDebug(gsheet_app_debug_t &app_debug) { app_debug.update(); }

        void setDebugBase(gsheet_app_debug_t &app_debug, const String &msg) { app_debug.setDebug(msg); }

        void resetDebug(gsheet_app_debug_t &app_debug) { app_debug.reset(); }

        bool isDebugBase(gsheet_app_debug_t &app_debug) { return app_debug.isDebug(); }

        bool isEventBase(gsheet_app_event_t &app_event) { return app_event.isEvent(); }

        void clearAppData(gsheet_app_data_t &app_data) { reset(app_data); }
    };

}

#endif