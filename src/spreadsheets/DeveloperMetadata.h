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

#ifndef GSHEET_DEVELOPER_METADATA_H
#define GSHEET_DEVELOPER_METADATA_H

#include <Arduino.h>
#include "./spreadsheets/GSheetBase.h"

class DeveloperMetadata : public GSheetBase
{
private:
public:
    /** Get the developer metadata with the specified ID.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param metadataId The ID of the developer metadata to retrieve.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.developerMetadata/get
     */
    void get(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, const String &metadataId, GSheetAsyncResult &aResult)
    {
       
    }

    /** Get all developer metadata matching the specified DataFilter.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param filter The GSHEET::DataFilter object yhat describing the criteria used to determine which DeveloperMetadata entries to return.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.developerMetadata/search
     */
    void search(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, const GSHEET::DataFilter &filter, GSheetAsyncResult &aResult)
    {
    }
};

#endif