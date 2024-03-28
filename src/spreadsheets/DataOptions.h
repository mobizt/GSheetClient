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
#include "./spreadsheets/requests/Common.h"
#include "./spreadsheets/requests/Dimension.h"
#include "./spreadsheets/requests/NamedRange.h"
#include "./spreadsheets/requests/Cell.h"

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

    /**
     * A pair mapping a spreadsheet theme color type to the concrete color it represents.
     */
    class ThemeColorPair : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ThemeColorPair &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ThemeColorPair() {}
        // The type of the spreadsheet theme color.
        ThemeColorPair &colorType(ThemeColorType value)
        {
            if (value == THEME_COLOR_TYPE_UNSPECIFIED)
                return setObject(buf[1], "colorType", "THEME_COLOR_TYPE_UNSPECIFIED", true, true);
            else if (value == TEXT)
                return setObject(buf[1], "colorType", "TEXT", true, true);
            else if (value == BACKGROUND)
                return setObject(buf[1], "colorType", "BACKGROUND", true, true);
            else if (value == ACCENT1)
                return setObject(buf[1], "colorType", "ACCENT1", true, true);
            else if (value == ACCENT2)
                return setObject(buf[1], "colorType", "ACCENT2", true, true);
            else if (value == ACCENT3)
                return setObject(buf[1], "colorType", "ACCENT3", true, true);
            else if (value == ACCENT4)
                return setObject(buf[1], "colorType", "ACCENT4", true, true);
            else if (value == ACCENT5)
                return setObject(buf[1], "colorType", "ACCENT5", true, true);
            else if (value == ACCENT6)
                return setObject(buf[1], "colorType", "ACCENT6", true, true);
            else if (value == LINK)
                return setObject(buf[1], "colorType", "LINK", true, true);
            return *this;
        }
        // The concrete color corresponding to the theme color type.
        ThemeColorPair &color(const ColorStyle value) { return setObject(buf[2], "color", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Settings to control how circular dependencies are resolved with iterative calculation.
     */
    class IterativeCalculationSettings : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        IterativeCalculationSettings &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        IterativeCalculationSettings() {}
        // When iterative calculation is enabled, the maximum number of calculation rounds to perform.
        IterativeCalculationSettings &maxIterations(int value) { return setObject(buf[1], "maxIterations", String(value), false, true); }
        // When iterative calculation is enabled and successive results differ by less than this threshold value, the calculation rounds stop.
        IterativeCalculationSettings &convergenceThreshold(double value) { return setObject(buf[2], "convergenceThreshold", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Represents spreadsheet theme
     */
    class SpreadsheetTheme : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SpreadsheetTheme &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SpreadsheetTheme() {}
        // Name of the primary font family.
        SpreadsheetTheme &primaryFontFamily(const String &value) { return setObject(buf[1], "primaryFontFamily", value, true, true); }
        /** The spreadsheet theme color pairs. To update you must provide all theme color pairs.
         * The item or value will be added to the array or list.
         * To add more items, call this function again.
         */
        SpreadsheetTheme &themeColors(ThemeColorPair value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("themeColors"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Properties of a spreadsheet.
     */
    class SpreadsheetProperties : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SpreadsheetProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SpreadsheetProperties() {}
        // The title of the spreadsheet.
        SpreadsheetProperties &title(const String &value) { return setObject(buf[1], "title", value, true, true); }
        // The locale of the spreadsheet in one of the following formats:
        // an ISO 639-1 language code such as en
        // an ISO 639-2 language code such as fil, if no 639-1 code exists
        // a combination of the ISO language code and country code, such as en_US
        // Note: when updating this field, not all locales/languages are supported.
        SpreadsheetProperties &local(const String &value) { return setObject(buf[2], "local", value, true, true); }
        // The amount of time to wait before volatile functions are recalculated.
        SpreadsheetProperties &autoRecalc(RecalculationInterval value)
        {
            if (value == RECALCULATION_INTERVAL_UNSPECIFIED)
                return setObject(buf[3], "autoRecalc", "RECALCULATION_INTERVAL_UNSPECIFIED", true, true);
            else if (value == ON_CHANGE)
                return setObject(buf[3], "autoRecalc", "ON_CHANGE", true, true);
            else if (value == MINUTE)
                return setObject(buf[3], "autoRecalc", "MINUTE", true, true);
            else if (value == HOUR)
                return setObject(buf[3], "autoRecalc", "HOUR", true, true);
            return *this;
        }
        // The time zone of the spreadsheet, in CLDR format such as America/New_York. If the time zone isn't recognized, this may be a custom time zone such as GMT-07:00.
        SpreadsheetProperties &timeZone(const String &value) { return setObject(buf[4], "timeZone", value, true, true); }
        // The default format of all cells in the spreadsheet. CellData.effectiveFormat will not be set if the cell's format is equal to this default format. This field is read-only.
        SpreadsheetProperties &defaultFormat(const CellFormat &value) { return setObject(buf[5], "defaultFormat", value.c_str(), false, true); }
        // Determines whether and how circular references are resolved with iterative calculation. Absence of this field means that circular references result in calculation errors.
        SpreadsheetProperties &iterativeCalculationSettings(const IterativeCalculationSettings &value) { return setObject(buf[6], "iterativeCalculationSettings", value.c_str(), false, true); }
        // Theme applied to the spreadsheet.
        SpreadsheetProperties &spreadsheetTheme(const SpreadsheetTheme &value) { return setObject(buf[7], "spreadsheetTheme", value.c_str(), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates properties of a spreadsheet.
     */
    class UpdateSpreadsheetPropertiesRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateSpreadsheetPropertiesRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateSpreadsheetPropertiesRequest() {}
        // The properties to update.
        UpdateSpreadsheetPropertiesRequest &properties(const SpreadsheetProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root 'properties' is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSpreadsheetPropertiesRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Properties of a grid.
     */
    class GridProperties : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GridProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GridProperties() {}
        // The number of rows in the grid.
        GridProperties &rowCount(int value) { return setObject(buf[1], "rowCount", String(value), false, true); }
        // The number of columns in the grid.
        GridProperties &columnCount(int value) { return setObject(buf[2], "columnCount", String(value), false, true); }
        // The number of rows that are frozen in the grid.
        GridProperties &frozenRowCount(int value) { return setObject(buf[3], "frozenRowCount", String(value), false, true); }
        // The number of columns that are frozen in the grid.
        GridProperties &frozenColumnCount(int value) { return setObject(buf[4], "frozenColumnCount", String(value), false, true); }
        // True if the grid isn't showing gridlines in the UI.
        GridProperties &hideGridlines(bool value) { return setObject(buf[5], "hideGridlines", owriter.getBoolStr(value), false, true); }
        // True if the row grouping control toggle is shown after the group.
        GridProperties &rowGroupControlAfter(bool value) { return setObject(buf[6], "rowGroupControlAfter", owriter.getBoolStr(value), false, true); }
        // True if the column grouping control toggle is shown after the group.
        GridProperties &columnGroupControlAfter(bool value) { return setObject(buf[7], "columnGroupControlAfter", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Properties of a spreadsheet.
     */
    class SheetProperties : public Printable
    {
    private:
        size_t bufSize = 9;
        String buf[9];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SheetProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SheetProperties() {}
        // The ID of the sheet. Must be non-negative. This field cannot be changed once set.
        SheetProperties &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        // The name of the sheet.
        SheetProperties &title(const String &value) { return setObject(buf[2], "title", value, true, true); }
        // The index of the sheet within the spreadsheet. When adding or updating sheet properties, if this field is excluded then the sheet is added or moved to the end of the sheet list. When updating sheet indices or inserting sheets, movement is considered in "before the move" indexes. For example, if there were three sheets (S1, S2, S3) in order to move S1 ahead of S2 the index would have to be set to 2. A sheet index update request is ignored if the requested index is identical to the sheets current index or if the requested new index is equal to the current sheet index + 1.
        SheetProperties &index(int value) { return setObject(buf[3], "index", String(value), false, true); }
        SheetProperties &sheetType(SheetType value)
        {

            if (value == SHEET_TYPE_UNSPECIFIED)
                return setObject(buf[4], "sheetType", "SHEET_TYPE_UNSPECIFIED", true, true);
            else if (value == GRID)
                return setObject(buf[4], "sheetType", "GRID", true, true);
            else if (value == OBJECT)
                return setObject(buf[4], "sheetType", "OBJECT", true, true);
            else if (value == DATA_SOURCE)
                return setObject(buf[4], "sheetType", "DATA_SOURCE", true, true);
            return *this;
        }
        // Additional properties of the sheet if this sheet is a grid. (If the sheet is an object sheet, containing a chart or image, then this field will be absent.) When writing it is an error to set any grid properties on non-grid sheets.
        // If this sheet is a DATA_SOURCE sheet, this field is output only but contains the properties that reflect how a data source sheet is rendered in the UI, e.g. rowCount.
        SheetProperties &gridProperties(const GridProperties &value) { return setObject(buf[5], "gridProperties", value.c_str(), true, true); }
        // True if the sheet is hidden in the UI, false if it's visible.
        SheetProperties &hidden(bool value) { return setObject(buf[6], "hidden", owriter.getBoolStr(value), false, true); }
        // The color of the tab in the UI.
        SheetProperties &tabColorStyle(const ColorStyle &value) { return setObject(buf[7], "tabColorStyle", value.c_str(), true, true); }
        // True if the sheet is an RTL sheet instead of an LTR sheet.
        SheetProperties &rightToLeft(bool value) { return setObject(buf[8], "rightToLeft", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Adds a new sheet. When a sheet is added at a given index, all subsequent sheets' indexes are incremented. To add an object sheet, use AddChartRequest instead and specify EmbeddedObjectPosition.sheetId or EmbeddedObjectPosition.newSheet.
     */
    class AddSheetRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        AddSheetRequest() {}
        // The properties the new sheet should have. All properties are optional. The sheetId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a sheet that already exists.)
        AddSheetRequest &properties(const SheetProperties &value)
        {
            clear();
            jut.addObject(buf, "properties", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Deletes the requested sheet.
     */
    class DeleteSheetRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DeleteSheetRequest() {}
        // The ID of the sheet to delete.
        // If the sheet is of DATA_SOURCE type, the associated DataSource is also deleted.
        DeleteSheetRequest &properties(int value)
        {
            clear();
            jut.addObject(buf, "sheetId", String(value), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Updates properties of the sheet with the specified sheetId.
     */
    class UpdateSheetPropertiesRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateSheetPropertiesRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateSheetPropertiesRequest() {}
        // The properties to update.
        UpdateSheetPropertiesRequest &properties(const SheetProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSheetPropertiesRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    
    /**
     * Moves data from the source to the destination.
     */
    class CutPasteRequest : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CutPasteRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CutPasteRequest() {}
        // The source data to cut.
        CutPasteRequest &source(const GridRange &value) { return setObject(buf[1], "source", value.c_str(), false, true); }
        // The top-left coordinate where the data should be pasted.
        CutPasteRequest &destination(const GridCoordinate &value) { return setObject(buf[2], "destination", value.c_str(), false, true); }
        // What kind of data to paste. All the source data will be cut, regardless of what is pasted.
        CutPasteRequest &pasteType(PasteType value)
        {
            if (value == PASTE_NORMAL)
                return setObject(buf[3], "pasteType", "PASTE_NORMAL", true, true);
            else if (value == PASTE_VALUES)
                return setObject(buf[3], "pasteType", "PASTE_VALUES", true, true);
            else if (value == PASTE_FORMAT)
                return setObject(buf[3], "pasteType", "PASTE_FORMAT", true, true);
            else if (value == PASTE_NO_BORDERS)
                return setObject(buf[3], "pasteType", "PASTE_NO_BORDERS", true, true);
            else if (value == PASTE_FORMULA)
                return setObject(buf[3], "pasteType", "PASTE_FORMULA", true, true);
            else if (value == PASTE_DATA_VALIDATION)
                return setObject(buf[3], "pasteType", "PASTE_DATA_VALIDATION", true, true);
            else if (value == PASTE_CONDITIONAL_FORMATTING)
                return setObject(buf[3], "pasteType", "PASTE_CONDITIONAL_FORMATTING", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Fills in more data based on existing data.
     */
    class AutoFillRequest : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        AutoFillRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        AutoFillRequest() {}
        // True if we should generate data with the "alternate" series. This differs based on the type and amount of source data.
        AutoFillRequest &properties(bool value) { return setObject(buf[1], "useAlternateSeries", owriter.getBoolStr(value), false, true); }
        // The range to autofill. This will examine the range and detect the location that has data and automatically fill that data in to the rest of the range.
        // Union field area
        AutoFillRequest &range(const GridRange &value) { return buf[3].length() == 0 ? setObject(buf[2], "range", value.c_str(), false, true) : *this; }
        // The source and destination areas to autofill. This explicitly lists the source of the autofill and where to extend that data.
        // Union field area
        AutoFillRequest &sourceAndDestination(const SourceAndDestination &value) { return buf[2].length() == 0 ? setObject(buf[3], "sourceAndDestination", value.c_str(), false, true) : *this; }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    // https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/request#Request
    /**
     * A single kind of update to apply to a spreadsheet.
     */
    template <typename T>
    class Request : public Printable
    {
    private:
        size_t bufSize = 2;
        String buf[2];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Request &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Request(const T &value)
        {
            String name;
            if (std::is_same<T, SpreadsheetProperties>::value)
                name = FPSTR("updateSpreadsheetProperties");
            else if (std::is_same<T, UpdateSheetPropertiesRequest>::value)
                name = FPSTR("updateSheetProperties");
            else if (std::is_same<T, UpdateDimensionPropertiesRequest>::value)
                name = "updateDimensionProperties";
            else if (std::is_same<T, UpdateNamedRangeRequest>::value)
                name = "updateNamedRange";
            else if (std::is_same<T, RepeatCellRequest>::value)
                name = "repeatCell";
            else if (std::is_same<T, AddNamedRangeRequest>::value)
                name = "addNamedRange";
            else if (std::is_same<T, DeleteNamedRangeRequest>::value)
                name = "deleteNamedRange";
            else if (std::is_same<T, AddSheetRequest>::value)
                name = "addSheet";
            else if (std::is_same<T, DeleteSheetRequest>::value)
                name = "deleteSheet";
            else if (std::is_same<T, AutoFillRequest>::value)
                name = "autoFill";
            else if (std::is_same<T, CutPasteRequest>::value)
                name = "cutPaste";

            name = "copyPaste";
            name = "mergeCells";
            name = "unmergeCells";
            name = "updateBorders";
            name = "updateCells";
            name = "addFilterView";
            name = "appendCells";
            name = "clearBasicFilter";
            name = "deleteDimension";
            name = "deleteEmbeddedObject";
            name = "deleteFilterView";
            name = "duplicateFilterView";
            name = "duplicateSheet";
            name = "findReplace";
            name = "insertDimension";
            name = "insertRange";
            name = "moveDimension";
            name = "updateEmbeddedObjectPosition";
            name = "pasteData";
            name = "textToColumns";
            name = "updateFilterView";
            name = "deleteRange";
            name = "appendDimension";
            name = "addConditionalFormatRule";
            name = "updateConditionalFormatRule";
            name = "deleteConditionalFormatRule";
            name = "sortRange";
            name = "setDataValidation";
            name = "setBasicFilter";
            name = "addProtectedRange";
            name = "updateProtectedRange";
            name = "deleteProtectedRange";
            name = "autoResizeDimensions";
            name = "addChart";
            name = "updateChartSpec";
            name = "updateBanding";
            name = "addBanding";
            name = "deleteBanding";
            name = "createDeveloperMetadata";
            name = "updateDeveloperMetadata";
            name = "deleteDeveloperMetadata";
            name = "randomizeRange";
            name = "addDimensionGroup";
            name = "deleteDimensionGroup";
            name = "updateDimensionGroup";
            name = "trimWhitespace";
            name = "deleteDuplicates";
            name = "updateEmbeddedObjectBorder";
            name = "addSlicer";
            name = "updateSlicerSpec";
            name = "addDataSource";
            name = "updateDataSource";
            name = "deleteDataSource";
            name = "refreshDataSource";

            if (name.length())
                setObject(buf[1], name, value.c_str(), false, true);
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
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