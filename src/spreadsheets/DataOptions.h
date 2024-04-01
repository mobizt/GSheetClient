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
    class BatchUpdateOptions : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BatchUpdateOptions &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BatchUpdateOptions() {}
        // A list of updates to apply to the spreadsheet. Requests will be applied in the order they are specified. If any request is not valid, no requests will be applied.
        template <typename T>
        BatchUpdateOptions &addRequest(const Request<T> &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("values"), value.c_str(), false);
            return *this;
        }
        // Determines if the update response should include the spreadsheet resource.
        BatchUpdateOptions &includeSpreadsheetInResponse(bool value) { return setObject(buf[2], "includeSpreadsheetInResponse", owriter.getBoolStr(value), false, true); }
        // Limits the ranges included in the response spreadsheet. Meaningful only if includeSpreadsheetInResponse is 'true'.
        BatchUpdateOptions &addResponseRanges(const String &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("responseRanges"), value, true);
            return *this;
        }
        // True if grid data should be returned. Meaningful only if includeSpreadsheetInResponse is 'true'. This parameter is ignored if a field mask was set in the request.
        BatchUpdateOptions &responseIncludeGridData(bool value) { return setObject(buf[4], "responseIncludeGridData", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
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