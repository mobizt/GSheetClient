#ifndef REQUESTS_H
#define REQUESTS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Dimension.h"
#include "./spreadsheets/requests/NamedRange.h"
#include "./spreadsheets/requests/Cell.h"
#include "./spreadsheets/requests/GridCoordinate.h"
#include "./spreadsheets/requests/Spreadsheets.h"

namespace GSHEET
{

    // What kind of data should be pasted.
    enum PasteType
    {
        PASTE_NORMAL,                //	Paste values, formulas, formats, and merges.
        PASTE_VALUES,                //	Paste the values ONLY without formats, formulas, or merges.
        PASTE_FORMAT,                //	Paste the format and data validation only.
        PASTE_NO_BORDERS,            //	Like PASTE_NORMAL but without borders.
        PASTE_FORMULA,               //	Paste the formulas only.
        PASTE_DATA_VALIDATION,       //	Paste the data validation only.
        PASTE_CONDITIONAL_FORMATTING //	Paste the conditional formatting rules only.
    };

    // How a paste operation should be performed.
    enum PasteOrientation
    {
        NORMAL,   //	Paste normally.
        TRANSPOSE //	Paste transposed, where all rows become columns and vice versa.
    };

    // The type of merge to create.
    enum MergeType
    {
        MERGE_ALL,     //	Create a single merge from the range
        MERGE_COLUMNS, //	Create a merge for each column in the range
        MERGE_ROWS     //	Create a merge for each row in the range
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
     * Updates properties of dimensions within the specified range.
     */
    class UpdateDimensionPropertiesRequest : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateDimensionPropertiesRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateDimensionPropertiesRequest() {}
        // Properties to update.
        UpdateDimensionPropertiesRequest &properties(const DimensionProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDimensionPropertiesRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }
        // Union field dimension_range
        // The rows or columns to update.
        UpdateDimensionPropertiesRequest &range(const DimensionRange &value) { return setObject(buf[3], "range", value.c_str(), false, true); }
        // Union field dimension_range
        // The columns on a data source sheet to update.
        UpdateDimensionPropertiesRequest &dataSourceSheetRange(const DataSourceSheetDimensionRange &value) { return setObject(buf[3], "dataSourceSheetRange", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates properties of the named range with the specified namedRangeId.
     */
    class UpdateNamedRangeRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateNamedRangeRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateNamedRangeRequest() {}
        // The named range to update with the new properties.
        UpdateNamedRangeRequest &namedRange(const String &value) { return setObject(buf[1], "namedRange", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root namedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateNamedRangeRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates all cells in the range to the values in the given Cell object. Only the fields listed in the fields field are updated; others are unchanged.
     * If writing a cell with a formula, the formula's ranges will automatically increment for each field in the range. For example, if writing a cell with formula =A1 into range B2:C4, B2 would be =A1, B3 would be =A2, B4 would be =A3, C2 would be =B1, C3 would be =B2, C4 would be =B3.
     * keep the formula's ranges static, use the $ indicator. For example, use the formula =$A$1 to prevent both the row and the column from incrementing.
     */
    class RepeatCellRequest : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        RepeatCellRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        RepeatCellRequest() {}
        // The range to repeat the cell in.
        RepeatCellRequest &range(const GridRange &value) { return setObject(buf[1], "range", value.c_str(), false, true); }
        // The data to write.
        RepeatCellRequest &cell(const CellData &value) { return setObject(buf[2], "cell", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root cell is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        RepeatCellRequest &fields(const String &value) { return setObject(buf[3], "fields", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Adds a named range to the spreadsheet.
     */
    class AddNamedRangeRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        AddNamedRangeRequest() {}
        // The named range to add. The namedRangeId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a range that already exists.)
        AddNamedRangeRequest &name(const NamedRange &value)
        {
            clear();
            jut.addObject(buf, "namedRange", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };
    /**
     * Removes the named range with the given ID from the spreadsheet.
     */
    class DeleteNamedRangeRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DeleteNamedRangeRequest() {}
        // The ID of the named range to delete.
        DeleteNamedRangeRequest &namedRangeId(const String &value)
        {
            clear();
            jut.addObject(buf, "namedRangeId", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
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

    class CopyPasteRequest : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CopyPasteRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CopyPasteRequest() {}
        // The source range to copy.
        CopyPasteRequest &source(const GridRange &value) { return setObject(buf[1], "source", value.c_str(), false, true); }
        // The location to paste to. If the range covers a span that's a multiple of the source's height or width, then the data will be repeated to fill in the destination range. If the range is smaller than the source range, the entire source data will still be copied (beyond the end of the destination range).
        CopyPasteRequest &destination(const GridRange &value) { return setObject(buf[2], "destination", value.c_str(), false, true); }
        // What kind of data to paste.
        CopyPasteRequest &pasteType(PasteType value)
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
        // How that data should be oriented when pasting.
        CopyPasteRequest &pasteOrientation(PasteOrientation value)
        {
            if (value == NORMAL)
                return setObject(buf[4], "pasteOrientation", "NORMAL", true, true);
            else if (value == TRANSPOSE)
                return setObject(buf[4], "pasteOrientation", "TRANSPOSE", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Merges all cells in the range.
     */
    class MergeCellsRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        MergeCellsRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        MergeCellsRequest() {}
        // The range of cells to merge.
        MergeCellsRequest &range(const GridRange &value) { return setObject(buf[1], "range", value.c_str(), false, true); }
        // How the cells should be merged.
        MergeCellsRequest &mergeType(MergeType value)
        {
            if (value == MERGE_ALL)
                return setObject(buf[2], "mergeType", "MERGE_ALL", true, true);
            else if (value == MERGE_COLUMNS)
                return setObject(buf[2], "mergeType", "MERGE_COLUMNS", true, true);
            else if (value == MERGE_ROWS)
                return setObject(buf[2], "mergeType", "MERGE_ROWS", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Unmerges cells in the given range.
     */
    class UnmergeCellsRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        UnmergeCellsRequest() {}
        // The range within which all cells should be unmerged. If the range spans multiple merges, all will be unmerged. The range must not partially span any merge.
        UnmergeCellsRequest &range(const GridRange &value)
        {
            clear();
            jut.addObject(buf, "range", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Fills in more data based on existing data.
     */
    class AutoFillRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
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
        // Union field area
        // The range to autofill. This will examine the range and detect the location that has data and automatically fill that data in to the rest of the range.
        AutoFillRequest &range(const GridRange &value) { return setObject(buf[2], "range", value.c_str(), false, true); }
        // Union field area
        // The source and destination areas to autofill. This explicitly lists the source of the autofill and where to extend that data.
        AutoFillRequest &sourceAndDestination(const SourceAndDestination &value) { return setObject(buf[2], "sourceAndDestination", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

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
            else if (std::is_same<T, CopyPasteRequest>::value)
                name = "copyPaste";
            else if (std::is_same<T, MergeCellsRequest>::value)
                name = "mergeCells";
            else if (std::is_same<T, UnmergeCellsRequest>::value)
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

}

#endif