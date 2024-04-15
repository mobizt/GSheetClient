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

#ifndef GSHEET_VALUES_H
#define GSHEET_VALUES_H

#include <Arduino.h>
#include "./spreadsheets/GSheetBase.h"

class Values : public GSheetBase
{

public:
    /** Get a range of values from a spreadsheet.
     *
     * @param response (FirebaseJson or String) The returned response.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param range The A1 notation or R1C1 notation of the range to retrieve values from.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/get
     */
    void get(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, const String &range, GSheetAsyncResult &aResult)
    {
    }

    /** Get one or more ranges of values from a spreadsheet.
     *
     * @param aClient The async client.
     * @param parent The GSHEET::Parent object included spreadsheet Id in its constructor.
     * @param options The GSHEET::BatchGetOptions object included ranges, majorDimension, valueRenderOption and dateTimeRenderOption in its constructor.
     * The GSHEET::BatchUpdateOptions constructor parameters.
     * - ranges: The A1 notation or R1C1 notation of the range to retrieve values from.
     * - majorDimension: The major dimension that results should use. For example, if the spreadsheet data is: A1=1,B1=2,A2=3,B2=4, then requesting ranges=["A1:B2"],majorDimension=ROWS returns [[1,2],[3,4]], whereas requesting ranges=["A1:B2"],majorDimension=COLUMNS returns [[1,3],[2,4]].
     * - valueRenderOption: How values should be represented in the output. The default render option is ValueRenderOption.FORMATTED_VALUE.
     * - dateTimeRenderOption: How dates, times, and durations should be represented in the output. This is ignored if valueRenderOption is FORMATTED_VALUE. The default dateTime render option is SERIAL_NUMBER.
     * @param aResult The async result (GSheetAsyncResult).
     *
     * For ref doc go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/batchGet
     *
     */
    void batchGet(GSheetAsyncClientClass &aClient, const GSHEET::Parent &parent, const GSHEET::BatchGetOptions &options, GSheetAsyncResult &aResult)
    {
    }
};

#endif