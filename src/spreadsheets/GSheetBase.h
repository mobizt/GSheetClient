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

#ifndef GSHEET_BASE_H
#define GSHEET_BASE_H

#include <Arduino.h>
#include "./core/GSheetApp.h"
#include "./spreadsheets/DataOptions.h"

class GSheetBase
{
private:
public:
    ~GSheetBase(){};

    GSheetBase(const String &url = "")
    {
        this->service_url = url;
    };

    GSheetBase &operator=(GSheetBase &rhs)
    {
        this->service_url = rhs.service_url;
        return *this;
    }

    /**
     * Set the Firestore URL
     * @param url The Firestore URL.
     */
    void url(const String &url)
    {
        this->service_url = url;
    }

    void setApp(uint32_t app_addr, app_token_t *app_token)
    {
        this->app_addr = app_addr;
        this->app_token = app_token;
    }

    app_token_t *appToken()
    {
        GSheetList vec;
        return vec.existed(aVec, app_addr) ? app_token : nullptr;
    }

    /**
     * Perform the async task repeatedly.
     * Should be places in main loop function.
     */
    void loop()
    {
        for (size_t i = 0; i < cVec.size(); i++)
        {
            GSheetAsyncClientClass *aClient = reinterpret_cast<GSheetAsyncClientClass *>(cVec[i]);
            if (aClient)
            {
                aClient->process(true);
                aClient->handleRemove();
            }
        }
    }

    /** Applies one or more updates to the spreadsheet.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param options The GSHEET::BatchUpdateOptions object included requests, includeSpreadsheetInResponse, responseRanges and responseIncludeGridData in its constructor.
     * The GSHEET::BatchUpdateOptions constructor parameters.
     * - requests: A list of updates to apply to the spreadsheet. Requests will be applied in the order they are specified. If any request is not valid, no requests will be applied.
     * - includeSpreadsheetInResponse: Bool option. Determines if the update response should include the spreadsheet resource.
     * - responseRanges: Limits the ranges included in the response spreadsheet. Meaningful only if includeSpreadsheetInResponse is 'true'.
     * - responseIncludeGridData: Bool option. True if grid data should be returned. Meaningful only if includeSpreadsheetInResponse is 'true'. This parameter is ignored if a field mask was set in the request.
     * @return Boolean type status indicates the success of the operation.
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/batchUpdate
     *
     */
    bool batchUpdate(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, GSHEET::BatchUpdateOptions options)
    {
        return true;
    }

protected:
    String service_url;
    String path;
    String uid;
    uint32_t app_addr = 0;
    app_token_t *app_token = nullptr;

    struct async_request_data_t
    {
    public:
        GSheetAsyncClientClass *aClient = nullptr;
        String path;
        String uid;
        gsheet_async_request_handler_t::http_request_method method = gsheet_async_request_handler_t::http_undefined;
        gsheet_slot_options_t opt;
        GSHEET::DataOptions *options = nullptr;
        GSheetAsyncResult *aResult = nullptr;
        GSheetAsyncResultCallback cb = NULL;
        async_request_data_t() {}
        async_request_data_t(GSheetAsyncClientClass *aClient, const String &path, gsheet_async_request_handler_t::http_request_method method, gsheet_slot_options_t opt, GSHEET::DataOptions *options, GSheetAsyncResult *aResult, GSheetAsyncResultCallback cb, const String &uid = "")
        {
            this->aClient = aClient;
            this->path = path;
            this->method = method;
            this->opt = opt;
            this->options = options;
            this->aResult = aResult;
            this->cb = cb;
            this->uid = uid;
        }
    };
};

#endif