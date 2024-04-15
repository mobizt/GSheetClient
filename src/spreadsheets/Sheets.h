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

#ifndef GSHEET_SHEETS_H
#define GSHEET_SHEETS_H

#include <Arduino.h>
#include "./spreadsheets/GSheetBase.h"

class Sheets : public GSheetBase
{

public:

    /** Get the developer metadata with the specified ID.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id and sheet Id in its constructor.
     * @param destinationSpreadsheetId The ID of the spreadsheet to copy the sheet to.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * @note The sheet id is the integer number which you can get it from gid parameter of spreadsheet URL when select the sheet tab.
     * For example, https://docs.google.com/spreadsheets/d/xxxxxxx/edit#gid=1180731163
     * The sheet id of above case is 1180731163
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.sheets/copyTo
     */
    void copyTo(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, const String &destinationSpreadsheetId, GSheetAsyncResult &aResult)
    {
    }

};

#endif