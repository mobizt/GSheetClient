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
#include "./spreadsheets/requests/Common.h"

namespace GSHEET
{

    /**
     * Updates properties of a spreadsheet.
     */
    class UpdateSpreadsheetPropertiesRequest : public O4
    {
    public:
        UpdateSpreadsheetPropertiesRequest() {}
        // The properties to update.
        UpdateSpreadsheetPropertiesRequest &properties(const SpreadsheetProperties &value) { return wr.set<UpdateSpreadsheetPropertiesRequest &, SpreadsheetProperties>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields that should be updated. At least one field must be specified. The root 'properties' is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSpreadsheetPropertiesRequest &fields(const String &value) { return wr.set<UpdateSpreadsheetPropertiesRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Adds a new sheet. When a sheet is added at a given index, all subsequent sheets' indexes are incremented. To add an object sheet, use AddChartRequest instead and specify EmbeddedObjectPosition.sheetId or EmbeddedObjectPosition.newSheet.
     */
    class AddSheetRequest : public O1
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        AddSheetRequest() {}
        // The properties the new sheet should have. All properties are optional. The sheetId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a sheet that already exists.)
        AddSheetRequest &properties(const SheetProperties &value) { return wr.add<AddSheetRequest &, SheetProperties>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes the requested sheet.
     */
    class DeleteSheetRequest : public O1
    {
    public:
        DeleteSheetRequest() {}
        // The ID of the sheet to delete.
        // If the sheet is of DATA_SOURCE type, the associated DataSource is also deleted.
        DeleteSheetRequest &properties(int value) { return wr.add<DeleteSheetRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates properties of the sheet with the specified sheetId.
     */
    class UpdateSheetPropertiesRequest : public O4
    {

    public:
        UpdateSheetPropertiesRequest() {}
        // The properties to update.
        UpdateSheetPropertiesRequest &properties(const SheetProperties &value) { return wr.set<UpdateSheetPropertiesRequest &, SheetProperties>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSheetPropertiesRequest &fields(const String &value) { return wr.set<UpdateSheetPropertiesRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Updates properties of dimensions within the specified range.
     */
    class UpdateDimensionPropertiesRequest : public O4
    {

    public:
        UpdateDimensionPropertiesRequest() {}
        // Properties to update.
        UpdateDimensionPropertiesRequest &properties(const DimensionProperties &value) { return wr.set<UpdateDimensionPropertiesRequest &, DimensionProperties>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDimensionPropertiesRequest &fields(const String &value) { return wr.set<UpdateDimensionPropertiesRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Union field dimension_range
        // The rows or columns to update.
        UpdateDimensionPropertiesRequest &range(const DimensionRange &value) { return wr.set<UpdateDimensionPropertiesRequest &, DimensionRange>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // Union field dimension_range
        // The columns on a data source sheet to update.
        UpdateDimensionPropertiesRequest &dataSourceSheetRange(const DataSourceSheetDimensionRange &value) { return wr.set<UpdateDimensionPropertiesRequest &, DataSourceSheetDimensionRange>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * Updates properties of the named range with the specified namedRangeId.
     */
    class UpdateNamedRangeRequest : public O4
    {

    public:
        UpdateNamedRangeRequest() {}
        // The named range to update with the new properties.
        UpdateNamedRangeRequest &namedRange(const String &value) { return wr.set<UpdateNamedRangeRequest &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields that should be updated. At least one field must be specified. The root namedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateNamedRangeRequest &fields(const String &value) { return wr.set<UpdateNamedRangeRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Updates all cells in the range to the values in the given Cell object. Only the fields listed in the fields field are updated; others are unchanged.
     * If writing a cell with a formula, the formula's ranges will automatically increment for each field in the range. For example, if writing a cell with formula =A1 into range B2:C4, B2 would be =A1, B3 would be =A2, B4 would be =A3, C2 would be =B1, C3 would be =B2, C4 would be =B3.
     * keep the formula's ranges static, use the $ indicator. For example, use the formula =$A$1 to prevent both the row and the column from incrementing.
     */
    class RepeatCellRequest : public O4
    {

    public:
        RepeatCellRequest() {}
        // The range to repeat the cell in.
        RepeatCellRequest &range(const GridRange &value) { return wr.set<RepeatCellRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The data to write.
        RepeatCellRequest &cell(const CellData &value) { return wr.set<RepeatCellRequest &, CellData>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields that should be updated. At least one field must be specified. The root cell is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        RepeatCellRequest &fields(const String &value) { return wr.set<RepeatCellRequest &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
    };

    /**
     * Adds a named range to the spreadsheet.
     */
    class AddNamedRangeRequest : public O1
    {
    public:
        AddNamedRangeRequest() {}
        // The named range to add. The namedRangeId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a range that already exists.)
        AddNamedRangeRequest &name(const NamedRange &value) { return wr.add<AddNamedRangeRequest &, NamedRange>(*this, value, buf, FPSTR(__func__)); }
    };
    /**
     * Removes the named range with the given ID from the spreadsheet.
     */
    class DeleteNamedRangeRequest : public O1
    {
    public:
        DeleteNamedRangeRequest() {}
        // The ID of the named range to delete.
        DeleteNamedRangeRequest &namedRangeId(const String &value) { return wr.add<DeleteNamedRangeRequest &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Moves data from the source to the destination.
     */
    class CutPasteRequest : public O4
    {

    public:
        CutPasteRequest() {}
        // The source data to cut.
        CutPasteRequest &source(const GridRange &value) { return wr.set<CutPasteRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The top-left coordinate where the data should be pasted.
        CutPasteRequest &destination(const GridCoordinate &value) { return wr.set<CutPasteRequest &, GridCoordinate>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // What kind of data to paste. All the source data will be cut, regardless of what is pasted.
        CutPasteRequest &pasteType(Paste::PasteType value) { return wr.set<CutPasteRequest &, const char *>(*this, _PasteType[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    class CopyPasteRequest : public O4
    {

    public:
        CopyPasteRequest() {}
        // The source range to copy.
        CopyPasteRequest &source(const GridRange &value) { return wr.set<CopyPasteRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The location to paste to. If the range covers a span that's a multiple of the source's height or width, then the data will be repeated to fill in the destination range. If the range is smaller than the source range, the entire source data will still be copied (beyond the end of the destination range).
        CopyPasteRequest &destination(const GridRange &value) { return wr.set<CopyPasteRequest &, GridRange>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // What kind of data to paste.
        CopyPasteRequest &pasteType(Paste::PasteType value) { return wr.set<CopyPasteRequest &, const char *>(*this, _PasteType[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
        // How that data should be oriented when pasting.
        CopyPasteRequest &pasteOrientation(Paste::PasteOrientation value) { return wr.set<CopyPasteRequest &, const char *>(*this, _PasteOrientation[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * Merges all cells in the range.
     */
    class MergeCellsRequest : public O4
    {

    public:
        MergeCellsRequest() {}
        // The range of cells to merge.
        MergeCellsRequest &range(const GridRange &value) { return wr.set<MergeCellsRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // How the cells should be merged.
        MergeCellsRequest &mergeType(Merge::MergeType value) { return wr.set<MergeCellsRequest &, const char *>(*this, _MergeType[value].text, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Unmerges cells in the given range.
     */
    class UnmergeCellsRequest : public O1
    {

    public:
        UnmergeCellsRequest() {}
        // The range within which all cells should be unmerged. If the range spans multiple merges, all will be unmerged. The range must not partially span any merge.
        UnmergeCellsRequest &range(const GridRange &value) { return wr.add<UnmergeCellsRequest &, GridRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates the borders of a range. If a field is not set in the request, that means the border remains as-is. For example, with two subsequent UpdateBordersRequest:
     * range: A1:A5 { top: RED, bottom: WHITE }
     * range: A1:A5 { left: BLUE }
     * That would result in A1:A5 having a borders of { top: RED, bottom: WHITE, left: BLUE }.
     * If you want to clear a border, explicitly set the style to NONE.
     */
    class UpdateBordersRequest : public O8
    {
    public:
        UpdateBordersRequest() {}
        // The range whose borders should be updated.
        UpdateBordersRequest &range(const GridRange &value) { return wr.set<UpdateBordersRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The border to put at the top of the range.
        UpdateBordersRequest &top(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The border to put at the bottom of the range.
        UpdateBordersRequest &bottom(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // The border to put at the left of the range.
        UpdateBordersRequest &left(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // The border to put at the right of the range.
        UpdateBordersRequest &right(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, buf[5], FPSTR(__func__)); }
        // The horizontal border to put within the range.
        UpdateBordersRequest &innerHorizontal(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, buf[6], FPSTR(__func__)); }
        // The vertical border to put within the range.
        UpdateBordersRequest &innerVertical(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, buf[7], FPSTR(__func__)); }
    };
    /**
     * Updates all cells in a range with new data.
     */
    class UpdateCellsRequest : public O4
    {

    public:
        UpdateCellsRequest() {}
        // Add to a list
        // The data to write.
        UpdateCellsRequest &rows(const RowData &value) { return wr.append<UpdateCellsRequest &, RowData>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields of CellData that should be updated. At least one field must be specified. The root is the CellData; 'row.values.' should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateCellsRequest &fields(const String &value) { return wr.set<UpdateCellsRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Union field area
        // The coordinate to start writing data at. Any number of rows and columns (including a different number of columns per row) may be written.
        UpdateCellsRequest &start(const GridCoordinate &value) { return wr.set<UpdateCellsRequest &, GridCoordinate>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // Union field area
        // The range to write data to.
        // If the data in rows does not cover the entire requested range, the fields matching those set in fields will be cleared.
        UpdateCellsRequest &range(const GridRange &value) { return wr.set<UpdateCellsRequest &, GridRange>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };
    /**
     * Adds a filter view.
     */
    class AddFilterViewRequest : public O1
    {
    public:
        AddFilterViewRequest() {}
        // The filter to add. The filterViewId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a filter that already exists.)
        AddFilterViewRequest &filter(const FilterView &value) { return wr.add<AddFilterViewRequest &, FilterView>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Adds new cells after the last row with data in a sheet, inserting new rows into the sheet if necessary.
     */
    class AppendCellsRequest : public O4
    {

    public:
        AppendCellsRequest() {}
        // The sheet ID to append the data to.
        AppendCellsRequest &sheetId(int value) { return wr.set<AppendCellsRequest &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Add to list
        // The data to append.
        AppendCellsRequest &rows(const RowData &value) { return wr.append<AppendCellsRequest &, RowData>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The fields of CellData that should be updated. At least one field must be specified. The root is the CellData; 'row.values.' should not be specified. A single "*" can be used as short-hand for listing every field.
        AppendCellsRequest &fields(const String &value) { return wr.set<AppendCellsRequest &, String>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * Clears the basic filter, if any exists on the sheet.
     */
    class ClearBasicFilterRequest : public O1
    {
    public:
        ClearBasicFilterRequest() {}
        // The sheet ID on which the basic filter should be cleared.
        ClearBasicFilterRequest &sheetId(int value) { return wr.add<ClearBasicFilterRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes the dimensions from the sheet.
     */
    class DeleteDimensionRequest : public O1
    {
    public:
        DeleteDimensionRequest() {}
        // The dimensions to delete from the sheet.
        DeleteDimensionRequest &range(const DimensionRange &value) { return wr.add<DeleteDimensionRequest &, DimensionRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes the embedded object with the given ID.
     */
    class DeleteEmbeddedObjectRequest : public O1
    {
    public:
        DeleteEmbeddedObjectRequest() {}
        // The ID of the embedded object to delete.
        DeleteEmbeddedObjectRequest &objectId(int value) { return wr.add<DeleteEmbeddedObjectRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes a particular filter view.
     */
    class DeleteFilterViewRequest : public O1
    {
    public:
        DeleteFilterViewRequest() {}
        // The ID of the filter to delete.
        DeleteFilterViewRequest &filterId(int value) { return wr.add<DeleteFilterViewRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Duplicates a particular filter view.
     */
    class DuplicateFilterViewRequest : public O1
    {
    public:
        DuplicateFilterViewRequest() {}
        // The ID of the filter being duplicated.
        DuplicateFilterViewRequest &filterId(int value) { return wr.add<DuplicateFilterViewRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Duplicates the contents of a sheet.
     */
    class DuplicateSheetRequest : public O6
    {

    public:
        DuplicateSheetRequest() {}
        // The sheet to duplicate.
        // If the source sheet is of DATA_SOURCE type, its backing DataSource is also duplicated and associated with the new copy of the sheet. No data execution is triggered, the grid data of this sheet is also copied over but only available after the batch request completes.
        DuplicateSheetRequest &sourceSheetId(int value) { return wr.set<DuplicateSheetRequest &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The zero-based index where the new sheet should be inserted. The index of all sheets after this are incremented.
        DuplicateSheetRequest &insertSheetIndex(int value) { return wr.set<DuplicateSheetRequest &, int>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // If set, the ID of the new sheet. If not set, an ID is chosen. If set, the ID must not conflict with any existing sheet ID. If set, it must be non-negative.
        DuplicateSheetRequest &newSheetId(int value) { return wr.set<DuplicateSheetRequest &, int>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // The name of the new sheet. If empty, a new name is chosen for you.
        DuplicateSheetRequest &newSheetName(const String &value) { return wr.set<DuplicateSheetRequest &, String>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
    };

    /**
     * Finds and replaces data in cells over a range, sheet, or all sheets.
     */
    class FindReplaceRequest : public O8
    {

    public:
        FindReplaceRequest() {}
        // The value to search.
        FindReplaceRequest &find(const String &value) { return wr.set<FindReplaceRequest &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The value to use as the replacement.
        FindReplaceRequest &replacement(const String &value) { return wr.set<FindReplaceRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // True if the search is case sensitive.
        FindReplaceRequest &matchCase(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // True if the find value should match the entire cell.
        FindReplaceRequest &matchEntireCell(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // True if the find value is a regex. The regular expression and replacement should follow Java regex rules at https://docs.oracle.com/javase/8/docs/api/java/util/regex/Pattern.html. The replacement string is allowed to refer to capturing groups. For example, if one cell has the contents "Google Sheets" and another has "Google Docs", then searching for "o.* (.*)" with a replacement of "$1 Rocks" would change the contents of the cells to "GSheets Rocks" and "GDocs Rocks" respectively.
        FindReplaceRequest &searchByRegex(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, buf[5], FPSTR(__func__)); }
        // True if the search should include cells with formulas. False to skip cells with formulas.
        FindReplaceRequest &includeFormulas(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, buf[6], FPSTR(__func__)); }
        // Union field scope
        // The range to find/replace over.
        FindReplaceRequest &range(const GridRange &value) { return wr.set<FindReplaceRequest &, GridRange>(*this, value, buf, bufSize, buf[7], FPSTR(__func__)); }
        // Union field scope
        // The sheet to find/replace over.
        FindReplaceRequest &sheetId(int value) { return wr.set<FindReplaceRequest &, int>(*this, value, buf, bufSize, buf[7], FPSTR(__func__)); }
        // Union field scope
        // True to find/replace over all sheets.
        FindReplaceRequest &allSheets(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, buf[7], FPSTR(__func__)); }
    };

    /**
     * Inserts rows or columns in a sheet at a particular index.
     */
    class InsertDimensionRequest : public O4
    {

    public:
        InsertDimensionRequest() {}
        // The dimensions to insert. Both the start and end indexes must be bounded.
        InsertDimensionRequest &range(const DimensionRange &value) { return wr.set<InsertDimensionRequest &, DimensionRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Whether dimension properties should be extended from the dimensions before or after the newly inserted dimensions. True to inherit from the dimensions before (in which case the start index must be greater than 0), and false to inherit from the dimensions after.
        // For example, if row index 0 has red background and row index 1 has a green background, then inserting 2 rows at index 1 can inherit either the green or red background. If inheritFromBefore is true, the two new rows will be red (because the row before the insertion point was red), whereas if inheritFromBefore is false, the two new rows will be green (because the row after the insertion point was green).
        InsertDimensionRequest &inheritFromBefore(bool value) { return wr.set<InsertDimensionRequest &, bool>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Inserts cells into a range, shifting the existing cells over or down.
     */
    class InsertRangeRequest : public O4
    {

    public:
        InsertRangeRequest() {}
        // The range to insert new cells into.
        InsertRangeRequest &range(const GridRange &value) { return wr.set<InsertRangeRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The dimension which will be shifted when inserting cells. If ROWS, existing cells will be shifted down. If COLUMNS, existing cells will be shifted right.
        InsertRangeRequest &shiftDimension(Dimensions::Dimension value) { return wr.set<InsertRangeRequest &, const char *>(*this, _Dimension[value].text, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Moves one or more rows or columns.
     */
    class MoveDimensionRequest : public O4
    {
    public:
        MoveDimensionRequest() {}
        // The source dimensions to move.
        MoveDimensionRequest &source(const DimensionRange &value) { return wr.set<MoveDimensionRequest &, DimensionRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The zero-based start index of where to move the source data to, based on the coordinates before the source data is removed from the grid. Existing data will be shifted down or right (depending on the dimension) to make room for the moved dimensions. The source dimensions are removed from the grid, so the the data may end up in a different index than specified.
        // For example, given A1..A5 of 0, 1, 2, 3, 4 and wanting to move "1" and "2" to between "3" and "4", the source would be ROWS [1..3),and the destination index would be "4" (the zero-based index of row 5). The end result would be A1..A5 of 0, 3, 1, 2, 4.
        MoveDimensionRequest &destinationIndex(int value) { return wr.set<MoveDimensionRequest &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
    };

    /**
     * Update an embedded object's position (such as a moving or resizing a chart or image).
     */
    class UpdateEmbeddedObjectPositionRequest : public O4
    {
    public:
        UpdateEmbeddedObjectPositionRequest() {}
        // The ID of the object to moved.
        UpdateEmbeddedObjectPositionRequest &objectId(int value) { return wr.set<UpdateEmbeddedObjectPositionRequest &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // An explicit position to move the embedded object to. If newPosition.sheetId is set, a new sheet with that ID will be created. If newPosition.newSheet is set to true, a new sheet will be created with an ID that will be chosen for you.
        UpdateEmbeddedObjectPositionRequest &newPosition(const EmbeddedObjectPosition &value) { return wr.set<UpdateEmbeddedObjectPositionRequest &, EmbeddedObjectPosition>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The fields of OverlayPosition that should be updated when setting a new position. Used only if newPosition.overlayPosition is set, in which case at least one field must be specified. The root newPosition.overlayPosition is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateEmbeddedObjectPositionRequest &fields(const String &value) { return wr.set<UpdateEmbeddedObjectPositionRequest &, String>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * Inserts data into the spreadsheet starting at the specified coordinate.
     */
    class PasteDataRequest : public O6
    {

    public:
        PasteDataRequest() {}
        // The coordinate at which the data should start being inserted.
        PasteDataRequest &coordinate(const GridCoordinate &value) { return wr.set<PasteDataRequest &, GridCoordinate>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The data to insert.
        PasteDataRequest &data(const String &value) { return wr.set<PasteDataRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // How the data should be pasted.
        PasteDataRequest &type(Paste::PasteType value){ return wr.set<PasteDataRequest &, const char*>(*this, _PasteType[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
        // Union field kind
        // The delimiter in the data.
        PasteDataRequest &delimiter(const String &value) { return wr.set<PasteDataRequest &, String>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // Union field kind
        // True if the data is HTML.
        PasteDataRequest &html(bool value) { return wr.set<PasteDataRequest &, bool>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
    };

    /**
     * Splits a column of text into multiple columns, based on a delimiter in each cell.
     */
    class TextToColumnsRequest : public O4
    {

    public:
        TextToColumnsRequest() {}
        // The source data range. This must span exactly one column.
        TextToColumnsRequest &source(const GridRange &value) { return wr.set<TextToColumnsRequest &, GridRange>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The delimiter to use. Used only if delimiterType is CUSTOM.
        TextToColumnsRequest &delimiter(const String &value) { return wr.set<TextToColumnsRequest &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The delimiter type to use.
        TextToColumnsRequest &delimiterType(Delim::DelimiterType value) { return wr.set<TextToColumnsRequest &, const char *>(*this, _DelimiterType[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * Fills in more data based on existing data.
     */
    class AutoFillRequest : public O4
    {

    public:
        AutoFillRequest() {}
        // True if we should generate data with the "alternate" series. This differs based on the type and amount of source data.
        AutoFillRequest &useAlternateSeries(bool value) { return wr.set<AutoFillRequest &, bool>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Union field area
        // The range to autofill. This will examine the range and detect the location that has data and automatically fill that data in to the rest of the range.
        AutoFillRequest &range(const GridRange &value) { return wr.set<AutoFillRequest &, GridRange>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Union field area
        // The source and destination areas to autofill. This explicitly lists the source of the autofill and where to extend that data.
        AutoFillRequest &sourceAndDestination(const SourceAndDestination &value) { return wr.set<AutoFillRequest &, SourceAndDestination>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * A single kind of update to apply to a spreadsheet.
     */
    template <typename T>
    class Request : public O1
    {

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
            else if (std::is_same<T, UpdateBordersRequest>::value)
                name = "updateBorders";
            else if (std::is_same<T, UpdateCellsRequest>::value)
                name = "updateCells";
            else if (std::is_same<T, AddFilterViewRequest>::value)
                name = "addFilterView";
            else if (std::is_same<T, AppendCellsRequest>::value)
                name = "appendCells";
            else if (std::is_same<T, ClearBasicFilterRequest>::value)
                name = "clearBasicFilter";
            else if (std::is_same<T, DeleteDimensionRequest>::value)
                name = "deleteDimension";
            else if (std::is_same<T, DeleteEmbeddedObjectRequest>::value)
                name = "deleteEmbeddedObject";
            else if (std::is_same<T, DeleteFilterViewRequest>::value)
                name = "deleteFilterView";
            else if (std::is_same<T, DuplicateFilterViewRequest>::value)
                name = "duplicateFilterView";
            else if (std::is_same<T, DuplicateSheetRequest>::value)
                name = "duplicateSheet";
            else if (std::is_same<T, FindReplaceRequest>::value)
                name = "findReplace";
            else if (std::is_same<T, InsertDimensionRequest>::value)
                name = "insertDimension";
            else if (std::is_same<T, InsertRangeRequest>::value)
                name = "insertRange";
            else if (std::is_same<T, MoveDimensionRequest>::value)
                name = "moveDimension";
            else if (std::is_same<T, UpdateEmbeddedObjectPositionRequest>::value)
                name = "updateEmbeddedObjectPosition";
            else if (std::is_same<T, PasteDataRequest>::value)
                name = "pasteData";
            else if (std::is_same<T, TextToColumnsRequest>::value)
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
                wr.add<Request &, T>(*this, value, buf, FPSTR(__func__));
        }
    };
}

#endif