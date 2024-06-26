/**
 * Created March 24, 2024
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
#ifndef GSHEET_DATAOPTIONS_H
#define GSHEET_DATAOPTIONS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Requests.h"

#define GSHEET_RESOURCE_PATH_BASE FPSTR("<resource_path>")

enum gsheet_request_type
{
    gsheet_request_type_undefined,
    gsheet_request_type_rollback = 200,
    gsheet_request_type_begin_transaction,
    gsheet_request_type_commit_document,
    gsheet_request_type_batch_write_doc,
    gsheet_request_type_run_query,
    gsheet_request_type_list_collection,
    gsheet_request_type_export_docs,
    gsheet_request_type_import_docs,
    gsheet_request_type_create_doc,
    gsheet_request_type_batch_get_doc,
    gsheet_request_type_create_composite_index,
    gsheet_request_type_create_field_index,
    gsheet_request_type_manage_database,

    gsheet_request_type_get_doc = 300,
    gsheet_request_type_list_doc,
    gsheet_request_type_list_index,
    gsheet_request_type_get_index,

    gsheet_request_type_patch_doc = 400,

    gsheet_request_type_delete_doc = 500,
    gsheet_request_type_delete_index
};

namespace GSHEET
{

    // https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/request#Request

    /**
     * Batch update options
     */
    class BatchUpdateOptions : public BaseG6
    {
    public:
        BatchUpdateOptions() {}

        // This value represents the item to add to an array.
        // A list of updates to apply to the spreadsheet. Requests will be applied in the order they are specified. If any request is not valid, no requests will be applied.
        template <typename T>
        BatchUpdateOptions &requests(const Request<T> &value) { return wr.append<BatchUpdateOptions &, Request<T>>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Determines if the update response should include the spreadsheet resource.
        BatchUpdateOptions &includeSpreadsheetInResponse(bool value) { return wr.set<BatchUpdateOptions &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // Limits the ranges included in the response spreadsheet. Meaningful only if includeSpreadsheetInResponse is 'true'.
        BatchUpdateOptions &responseRanges(const String &value) { return wr.append<BatchUpdateOptions &, String>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if grid data should be returned. Meaningful only if includeSpreadsheetInResponse is 'true'. This parameter is ignored if a field mask was set in the request.
        BatchUpdateOptions &responseIncludeGridData(bool value) { return wr.set<BatchUpdateOptions &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    class GetOptions : public BaseG2
    {
    private:
        String qr[2];

    public:
        GetOptions() {}

        // The ranges to retrieve from the spreadsheet.
        // Ranges separated with comma ",".
        GetOptions &ranges(const String &value)
        {
            qr[0] = FPSTR(__func__);
            qr[0] += "=";
            qr[0] = value;
            return *this;
        }

        // This value represents the item to add to an array.
        // The DataFilters used to select which ranges to retrieve from the spreadsheet.
        GetOptions &dataFilters(const DataFilter &value) { return wr.append<GetOptions &, DataFilter>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // True if grid data should be returned. This parameter is ignored if a field mask was set in the request.
        GetOptions &includeGridData(bool value)
        {
            if (qr[1].indexOf("fields") == -1)
            {
                qr[1] = FPSTR(__func__);
                qr[1] += "=";
                qr[1] += value ? "true" : "false";
            }
            return *this;
        }

        // The desired fields.
        GetOptions &fields(const String &value)
        {
            qr[1] = FPSTR(__func__);
            qr[1] += "=";
            qr[1] += value;
            return *this;
        }

        String getQueryString()
        {
            String str;
            if (qr[0].length())
                str = qr[0];
            if (qr[1].length())
            {
                if (str.length())
                    str += "&";
                str += qr[1];
            }
            return str;
        }
    };

    class ListOptions
    {
    private:
        String qr[3];

    public:
        ListOptions() {}

        ListOptions &pageSize(int value)
        {
            qr[0] = FPSTR(__func__);
            qr[0] += "=";
            qr[0] += value;
            return *this;
        }

        ListOptions &orderBy(const String &value)
        {
            qr[1] = FPSTR(__func__);
            qr[1] += "=";
            qr[1] += value;
            return *this;
        }

        ListOptions &pageToken(const String &value)
        {
            qr[2] = FPSTR(__func__);
            qr[2] += "=";
            qr[2] += value;
            return *this;
        }

        String getQueryString()
        {
            String str;
            for (size_t i = 0; i < 3; i++)
            {
                if (qr[i].length())
                {
                    if (str.length())
                        str += "&";
                    str += qr[i];
                }
            }
            return str;
        }
    };

    class BatchGetOptions
    {
    private:
        static const size_t bufSize = 4;
        String qr[bufSize];

    public:
        BatchGetOptions() {}

        // The A1 notation or R1C1 notation of the range to retrieve values from.
        BatchGetOptions &ranges(const String &value)
        {
            qr[0] = FPSTR(__func__);
            qr[0] += "=";
            qr[0] += value;
            return *this;
        }

        // The major dimension that results should use.
        // For example, if the spreadsheet data is: A1=1,B1=2,A2=3,B2=4, then requesting ranges=["A1:B2"],majorDimension=ROWS returns [[1,2],[3,4]], whereas requesting ranges=["A1:B2"],majorDimension=COLUMNS returns [[1,3],[2,4]].
        BatchGetOptions &majorDimension(Dimensions::Dimension value)
        {
            qr[1] = FPSTR(__func__);
            qr[1] += "=";
            qr[1] += _Dimension[value].text;
            return *this;
        }

        // How values should be represented in the output. The default render option is ValueRenderOption.FORMATTED_VALUE.
        BatchGetOptions &valueRenderOption(GSHEET::ValueRenderOption value)
        {
            qr[2] = FPSTR(__func__);
            qr[2] += "=";
            qr[2] += _ValueRenderOption[value].text;
            ;
            return *this;
        }

        // How dates, times, and durations should be represented in the output. This is ignored if valueRenderOption is FORMATTED_VALUE. The default dateTime render option is SERIAL_NUMBER.
        BatchGetOptions &dateTimeRenderOption(GSHEET::DateTimeRenderOption value)
        {
            qr[3] = FPSTR(__func__);
            qr[3] += "=";
            qr[3] += _DateTimeRenderOption[value].text;
            ;
            return *this;
        }

        String getQueryString()
        {
            String str;
            for (size_t i = 0; i < bufSize; i++)
            {
                if (qr[i].length())
                {
                    if (str.length())
                        str += "&";
                    str += qr[i];
                }
            }
            return str;
        }
    };

    class BatchGetByDataFilterOpions : public BaseG6
    {

    public:
        BatchGetByDataFilterOpions() {}

        // This value represents the item to add to an array.
        // The data filters used to match the ranges of values to retrieve. Ranges that match any of the specified data filters are included in the response.
        BatchGetByDataFilterOpions &dataFilters(const DataFilter &value) { return wr.append<BatchGetByDataFilterOpions &, DataFilter>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The major dimension that results should use.
        // For example, if the spreadsheet data is: A1=1,B1=2,A2=3,B2=4, then a request that selects that range and sets majorDimension=ROWS returns [[1,2],[3,4]], whereas a request that sets majorDimension=COLUMNS returns [[1,3],[2,4]].
        BatchGetByDataFilterOpions &majorDimension(Dimensions::Dimension value) { return wr.set<BatchGetByDataFilterOpions &, const char *>(*this, _Dimension[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // How values should be represented in the output. The default render option is FORMATTED_VALUE.
        BatchGetByDataFilterOpions &valueRenderOption(ValueRenderOption value) { return wr.set<BatchGetByDataFilterOpions &, const char *>(*this, _ValueRenderOption[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // How dates, times, and durations should be represented in the output. This is ignored if valueRenderOption is FORMATTED_VALUE. The default dateTime render option is SERIAL_NUMBER.
        BatchGetByDataFilterOpions &dateTimeRenderOption(DateTimeRenderOption value) { return wr.set<BatchGetByDataFilterOpions &, const char *>(*this, _DateTimeRenderOption[value].text, buf, bufSize, 4, FPSTR(__func__)); }
    };

    class Parent
    {
        friend class GSheetBase;

    private:
        String spreadsheetId;
        String sheetId;

    public:
        Parent() {}
        Parent(const String &spreadsheetId, const String &sheetId = "")
        {
            this->spreadsheetId = spreadsheetId;
            this->sheetId = sheetId;
        }
        void setSpreadsheetId(const String &spreadsheetId) { this->spreadsheetId = spreadsheetId; }
        void setSheetId(const String &) { this->sheetId = sheetId; }
        String getSpreadsheetId() const { return spreadsheetId.c_str(); }
        String getSheetId() const { return sheetId.c_str(); }
    };

    class DataOptions
    {
        friend class GSheetBase;

    public:
        Parent parent;
        String spreadsheetId;
        String sheetId;
        String extras;
        String payload;
        gsheet_request_type requestType = gsheet_request_type_undefined;
        unsigned long requestTime = 0;

        void copy(DataOptions &rhs)
        {
            this->parent = rhs.parent;
            this->spreadsheetId = rhs.spreadsheetId;
            this->sheetId = rhs.sheetId;
            this->extras = rhs.extras;
            this->payload = rhs.payload;
        }

    private:
    };
}

#endif