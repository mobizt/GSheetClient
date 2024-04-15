/**
 * Created April 15, 2024
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
    std::vector<uint32_t> cVec; // GSheetAsyncClient vector

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

    void setApp(uint32_t app_addr, app_token_t *app_token, uint32_t avec_addr)
    {
        this->app_addr = app_addr;
        this->app_token = app_token;
        this->avec_addr = avec_addr; // GSheetAsyncClient vector (list) address
    }

    app_token_t *appToken()
    {
        if (avec_addr > 0)
        {
            std::vector<uint32_t> *cVec = reinterpret_cast<std::vector<uint32_t> *>(avec_addr);
            GSheetList vec;
            if (cVec)
                return vec.existed(*cVec, app_addr) ? app_token : nullptr;
        }
        return nullptr;
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
     * @param aResult The async result (GSheetAsyncResult).
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/batchUpdate
     *
     */
    void batchUpdate(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, const GSHEET::BatchUpdateOptions &options, GSheetAsyncResult &aResult)
    {
       
    }

    /** Creates a spreadsheet, returning the newly created spreadsheet.
     *
     * @param aClient The async client.
     * @param spreadsheet The GSHEET::Spreadsheet object that represents a spreadsheet.
     * @param sharedUserEmail The Email of user to share the access.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * @note Drive API should be enabled at, https://console.cloud.google.com/apis/library/drive.googleapis.com
     *
     * For ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/create
     *
     */
    void create(GSheetAsyncClientClass &aClient, const GSHEET::Spreadsheet &spreadsheet, const String &sharedUserEmail, GSheetAsyncResult &aResult)
    {
        
    }

    /** Returns the spreadsheet at the given ID. The caller must specify the spreadsheet ID.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param options The GSHEET::GetOptions object included ranges, dataFilters, includeGridData and fields in its constructor.
     * The GSHEET::GetOptions constructor parameters.
     * - ranges: The ranges to retrieve from the spreadsheet.
     * - dataFilters: The DataFilters used to select which ranges to retrieve from the spreadsheet.
     * - includeGridData: True if grid data should be returned. This parameter is ignored if a field mask was set in the request.
     * - fields: The desired fields.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * For ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/get
     * For the dataFilters, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/getByDataFilter
     *
     */
    void get(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, GSHEET::GetOptions options, GSheetAsyncResult &aResult)
    {
    }

    /** Delete spreadsheets from Google Drive.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param aResult The async result (GSheetAsyncResult).
     *
     */
    void deleteFile(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, GSheetAsyncResult &aResult)
    {
    }

    /** List the spreadsheets in Google Drive.
     *
     * @param aClient The async client.
     * @param options The GSHEET::ListOptions object included pageSize, orderBy and pageToken in its constructor.
     * The GSHEET::ListOptions constructor parameters.
     * - pageSize: The maximum number of files to return per page.
     * - orderBy: The comma-separated list of sort keys.
     * The Valid keys are 'createdTime', 'folder', 'modifiedByMeTime', 'modifiedTime', 'name', 'name_natural',
     * 'quotaBytesUsed', 'recency', 'sharedWithMeTime', 'starred', and 'viewedByMeTime'.
     * Each key sorts ascending by default, but may be reversed with the 'desc' modifier.
     * Example usage: ?orderBy=folder,modifiedTime desc,name.
     * - pageToken: The token for continuing a previous list request on the next page.
     * @param aResult The async result (GSheetAsyncResult).
     *
     */
    void listFiles(GSheetAsyncClientClass &aClient, GSHEET::ListOptions options, GSheetAsyncResult &aResult)
    {
    }

protected:
    String service_url;
    // GSheetApp address and GSheetApp vector address
    uint32_t app_addr = 0, avec_addr = 0;
    String path;
    String uid;
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