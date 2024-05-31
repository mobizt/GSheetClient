/**
 * Created May 20, 2024
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
#ifndef GSHEET_CORE_APP_H_
#define GSHEET_CORE_APP_H_

#include <Arduino.h>
#include "./core/AsyncResult/AsyncResult.h"

class GSheetAppBase
{

public:
    GSheetAppBase() {}

protected:
    void setResultUID(GSheetAsyncResult *aResult, const String &uid) { aResult->val[gsheet_ares_ns::res_uid] = uid; }

    void setRVec(GSheetAsyncResult *aResult, uint32_t addr) { aResult->rvec_addr = addr; }

    std::vector<uint32_t> &getRVec(GSheetAsyncClientClass *aClient) { return aClient->rVec; }

    gsheet_app_debug_t *getAppDebug(GSheetAsyncClientClass *aClient) { return &aClient->app_debug; }

    void resultSetDebug(GSheetAsyncResult *aResult, gsheet_app_debug_t *app_debug) { aResult->app_debug = app_debug; }

    void resultSetEvent(GSheetAsyncResult *aResult, gsheet_app_event_t *app_event) { aResult->app_event = app_event; }

    gsheet_app_event_t *getAppEvent(GSheetAsyncClientClass *aClient) { return &aClient->app_event; }

    void stopAsync(GSheetAsyncClientClass *aClient, gsheet_async_data_item_t *sData) { aClient->stop(sData); }

    gsheet_async_data_item_t *createSlotBase(GSheetAsyncClientClass *aClient, gsheet_slot_options_t &soption)
    {
        if (aClient)
            return aClient->createSlot(soption);
        return nullptr;
    }

    GSheetAsyncResult *getResultBase(GSheetAsyncClientClass *aClient) { return aClient->getResult(); }

    void newRequestBase(GSheetAsyncClientClass *aClient, gsheet_async_data_item_t *sData, const String &url, const String &path, const String &extras, gsheet_async_request_handler_t::http_request_method method, gsheet_slot_options_t &options, const String &uid) { aClient->newRequest(sData, url, path, extras, method, options, uid); }

    void setAuthTsBase(GSheetAsyncClientClass *aClient, uint32_t ts) { aClient->auth_ts = ts; }

    void addRemoveClientVecBase(GSheetAsyncClientClass *aClient, uint32_t cvec_addr, bool add) { aClient->addRemoveClientVec(cvec_addr, add); }

    void setContentLengthBase(GSheetAsyncClientClass *aClient, gsheet_async_data_item_t *sData, size_t len) { aClient->setContentLength(sData, len); }

    void handleRemoveBase(GSheetAsyncClientClass *aClient) { aClient->handleRemove(); }

    void removeSlotBase(GSheetAsyncClientClass *aClient, uint8_t slot, bool sse = true) { aClient->removeSlot(slot, sse); }

    size_t slotCountBase(GSheetAsyncClientClass *aClient) { return aClient->sVec.size(); }

    void setLastErrorBase(GSheetAsyncResult *aResult, int code, const String &message)
    {
        if (aResult)
            aResult->lastError.setLastError(code, message);
    }

    void clearLastErrorBase(GSheetAsyncResult *aResult)
    {
        if (aResult)
            aResult->lastError.reset();
    }

    void processBase(GSheetAsyncClientClass *aClient, bool async)
    {
        if (!aClient)
            return;

        aClient->process(async);
    }

    template <typename T>
    void setAppBase(T &app, uint32_t app_addr, gsheet_app_token_t *app_token, uint32_t avec_addr) { app.setApp(app_addr, app_token, avec_addr); }
};

#endif