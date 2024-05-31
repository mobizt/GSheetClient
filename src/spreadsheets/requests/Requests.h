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
#include "./spreadsheets/requests/DataFilter.h"

namespace GSHEET
{

    /**
     * Updates properties of a spreadsheet.
     */
    class UpdateSpreadsheetPropertiesRequest : public BaseG4
    {
    public:
        UpdateSpreadsheetPropertiesRequest() {}

        // The properties to update.
        UpdateSpreadsheetPropertiesRequest &properties(const SpreadsheetProperties &value) { return wr.set<UpdateSpreadsheetPropertiesRequest &, SpreadsheetProperties>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root 'properties' is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSpreadsheetPropertiesRequest &fields(const String &value) { return wr.set<UpdateSpreadsheetPropertiesRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Adds a new sheet. When a sheet is added at a given index, all subsequent sheets' indexes are incremented. To add an object sheet, use AddChartRequest instead and specify EmbeddedObjectPosition.sheetId or EmbeddedObjectPosition.newSheet.
     */
    class AddSheetRequest : public BaseG1
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
    class DeleteSheetRequest : public BaseG1
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
    class UpdateSheetPropertiesRequest : public BaseG4
    {

    public:
        UpdateSheetPropertiesRequest() {}

        // The properties to update.
        UpdateSheetPropertiesRequest &properties(const SheetProperties &value) { return wr.set<UpdateSheetPropertiesRequest &, SheetProperties>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSheetPropertiesRequest &fields(const String &value) { return wr.set<UpdateSheetPropertiesRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Updates properties of dimensions within the specified range.
     */
    class UpdateDimensionPropertiesRequest : public BaseG4
    {

    public:
        UpdateDimensionPropertiesRequest() {}

        // Properties to update.
        UpdateDimensionPropertiesRequest &properties(const DimensionProperties &value) { return wr.set<UpdateDimensionPropertiesRequest &, DimensionProperties>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDimensionPropertiesRequest &fields(const String &value) { return wr.set<UpdateDimensionPropertiesRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field dimension_range
        // The rows or columns to update.
        UpdateDimensionPropertiesRequest &range(const DimensionRange &value) { return wr.set<UpdateDimensionPropertiesRequest &, DimensionRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Union field dimension_range
        // The columns on a data source sheet to update.
        UpdateDimensionPropertiesRequest &dataSourceSheetRange(const DataSourceSheetDimensionRange &value) { return wr.set<UpdateDimensionPropertiesRequest &, DataSourceSheetDimensionRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Updates properties of the named range with the specified namedRangeId.
     */
    class UpdateNamedRangeRequest : public BaseG4
    {

    public:
        UpdateNamedRangeRequest() {}

        // The named range to update with the new properties.
        UpdateNamedRangeRequest &namedRange(const String &value) { return wr.set<UpdateNamedRangeRequest &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root namedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateNamedRangeRequest &fields(const String &value) { return wr.set<UpdateNamedRangeRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Updates all cells in the range to the values in the given Cell object. Only the fields listed in the fields field are updated; others are unchanged.
     * If writing a cell with a formula, the formula's ranges will automatically increment for each field in the range. For example, if writing a cell with formula =A1 into range B2:C4, B2 would be =A1, B3 would be =A2, B4 would be =A3, C2 would be =B1, C3 would be =B2, C4 would be =B3.
     * keep the formula's ranges static, use the $ indicator. For example, use the formula =$A$1 to prevent both the row and the column from incrementing.
     */
    class RepeatCellRequest : public BaseG4
    {

    public:
        RepeatCellRequest() {}

        // The range to repeat the cell in.
        RepeatCellRequest &range(const GridRange &value) { return wr.set<RepeatCellRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The data to write.
        RepeatCellRequest &cell(const CellData &value) { return wr.set<RepeatCellRequest &, CellData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root cell is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        RepeatCellRequest &fields(const String &value) { return wr.set<RepeatCellRequest &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
    };

    /**
     * Adds a named range to the spreadsheet.
     */
    class AddNamedRangeRequest : public BaseG1
    {
    public:
        AddNamedRangeRequest() {}

        // The named range to add. The namedRangeId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a range that already exists.)
        AddNamedRangeRequest &name(const NamedRange &value) { return wr.add<AddNamedRangeRequest &, NamedRange>(*this, value, buf, FPSTR(__func__)); }
    };
    /**
     * Removes the named range with the given ID from the spreadsheet.
     */
    class DeleteNamedRangeRequest : public BaseG1
    {
    public:
        DeleteNamedRangeRequest() {}

        // The ID of the named range to delete.
        DeleteNamedRangeRequest &namedRangeId(const String &value) { return wr.add<DeleteNamedRangeRequest &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Moves data from the source to the destination.
     */
    class CutPasteRequest : public BaseG4
    {

    public:
        CutPasteRequest() {}

        // The source data to cut.
        CutPasteRequest &source(const GridRange &value) { return wr.set<CutPasteRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The top-left coordinate where the data should be pasted.
        CutPasteRequest &destination(const GridCoordinate &value) { return wr.set<CutPasteRequest &, GridCoordinate>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // What kind of data to paste. All the source data will be cut, regardless of what is pasted.
        CutPasteRequest &pasteType(Paste::PasteType value) { return wr.set<CutPasteRequest &, const char *>(*this, _PasteType[value].text, buf, bufSize, 3, FPSTR(__func__)); }
    };

    class CopyPasteRequest : public BaseG4
    {

    public:
        CopyPasteRequest() {}

        // The source range to copy.
        CopyPasteRequest &source(const GridRange &value) { return wr.set<CopyPasteRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The location to paste to. If the range covers a span that's a multiple of the source's height or width, then the data will be repeated to fill in the destination range. If the range is smaller than the source range, the entire source data will still be copied (beyond the end of the destination range).
        CopyPasteRequest &destination(const GridRange &value) { return wr.set<CopyPasteRequest &, GridRange>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // What kind of data to paste.
        CopyPasteRequest &pasteType(Paste::PasteType value) { return wr.set<CopyPasteRequest &, const char *>(*this, _PasteType[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // How that data should be oriented when pasting.
        CopyPasteRequest &pasteOrientation(Paste::PasteOrientation value) { return wr.set<CopyPasteRequest &, const char *>(*this, _PasteOrientation[value].text, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Merges all cells in the range.
     */
    class MergeCellsRequest : public BaseG4
    {

    public:
        MergeCellsRequest() {}

        // The range of cells to merge.
        MergeCellsRequest &range(const GridRange &value) { return wr.set<MergeCellsRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // How the cells should be merged.
        MergeCellsRequest &mergeType(Merge::MergeType value) { return wr.set<MergeCellsRequest &, const char *>(*this, _MergeType[value].text, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Unmerges cells in the given range.
     */
    class UnmergeCellsRequest : public BaseG1
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
    class UpdateBordersRequest : public BaseG8
    {
    public:
        UpdateBordersRequest() {}

        // The range whose borders should be updated.
        UpdateBordersRequest &range(const GridRange &value) { return wr.set<UpdateBordersRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The border to put at the top of the range.
        UpdateBordersRequest &top(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The border to put at the bottom of the range.
        UpdateBordersRequest &bottom(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The border to put at the left of the range.
        UpdateBordersRequest &left(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The border to put at the right of the range.
        UpdateBordersRequest &right(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The horizontal border to put within the range.
        UpdateBordersRequest &innerHorizontal(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The vertical border to put within the range.
        UpdateBordersRequest &innerVertical(const Border &value) { return wr.set<UpdateBordersRequest &, Border>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }
    };
    /**
     * Updates all cells in a range with new data.
     */
    class UpdateCellsRequest : public BaseG4
    {

    public:
        UpdateCellsRequest() {}

        // This value represents the item to add to an array.
        // The data to write.
        UpdateCellsRequest &rows(const RowData &value) { return wr.append<UpdateCellsRequest &, RowData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields of CellData that should be updated. At least one field must be specified. The root is the CellData; 'row.values.' should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateCellsRequest &fields(const String &value) { return wr.set<UpdateCellsRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field area
        // The coordinate to start writing data at. Any number of rows and columns (including a different number of columns per row) may be written.
        UpdateCellsRequest &start(const GridCoordinate &value) { return wr.set<UpdateCellsRequest &, GridCoordinate>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Union field area
        // The range to write data to.
        // If the data in rows does not cover the entire requested range, the fields matching those set in fields will be cleared.
        UpdateCellsRequest &range(const GridRange &value) { return wr.set<UpdateCellsRequest &, GridRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };
    /**
     * Adds a filter view.
     */
    class AddFilterViewRequest : public BaseG1
    {
    public:
        AddFilterViewRequest() {}

        // The filter to add. The filterViewId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a filter that already exists.)
        AddFilterViewRequest &filter(const FilterView &value) { return wr.add<AddFilterViewRequest &, FilterView>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Adds new cells after the last row with data in a sheet, inserting new rows into the sheet if necessary.
     */
    class AppendCellsRequest : public BaseG4
    {

    public:
        AppendCellsRequest() {}

        // The sheet ID to append the data to.
        AppendCellsRequest &sheetId(int value) { return wr.set<AppendCellsRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The data to append.
        AppendCellsRequest &rows(const RowData &value) { return wr.append<AppendCellsRequest &, RowData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The fields of CellData that should be updated. At least one field must be specified. The root is the CellData; 'row.values.' should not be specified. A single "*" can be used as short-hand for listing every field.
        AppendCellsRequest &fields(const String &value) { return wr.set<AppendCellsRequest &, String>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Clears the basic filter, if any exists on the sheet.
     */
    class ClearBasicFilterRequest : public BaseG1
    {
    public:
        ClearBasicFilterRequest() {}

        // The sheet ID on which the basic filter should be cleared.
        ClearBasicFilterRequest &sheetId(int value) { return wr.add<ClearBasicFilterRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes the dimensions from the sheet.
     */
    class DeleteDimensionRequest : public BaseG1
    {
    public:
        DeleteDimensionRequest() {}

        // The dimensions to delete from the sheet.
        DeleteDimensionRequest &range(const DimensionRange &value) { return wr.add<DeleteDimensionRequest &, DimensionRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes the embedded object with the given ID.
     */
    class DeleteEmbeddedObjectRequest : public BaseG1
    {
    public:
        DeleteEmbeddedObjectRequest() {}

        // The ID of the embedded object to delete.
        DeleteEmbeddedObjectRequest &objectId(int value) { return wr.add<DeleteEmbeddedObjectRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes a particular filter view.
     */
    class DeleteFilterViewRequest : public BaseG1
    {
    public:
        DeleteFilterViewRequest() {}

        // The ID of the filter to delete.
        DeleteFilterViewRequest &filterId(int value) { return wr.add<DeleteFilterViewRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Duplicates a particular filter view.
     */
    class DuplicateFilterViewRequest : public BaseG1
    {
    public:
        DuplicateFilterViewRequest() {}

        // The ID of the filter being duplicated.
        DuplicateFilterViewRequest &filterId(int value) { return wr.add<DuplicateFilterViewRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Duplicates the contents of a sheet.
     */
    class DuplicateSheetRequest : public BaseG6
    {

    public:
        DuplicateSheetRequest() {}

        // The sheet to duplicate.
        // If the source sheet is of DATA_SOURCE type, its backing DataSource is also duplicated and associated with the new copy of the sheet. No data execution is triggered, the grid data of this sheet is also copied over but only available after the batch request completes.
        DuplicateSheetRequest &sourceSheetId(int value) { return wr.set<DuplicateSheetRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The zero-based index where the new sheet should be inserted. The index of all sheets after this are incremented.
        DuplicateSheetRequest &insertSheetIndex(int value) { return wr.set<DuplicateSheetRequest &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // If set, the ID of the new sheet. If not set, an ID is chosen. If set, the ID must not conflict with any existing sheet ID. If set, it must be non-negative.
        DuplicateSheetRequest &newSheetId(int value) { return wr.set<DuplicateSheetRequest &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The name of the new sheet. If empty, a new name is chosen for you.
        DuplicateSheetRequest &newSheetName(const String &value) { return wr.set<DuplicateSheetRequest &, String>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * Finds and replaces data in cells over a range, sheet, or all sheets.
     */
    class FindReplaceRequest : public BaseG8
    {

    public:
        FindReplaceRequest() {}

        // The value to search.
        FindReplaceRequest &find(const String &value) { return wr.set<FindReplaceRequest &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The value to use as the replacement.
        FindReplaceRequest &replacement(const String &value) { return wr.set<FindReplaceRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // True if the search is case sensitive.
        FindReplaceRequest &matchCase(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if the find value should match the entire cell.
        FindReplaceRequest &matchEntireCell(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // True if the find value is a regex. The regular expression and replacement should follow Java regex rules at https://docs.oracle.com/javase/8/docs/api/java/util/regex/Pattern.html. The replacement string is allowed to refer to capturing groups. For example, if one cell has the contents "Google Sheets" and another has "Google Docs", then searching for "o.* (.*)" with a replacement of "$1 Rocks" would change the contents of the cells to "GSheets Rocks" and "GDocs Rocks" respectively.
        FindReplaceRequest &searchByRegex(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // True if the search should include cells with formulas. False to skip cells with formulas.
        FindReplaceRequest &includeFormulas(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // Union field scope
        // The range to find/replace over.
        FindReplaceRequest &range(const GridRange &value) { return wr.set<FindReplaceRequest &, GridRange>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // Union field scope
        // The sheet to find/replace over.
        FindReplaceRequest &sheetId(int value) { return wr.set<FindReplaceRequest &, int>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // Union field scope
        // True to find/replace over all sheets.
        FindReplaceRequest &allSheets(bool value) { return wr.set<FindReplaceRequest &, bool>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }
    };

    /**
     * Inserts rows or columns in a sheet at a particular index.
     */
    class InsertDimensionRequest : public BaseG4
    {

    public:
        InsertDimensionRequest() {}

        // The dimensions to insert. Both the start and end indexes must be bounded.
        InsertDimensionRequest &range(const DimensionRange &value) { return wr.set<InsertDimensionRequest &, DimensionRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Whether dimension properties should be extended from the dimensions before or after the newly inserted dimensions. True to inherit from the dimensions before (in which case the start index must be greater than 0), and false to inherit from the dimensions after.
        // For example, if row index 0 has red background and row index 1 has a green background, then inserting 2 rows at index 1 can inherit either the green or red background. If inheritFromBefore is true, the two new rows will be red (because the row before the insertion point was red), whereas if inheritFromBefore is false, the two new rows will be green (because the row after the insertion point was green).
        InsertDimensionRequest &inheritFromBefore(bool value) { return wr.set<InsertDimensionRequest &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Inserts cells into a range, shifting the existing cells over or down.
     */
    class InsertRangeRequest : public BaseG4
    {

    public:
        InsertRangeRequest() {}

        // The range to insert new cells into.
        InsertRangeRequest &range(const GridRange &value) { return wr.set<InsertRangeRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The dimension which will be shifted when inserting cells. If ROWS, existing cells will be shifted down. If COLUMNS, existing cells will be shifted right.
        InsertRangeRequest &shiftDimension(Dimensions::Dimension value) { return wr.set<InsertRangeRequest &, const char *>(*this, _Dimension[value].text, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Moves one or more rows or columns.
     */
    class MoveDimensionRequest : public BaseG4
    {
    public:
        MoveDimensionRequest() {}

        // The source dimensions to move.
        MoveDimensionRequest &source(const DimensionRange &value) { return wr.set<MoveDimensionRequest &, DimensionRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The zero-based start index of where to move the source data to, based on the coordinates before the source data is removed from the grid. Existing data will be shifted down or right (depending on the dimension) to make room for the moved dimensions. The source dimensions are removed from the grid, so the the data may end up in a different index than specified.
        // For example, given A1..A5 of 0, 1, 2, 3, 4 and wanting to move "1" and "2" to between "3" and "4", the source would be ROWS [1..3),and the destination index would be "4" (the zero-based index of row 5). The end result would be A1..A5 of 0, 3, 1, 2, 4.
        MoveDimensionRequest &destinationIndex(int value) { return wr.set<MoveDimensionRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
    };

    /**
     * Update an embedded object's position (such as a moving or resizing a chart or image).
     */
    class UpdateEmbeddedObjectPositionRequest : public BaseG4
    {
    public:
        UpdateEmbeddedObjectPositionRequest() {}

        // The ID of the object to moved.
        UpdateEmbeddedObjectPositionRequest &objectId(int value) { return wr.set<UpdateEmbeddedObjectPositionRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // An explicit position to move the embedded object to. If newPosition.sheetId is set, a new sheet with that ID will be created. If newPosition.newSheet is set to true, a new sheet will be created with an ID that will be chosen for you.
        UpdateEmbeddedObjectPositionRequest &newPosition(const EmbeddedObjectPosition &value) { return wr.set<UpdateEmbeddedObjectPositionRequest &, EmbeddedObjectPosition>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields of OverlayPosition that should be updated when setting a new position. Used only if newPosition.overlayPosition is set, in which case at least one field must be specified. The root newPosition.overlayPosition is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateEmbeddedObjectPositionRequest &fields(const String &value) { return wr.set<UpdateEmbeddedObjectPositionRequest &, String>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Inserts data into the spreadsheet starting at the specified coordinate.
     */
    class PasteDataRequest : public BaseG6
    {

    public:
        PasteDataRequest() {}

        // The coordinate at which the data should start being inserted.
        PasteDataRequest &coordinate(const GridCoordinate &value) { return wr.set<PasteDataRequest &, GridCoordinate>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The data to insert.
        PasteDataRequest &data(const String &value) { return wr.set<PasteDataRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // How the data should be pasted.
        PasteDataRequest &type(Paste::PasteType value) { return wr.set<PasteDataRequest &, const char *>(*this, _PasteType[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // Union field kind
        // The delimiter in the data.
        PasteDataRequest &delimiter(const String &value) { return wr.set<PasteDataRequest &, String>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Union field kind
        // True if the data is HTML.
        PasteDataRequest &html(bool value) { return wr.set<PasteDataRequest &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * Splits a column of text into multiple columns, based on a delimiter in each cell.
     */
    class TextToColumnsRequest : public BaseG4
    {

    public:
        TextToColumnsRequest() {}

        // The source data range. This must span exactly one column.
        TextToColumnsRequest &source(const GridRange &value) { return wr.set<TextToColumnsRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The delimiter to use. Used only if delimiterType is CUSTOM.
        TextToColumnsRequest &delimiter(const String &value) { return wr.set<TextToColumnsRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The delimiter type to use.
        TextToColumnsRequest &delimiterType(Delim::DelimiterType value) { return wr.set<TextToColumnsRequest &, const char *>(*this, _DelimiterType[value].text, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Updates properties of the filter view.
     */
    class UpdateFilterViewRequest : public BaseG4
    {

    public:
        UpdateFilterViewRequest() {}

        // The new properties of the filter view.
        UpdateFilterViewRequest &filter(const FilterView &value) { return wr.set<UpdateFilterViewRequest &, FilterView>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root filter is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateFilterViewRequest &fields(const String &value) { return wr.set<UpdateFilterViewRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Deletes a range of cells, shifting other cells into the deleted area.
     */
    class DeleteRangeRequest : public BaseG4
    {

    public:
        DeleteRangeRequest() {}

        // The range of cells to delete.
        DeleteRangeRequest &range(const GridRange &value) { return wr.set<DeleteRangeRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The dimension from which deleted cells will be replaced with. If ROWS, existing cells will be shifted upward to replace the deleted cells. If COLUMNS, existing cells will be shifted left to replace the deleted cells.
        DeleteRangeRequest &shiftDimension(Dimensions::Dimension value) { return wr.set<DeleteRangeRequest &, const char *>(*this, _Dimension[value].text, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Appends rows or columns to the end of a sheet.
     */
    class AppendDimensionRequest : public BaseG4
    {

    public:
        AppendDimensionRequest() {}

        // The sheet to append rows or columns to.
        AppendDimensionRequest &sheetId(int value) { return wr.set<AppendDimensionRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Whether rows or columns should be appended.
        AppendDimensionRequest &dimension(Dimensions::Dimension value) { return wr.set<AppendDimensionRequest &, const char *>(*this, _Dimension[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // The number of rows or columns to append.
        AppendDimensionRequest &length(int value) { return wr.set<AppendDimensionRequest &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Adds a new conditional format rule at the given index. All subsequent rules' indexes are incremented.
     */
    class AddConditionalFormatRuleRequest : public BaseG4
    {

    public:
        AddConditionalFormatRuleRequest() {}

        // The rule to add.
        AddConditionalFormatRuleRequest &rule(const ConditionalFormatRule &value) { return wr.set<AddConditionalFormatRuleRequest &, ConditionalFormatRule>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The zero-based index where the rule should be inserted.
        AddConditionalFormatRuleRequest &index(int value) { return wr.set<AddConditionalFormatRuleRequest &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Updates a conditional format rule at the given index, or moves a conditional format rule to another index.
     */
    class UpdateConditionalFormatRuleRequest : public BaseG4
    {

    public:
        UpdateConditionalFormatRuleRequest() {}

        // The zero-based index of the rule that should be replaced or moved.
        UpdateConditionalFormatRuleRequest &index(int value) { return wr.set<UpdateConditionalFormatRuleRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The sheet of the rule to move. Required if newIndex is set, unused otherwise.
        UpdateConditionalFormatRuleRequest &sheetId(int value) { return wr.set<UpdateConditionalFormatRuleRequest &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field instruction.
        // The rule that should replace the rule at the given index.
        UpdateConditionalFormatRuleRequest &rule(const ConditionalFormatRule &value) { return wr.set<UpdateConditionalFormatRuleRequest &, ConditionalFormatRule>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Union field instruction.
        // The zero-based new index the rule should end up at.
        UpdateConditionalFormatRuleRequest &newIndex(int value) { return wr.set<UpdateConditionalFormatRuleRequest &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Deletes a conditional format rule at the given index. All subsequent rules' indexes are decremented.
     */
    class DeleteConditionalFormatRuleRequest : public BaseG4
    {

    public:
        DeleteConditionalFormatRuleRequest() {}

        // The zero-based index of the rule to be deleted.
        DeleteConditionalFormatRuleRequest &index(int value) { return wr.set<DeleteConditionalFormatRuleRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The sheet the rule is being deleted from.
        DeleteConditionalFormatRuleRequest &sheetId(int value) { return wr.set<DeleteConditionalFormatRuleRequest &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Sorts data in rows based on a sort order per column.
     */
    class SortRangeRequest : public BaseG4
    {

    public:
        SortRangeRequest() {}

        // The range to sort.
        SortRangeRequest &range(const GridRange &value) { return wr.set<SortRangeRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The sort order per column. Later specifications are used when values are equal in the earlier specifications.
        SortRangeRequest &sortSpecs(const SortSpec &value) { return wr.append<SortRangeRequest &, SortSpec>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Sets a data validation rule to every cell in the range. To clear validation in a range, call this with no rule specified.
     */
    class SetDataValidationRequest : public BaseG4
    {

    public:
        SetDataValidationRequest() {}

        // The range the data validation rule should apply to.
        SetDataValidationRequest &range(const GridRange &value) { return wr.set<SetDataValidationRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The data validation rule to set on each cell in the range, or empty to clear the data validation in the range.
        SetDataValidationRequest &rule(const DataValidationRule &value) { return wr.set<SetDataValidationRequest &, DataValidationRule>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Sets the basic filter associated with a sheet.
     */
    class SetBasicFilterRequest : public BaseG1
    {

    public:
        SetBasicFilterRequest() {}

        // The filter to set.
        SetBasicFilterRequest &filyer(const BasicFilter &value) { return wr.add<SetBasicFilterRequest &, BasicFilter>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Adds a new protected range.
     */
    class AddProtectedRangeRequest : public BaseG1
    {

    public:
        AddProtectedRangeRequest() {}

        // The protected range to be added. The protectedRangeId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a range that already exists.)
        AddProtectedRangeRequest &protectedRange(const ProtectedRange &value) { return wr.add<AddProtectedRangeRequest &, ProtectedRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates an existing protected range with the specified protectedRangeId.
     */
    class UpdateProtectedRangeRequest : public BaseG4
    {

    public:
        UpdateProtectedRangeRequest() {}

        // The protected range to update with the new properties.
        UpdateProtectedRangeRequest &protectedRange(const ProtectedRange &value) { return wr.set<UpdateProtectedRangeRequest &, ProtectedRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root protectedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateProtectedRangeRequest &fields(const String &value) { return wr.set<UpdateProtectedRangeRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Deletes the protected range with the given ID.
     */
    class DeleteProtectedRangeRequest : public BaseG1
    {

    public:
        DeleteProtectedRangeRequest() {}

        // The ID of the protected range to delete.
        DeleteProtectedRangeRequest &protectedRangeId(int value) { return wr.add<DeleteProtectedRangeRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Automatically resizes one or more dimensions based on the contents of the cells in that dimension.
     */
    class AutoResizeDimensionsRequest : public BaseG1
    {

    public:
        AutoResizeDimensionsRequest() {}

        // Union field dimension_range
        // The dimensions to automatically resize.
        AutoResizeDimensionsRequest &dimensions(const DimensionRange &value) { return wr.add<AutoResizeDimensionsRequest &, DimensionRange>(*this, value, buf, FPSTR(__func__)); }

        // Union field dimension_range
        // The dimensions on a data source sheet to automatically resize.
        AutoResizeDimensionsRequest &dataSourceSheetDimensions(const DataSourceSheetDimensionRange &value) { return wr.add<AutoResizeDimensionsRequest &, DataSourceSheetDimensionRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Adds a chart to a sheet in the spreadsheet.
     */
    class AddChartRequest : public BaseG1
    {

    public:
        AddChartRequest() {}

        // The chart that should be added to the spreadsheet, including the position where it should be placed. The chartId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of an embedded object that already exists.)
        AddChartRequest &chart(const EmbeddedChart &value) { return wr.add<AddChartRequest &, EmbeddedChart>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates a chart's specifications. (This does not move or resize a chart. To move or resize a chart, use UpdateEmbeddedObjectPositionRequest.)
     */
    class UpdateChartSpecRequest : public BaseG4
    {

    public:
        UpdateChartSpecRequest() {}

        // The ID of the chart to update.
        UpdateChartSpecRequest &chartId(int value) { return wr.set<UpdateChartSpecRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The specification to apply to the chart.
        UpdateChartSpecRequest &spec(const ChartSpec &value) { return wr.set<UpdateChartSpecRequest &, ChartSpec>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Updates properties of the supplied banded range.
     */
    class UpdateBandingRequest : public BaseG4
    {

    public:
        UpdateBandingRequest() {}

        // The banded range to update with the new properties.
        UpdateBandingRequest &bandedRange(const BandedRange &value) { return wr.set<UpdateBandingRequest &, BandedRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root bandedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateBandingRequest &fields(const String &value) { return wr.set<UpdateBandingRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Adds a new banded range to the spreadsheet.
     */
    class AddBandingRequest : public BaseG1
    {

    public:
        AddBandingRequest() {}

        // The banded range to add. The bandedRangeId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a range that already exists.)
        AddBandingRequest &bandedRange(const BandedRange &value) { return wr.add<AddBandingRequest &, BandedRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Removes the banded range with the given ID from the spreadsheet.
     */
    class DeleteBandingRequest : public BaseG1
    {

    public:
        DeleteBandingRequest() {}

        // The ID of the banded range to delete.
        DeleteBandingRequest &bandedRangeId(int value) { return wr.add<DeleteBandingRequest &, int>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A request to create developer metadata.
     */
    class CreateDeveloperMetadataRequest : public BaseG1
    {

    public:
        CreateDeveloperMetadataRequest() {}

        // The developer metadata to create.
        CreateDeveloperMetadataRequest &developerMetadata(const DeveloperMetadata &value) { return wr.add<CreateDeveloperMetadataRequest &, DeveloperMetadata>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A request to update properties of developer metadata. Updates the properties of the developer metadata selected by the filters to the values provided in the DeveloperMetadata resource. Callers must specify the properties they wish to update in the fields parameter, as well as specify at least one DataFilter matching the metadata they wish to update.
     */
    class UpdateDeveloperMetadataRequest : public BaseG4
    {

    public:
        UpdateDeveloperMetadataRequest() {}

        // This value represents the item to add to an array.
        // The filters matching the developer metadata entries to update.
        UpdateDeveloperMetadataRequest &dataFilters(const DataFilter &value) { return wr.append<UpdateDeveloperMetadataRequest &, DataFilter>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The value that all metadata matched by the data filters will be updated to.
        UpdateDeveloperMetadataRequest &developerMetadata(const DeveloperMetadata &value) { return wr.set<UpdateDeveloperMetadataRequest &, DeveloperMetadata>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root developerMetadata is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDeveloperMetadataRequest &fields(const String &value) { return wr.set<UpdateDeveloperMetadataRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A request to delete developer metadata.
     */
    class DeleteDeveloperMetadataRequest : public BaseG1
    {

    public:
        DeleteDeveloperMetadataRequest() {}

        // The data filter describing the criteria used to select which developer metadata entry to delete.
        DeleteDeveloperMetadataRequest &dataFilter(const DataFilter &value) { return wr.add<DeleteDeveloperMetadataRequest &, DataFilter>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Randomizes the order of the rows in a range.
     */
    class RandomizeRangeRequest : public BaseG1
    {

    public:
        RandomizeRangeRequest() {}

        // The range to randomize.
        RandomizeRangeRequest &range(const GridRange &value) { return wr.add<RandomizeRangeRequest &, GridRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Creates a group over the specified range.
     * If the requested range is a superset of the range of an existing group G, then the depth of G is incremented and this new group G' has the depth of that group. For example, a group [C:D, depth 1] + [B:E] results in groups [B:E, depth 1] and [C:D, depth 2]. If the requested range is a subset of the range of an existing group G, then the depth of the new group G' becomes one greater than the depth of G. For example, a group [B:E, depth 1] + [C:D] results in groups [B:E, depth 1] and [C:D, depth 2]. If the requested range starts before and ends within, or starts within and ends after, the range of an existing group G, then the range of the existing group G becomes the union of the ranges, and the new group G' has depth one greater than the depth of G and range as the intersection of the ranges. For example, a group [B:D, depth 1] + [C:E] results in groups [B:E, depth 1] and [C:D, depth 2].
     */
    class AddDimensionGroupRequest : public BaseG1
    {

    public:
        AddDimensionGroupRequest() {}

        // The range over which to create a group.
        AddDimensionGroupRequest &range(const DimensionRange &value) { return wr.add<AddDimensionGroupRequest &, DimensionRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Deletes a group over the specified range by decrementing the depth of the dimensions in the range.
     * For example, assume the sheet has a depth-1 group over B:E and a depth-2 group over C:D. Deleting a group over D:E leaves the sheet with a depth-1 group over B:D and a depth-2 group over C:C.
     */
    class DeleteDimensionGroupRequest : public BaseG1
    {

    public:
        DeleteDimensionGroupRequest() {}

        // The range of the group to be deleted.
        DeleteDimensionGroupRequest &range(const DimensionRange &value) { return wr.add<DeleteDimensionGroupRequest &, DimensionRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates the state of the specified group.
     */
    class UpdateDimensionGroupRequest : public BaseG4
    {

    public:
        UpdateDimensionGroupRequest() {}

        // The group whose state should be updated. The range and depth of the group should specify a valid group on the sheet, and all other fields updated.
        UpdateDimensionGroupRequest &dimensionGroup(const DimensionGroup &value) { return wr.set<UpdateDimensionGroupRequest &, DimensionGroup>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root dimensionGroup is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDimensionGroupRequest &fields(const String &value) { return wr.set<UpdateDimensionGroupRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Trims the whitespace (such as spaces, tabs, or new lines) in every cell in the specified range. This request removes all whitespace from the start and end of each cell's text, and reduces any subsequence of remaining whitespace characters to a single space. If the resulting trimmed text starts with a '+' or '=' character, the text remains as a string value and isn't interpreted as a formula.
     */
    class TrimWhitespaceRequest : public BaseG1
    {

    public:
        TrimWhitespaceRequest() {}

        // The range whose cells to trim.
        TrimWhitespaceRequest &range(const GridRange &value) { return wr.add<TrimWhitespaceRequest &, GridRange>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Removes rows within this range that contain values in the specified columns that are duplicates of values in any previous row. Rows with identical values but different letter cases, formatting, or formulas are considered to be duplicates.
     * This request also removes duplicate rows hidden from view (for example, due to a filter). When removing duplicates, the first instance of each duplicate row scanning from the top downwards is kept in the resulting range. Content outside of the specified range isn't removed, and rows considered duplicates do not have to be adjacent to each other in the range.
     */
    class DeleteDuplicatesRequest : public BaseG4
    {

    public:
        DeleteDuplicatesRequest() {}

        // The range to remove duplicates rows from.
        DeleteDuplicatesRequest &range(const GridRange &value) { return wr.set<DeleteDuplicatesRequest &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The columns in the range to analyze for duplicate values. If no columns are selected then all columns are analyzed for duplicates.
        DeleteDuplicatesRequest &comparisonColumns(const DimensionRange &value) { return wr.append<DeleteDuplicatesRequest &, DimensionRange>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Updates an embedded object's border property.
     */
    class UpdateEmbeddedObjectBorderRequest : public BaseG4
    {

    public:
        UpdateEmbeddedObjectBorderRequest() {}

        // The ID of the embedded object to update.
        UpdateEmbeddedObjectBorderRequest &objectId(int value) { return wr.set<UpdateEmbeddedObjectBorderRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The border that applies to the embedded object.
        UpdateEmbeddedObjectBorderRequest &border(const EmbeddedObjectBorder &value) { return wr.set<UpdateEmbeddedObjectBorderRequest &, EmbeddedObjectBorder>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root developerMetadata is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateEmbeddedObjectBorderRequest &fields(const String &value) { return wr.set<UpdateEmbeddedObjectBorderRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Adds a slicer to a sheet in the spreadsheet.
     */
    class AddSlicerRequest : public BaseG1
    {

    public:
        AddSlicerRequest() {}

        // The slicer that should be added to the spreadsheet, including the position where it should be placed. The slicerId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a slicer that already exists.)
        AddSlicerRequest &slicer(const Slicer &value) { return wr.add<AddSlicerRequest &, Slicer>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates a slicer's specifications. (This does not move or resize a slicer. To move or resize a slicer use UpdateEmbeddedObjectPositionRequest.
     */
    class UpdateSlicerSpecRequest : public BaseG4
    {

    public:
        UpdateSlicerSpecRequest() {}

        // The id of the slicer to update.
        UpdateSlicerSpecRequest &slicerId(int value) { return wr.set<UpdateSlicerSpecRequest &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The specification to apply to the slicer.
        UpdateSlicerSpecRequest &spec(const SlicerSpec &value) { return wr.set<UpdateSlicerSpecRequest &, SlicerSpec>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root developerMetadata is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSlicerSpecRequest &fields(const String &value) { return wr.set<UpdateSlicerSpecRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Adds a data source. After the data source is added successfully, an associated DATA_SOURCE sheet is created and an execution is triggered to refresh the sheet to read data from the data source. The request requires an additional bigquery.readonly OAuth scope.
     */
    class AddDataSourceRequest : public BaseG1
    {

    public:
        AddDataSourceRequest() {}

        // The data source to add.
        AddDataSourceRequest &dataSource(const DataSource &value) { return wr.add<AddDataSourceRequest &, DataSource>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Updates a data source. After the data source is updated successfully, an execution is triggered to refresh the associated DATA_SOURCE sheet to read data from the updated data source. The request requires an additional bigquery.readonly OAuth scope.
     */
    class UpdateDataSourceRequest : public BaseG4
    {

    public:
        UpdateDataSourceRequest() {}

        // The data source to update.
        UpdateDataSourceRequest &dataSource(const DataSource &value) { return wr.set<UpdateDataSourceRequest &, DataSource>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The fields that should be updated. At least one field must be specified. The root dataSource is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDataSourceRequest &fields(const String &value) { return wr.set<UpdateDataSourceRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Deletes a data source. The request also deletes the associated data source sheet, and unlinks all associated data source objects.
     */
    class DeleteDataSourceRequest : public BaseG1
    {

    public:
        DeleteDataSourceRequest() {}

        // The ID of the data source to delete.
        DeleteDataSourceRequest &dataSourceId(const String &value) { return wr.add<DeleteDataSourceRequest &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Reference to a data source object.
     */
    class DataSourceObjectReference : public BaseG1
    {

    public:
        DataSourceObjectReference() {}

        // Union field value.
        // References to a DATA_SOURCE sheet.
        DataSourceObjectReference &sheetId(const String &value) { return wr.add<DataSourceObjectReference &, String>(*this, value, buf, FPSTR(__func__)); }

        // Union field value.
        // References to a data source chart.
        DataSourceObjectReference &chartId(int value) { return wr.add<DataSourceObjectReference &, int>(*this, value, buf, FPSTR(__func__)); }

        // Union field value.
        // References to a DataSourceTable anchored at the cell.
        DataSourceObjectReference &dataSourceTableAnchorCell(const GridCoordinate &value) { return wr.add<DataSourceObjectReference &, GridCoordinate>(*this, value, buf, FPSTR(__func__)); }

        // Union field value.
        // References to a data source PivotTable anchored at the cell.
        DataSourceObjectReference &dataSourcePivotTableAnchorCell(const GridCoordinate &value) { return wr.add<DataSourceObjectReference &, GridCoordinate>(*this, value, buf, FPSTR(__func__)); }

        // Union field value.
        // References to a cell containing DataSourceFormula.
        DataSourceObjectReference &dataSourceFormulaCell(const GridCoordinate &value) { return wr.add<DataSourceObjectReference &, GridCoordinate>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A list of references to data source objects.
     */
    class DataSourceObjectReferences : public BaseG2
    {

    public:
        DataSourceObjectReferences() {}

        // This value represents the item to add to an array.
        // The references.
        DataSourceObjectReferences &references(const DataSourceObjectReference &value) { return wr.append<DataSourceObjectReferences &, DataSourceObjectReference>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
    };

    /**
     * Refreshes one or multiple data source objects in the spreadsheet by the specified references. The request requires an additional bigquery.readonly OAuth scope.
     * If there are multiple refresh requests referencing the same data source objects in one batch, only the last refresh request is processed, and all those requests will have the same response accordingly.
     */
    class RefreshDataSourceRequest : public BaseG4
    {

    public:
        RefreshDataSourceRequest() {}

        // Refreshes the data source objects regardless of the current state. If not set and a referenced data source object was in error state, the refresh will fail immediately.
        RefreshDataSourceRequest &force(bool value) { return wr.set<RefreshDataSourceRequest &, bool>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Union field target.
        // References to data source objects to refresh.
        RefreshDataSourceRequest &references(const DataSourceObjectReferences &value) { return wr.set<RefreshDataSourceRequest &, DataSourceObjectReferences>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field target.
        // Reference to a DataSource. If specified, refreshes all associated data source objects for the data source.
        RefreshDataSourceRequest &dataSourceId(const String &value) { return wr.set<RefreshDataSourceRequest &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field target.
        // Refreshes all existing data source objects in the spreadsheet.
        RefreshDataSourceRequest &isAll(bool value) { return wr.set<RefreshDataSourceRequest &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Fills in more data based on existing data.
     */
    class AutoFillRequest : public BaseG4
    {

    public:
        AutoFillRequest() {}

        // True if we should generate data with the "alternate" series. This differs based on the type and amount of source data.
        AutoFillRequest &useAlternateSeries(bool value) { return wr.set<AutoFillRequest &, bool>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Union field area
        // The range to autofill. This will examine the range and detect the location that has data and automatically fill that data in to the rest of the range.
        AutoFillRequest &range(const GridRange &value) { return wr.set<AutoFillRequest &, GridRange>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field area
        // The source and destination areas to autofill. This explicitly lists the source of the autofill and where to extend that data.
        AutoFillRequest &sourceAndDestination(const SourceAndDestination &value) { return wr.set<AutoFillRequest &, SourceAndDestination>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A single kind of update to apply to a spreadsheet.
     */
    template <typename T>
    class Request : public BaseG1
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
            else if (std::is_same<T, UpdateFilterViewRequest>::value)
                name = "updateFilterView";
            else if (std::is_same<T, DeleteRangeRequest>::value)
                name = "deleteRange";
            else if (std::is_same<T, AppendDimensionRequest>::value)
                name = "appendDimension";
            else if (std::is_same<T, AddConditionalFormatRuleRequest>::value)
                name = "addConditionalFormatRule";
            else if (std::is_same<T, UpdateConditionalFormatRuleRequest>::value)
                name = "updateConditionalFormatRule";
            else if (std::is_same<T, DeleteConditionalFormatRuleRequest>::value)
                name = "deleteConditionalFormatRule";
            else if (std::is_same<T, SortRangeRequest>::value)
                name = "sortRange";
            else if (std::is_same<T, SetDataValidationRequest>::value)
                name = "setDataValidation";
            else if (std::is_same<T, SetBasicFilterRequest>::value)
                name = "setBasicFilter";
            else if (std::is_same<T, AddProtectedRangeRequest>::value)
                name = "addProtectedRange";
            else if (std::is_same<T, UpdateProtectedRangeRequest>::value)
                name = "updateProtectedRange";
            else if (std::is_same<T, DeleteProtectedRangeRequest>::value)
                name = "deleteProtectedRange";
            else if (std::is_same<T, AutoResizeDimensionsRequest>::value)
                name = "autoResizeDimensions";
            else if (std::is_same<T, AddChartRequest>::value)
                name = "addChart";
            else if (std::is_same<T, UpdateChartSpecRequest>::value)
                name = "updateChartSpec";
            else if (std::is_same<T, UpdateBandingRequest>::value)
                name = "updateBanding";
            else if (std::is_same<T, AddBandingRequest>::value)
                name = "addBanding";
            else if (std::is_same<T, DeleteBandingRequest>::value)
                name = "deleteBanding";
            else if (std::is_same<T, CreateDeveloperMetadataRequest>::value)
                name = "createDeveloperMetadata";
            else if (std::is_same<T, UpdateDeveloperMetadataRequest>::value)
                name = "updateDeveloperMetadata";
            else if (std::is_same<T, DeleteDeveloperMetadataRequest>::value)
                name = "deleteDeveloperMetadata";
            else if (std::is_same<T, RandomizeRangeRequest>::value)
                name = "randomizeRange";
            else if (std::is_same<T, AddDimensionGroupRequest>::value)
                name = "addDimensionGroup";
            else if (std::is_same<T, DeleteDimensionGroupRequest>::value)
                name = "deleteDimensionGroup";
            else if (std::is_same<T, UpdateDimensionGroupRequest>::value)
                name = "updateDimensionGroup";
            else if (std::is_same<T, TrimWhitespaceRequest>::value)
                name = "trimWhitespace";
            else if (std::is_same<T, DeleteDuplicatesRequest>::value)
                name = "deleteDuplicates";
            else if (std::is_same<T, UpdateEmbeddedObjectBorderRequest>::value)
                name = "updateEmbeddedObjectBorder";
            else if (std::is_same<T, AddSlicerRequest>::value)
                name = "addSlicer";
            else if (std::is_same<T, UpdateSlicerSpecRequest>::value)
                name = "updateSlicerSpec";
            else if (std::is_same<T, AddDataSourceRequest>::value)
                name = "addDataSource";
            else if (std::is_same<T, UpdateDataSourceRequest>::value)
                name = "updateDataSource";
            else if (std::is_same<T, DeleteDataSourceRequest>::value)
                name = "deleteDataSource";
            else if (std::is_same<T, RefreshDataSourceRequest>::value)
                name = "refreshDataSource";

            if (name.length())
                wr.add<Request &, T>(*this, value, buf, FPSTR(__func__));
        }
    };
}

#endif