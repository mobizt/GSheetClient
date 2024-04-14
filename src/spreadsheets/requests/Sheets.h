#ifndef SHEETS_H
#define SHEETS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/Charts.h"
#include "./spreadsheets/requests/Dimension.h"
#include "./spreadsheets/requests/Cell.h"
#include "./spreadsheets/requests/Common.h"

/**
 * SHEET CLASS DEPENDENCIES
 *
 * Sheet            SheetProperties         (enum) SheetType
 *                                          GridProperties
 *                                          ColorStyle*
 *                                          DataSourceSheetProperties               DataSourceColumn                    DataSourceColumnReference
 *
 *                                                                                  DataExecutionStatus                 (enum) DataExecutionState
 *                                                                                                                      (enum) DataExecutionErrorCode
 *
 *                  GridData                RowData                                 CellData*
 *
 *                                          DimensionProperties                     DeveloperMetadata                   DeveloperMetadataLocation           (enum) DeveloperMetadataLocationType
 *                                                                                                                                                          DimensionRange
 *
 *                                                                                                                      (enum) DeveloperMetadataVisibility
 *
 *                                                                                  DataSourceColumnReference
 *
 *                  GridRange
 *                  ConditionalFormatRule   GridRange
 *                                          BooleanRule                             BooleanCondition*
 *                                                                                  CellFormat*
 *
 *                                          GradientRule                            InterpolationPoint                  ColorStyle
 *                                                                                                                      (enum) InterpolationPointType
 *
 *                  FilterView              GridRange
 *                                          SortSpec*
 *                                          FilterSpec*
 *
 *                  ProtectedRange          GridRange
 *                                          Editors
 *
 *                  BasicFilter             GridRange
 *                                          SortSpec*
 *                                          FilterSpec*
 *
 *                  SheetProperties           ChartSpec                               TextFormat
 *                                                                                  TextPosition
 *                                                                                  ColorStyle*
 *                                                                                  DataSourceChartProperties
 *                                                                                  FilterSpec*
 *                                                                                  SortSpec*
 *                                                                                  (enum) ChartHiddenDimensionStrategy
 *                                                                                  BasicChartSpec*
 *                                                                                  PieChartSpec*
 *                                                                                  BubbleChartSpec*
 *                                                                                  CandlestickChartSpec*
 *                                                                                  OrgChartSpec*
 *                                                                                  HistogramChartSpec*
 *                                                                                  WaterfallChartSpec*
 *                                                                                  TreemapChartSpec*
 *                                                                                  ScorecardChartSpec*
 *
 *                                          EmbeddedObjectPosition                  OverlayPosition                             GridCoordinate
 *
 *                                          EmbeddedObjectBorder                    ColorStyle*
 *
 *                  BandedRange             GridRange
 *                                          BandingProperties                       ColorStyle*
 *
 *                  DeveloperMetadata       DeveloperMetadataLocation*
 *                                          (enum) DeveloperMetadataVisibility
 *
 *                  DimensionGroup          DimensionRange*
 *
 *                  Slicer                  SlicerSpec                              GridRange
 *                                                                                  FilterCriteria*
 *                                                                                  TextFormat
 *                                                                                  ColorStyle*
 *                                                                                  (enum) HorizontalAlign
 *
 *                                          EmbeddedObjectPosition                  OverlayPosition
 *                                                                                  GridCoordinate*
 *
 */

namespace GSHEET
{

    /**
     * Properties of a grid.
     */
    class GridProperties : public BaseG8
    {
    public:
        GridProperties() = default;

        // The number of rows in the grid.
        GridProperties &rowCount(int value) { return wr.set<GridProperties &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The number of columns in the grid.
        GridProperties &columnCount(int value) { return wr.set<GridProperties &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The number of rows that are frozen in the grid.
        GridProperties &frozenRowCount(int value) { return wr.set<GridProperties &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The number of columns that are frozen in the grid.
        GridProperties &frozenColumnCount(int value) { return wr.set<GridProperties &, int>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // True if the grid isn't showing gridlines in the UI.
        GridProperties &hideGridlines(bool value) { return wr.set<GridProperties &, bool>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // True if the row grouping control toggle is shown after the group.
        GridProperties &rowGroupControlAfter(bool value) { return wr.set<GridProperties &, bool>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // True if the column grouping control toggle is shown after the group.
        GridProperties &columnGroupControlAfter(bool value) { return wr.set<GridProperties &, bool>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }
    };

    /**
     * Properties of a spreadsheet.
     */
    class SheetProperties : public BaseG12
    {
    public:
        SheetProperties() = default;

        // The ID of the sheet. Must be non-negative. This field cannot be changed once set.
        SheetProperties &sheetId(int value) { return wr.set<SheetProperties &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The name of the sheet.
        SheetProperties &title(const String &value) { return wr.set<SheetProperties &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The index of the sheet within the spreadsheet. When adding or updating sheet properties, if this field is excluded then the sheet is added or moved to the end of the sheet list. When updating sheet indices or inserting sheets, movement is considered in "before the move" indexes. For example, if there were three sheets (S1, S2, S3) in order to move S1 ahead of S2 the index would have to be set to 2. A sheet index update request is ignored if the requested index is identical to the sheets current index or if the requested new index is equal to the current sheet index + 1.
        SheetProperties &index(int value) { return wr.set<SheetProperties &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The type of sheet. Defaults to GRID. This field cannot be changed once set.
        SheetProperties &sheetType(SheetType value) { return wr.set<SheetProperties &, const char *>(*this, _SheetType[value].text, buf, bufSize, 4, FPSTR(__func__)); }

        // Additional properties of the sheet if this sheet is a grid. (If the sheet is an object sheet, containing a chart or image, then this field will be absent.) When writing it is an error to set any grid properties on non-grid sheets.
        // If this sheet is a DATA_SOURCE sheet, this field is output only but contains the properties that reflect how a data source sheet is rendered in the UI, e.g. rowCount.
        SheetProperties &gridProperties(const GridProperties &value) { return wr.set<SheetProperties &, GridProperties>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // True if the sheet is hidden in the UI, false if it's visible.
        SheetProperties &hidden(bool value) { return wr.set<SheetProperties &, bool>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The color of the tab in the UI.
        SheetProperties &tabColorStyle(const ColorStyle &value) { return wr.set<SheetProperties &, ColorStyle>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // True if the sheet is an RTL sheet instead of an LTR sheet.
        SheetProperties &rightToLeft(bool value) { return wr.set<SheetProperties &, bool>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }
    };

    /**
     * Data about each cell in a row.
     */
    class RowData : public BaseG2
    {
    public:
        RowData() = default;

        // This value represents the item to add to an array.
        // The values in the row, one per column.
        RowData &values(const CellData &value) { return wr.append<RowData &, CellData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
    };

    /**
     * Data in the grid, as well as metadata about the dimensions.
     */
    class GridData : public BaseG6
    {
    public:
        GridData() = default;

        // The first row this GridData refers to, zero-based.
        GridData &startRow(int value) { return wr.set<GridData &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The first column this GridData refers to, zero-based.
        GridData &startColumn(int value) { return wr.set<GridData &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The data in the grid, one entry per row, starting with the row in startRow. The values in RowData will correspond to columns starting at startColumn.
        GridData &rowData(const RowData &value) { return wr.append<GridData &, RowData>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // Metadata about the requested rows in the grid, starting with the row in startRow.
        GridData &rowMetadata(const DimensionProperties &value) { return wr.append<GridData &, DimensionProperties>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // Metadata about the requested columns in the grid, starting with the column in startColumn.
        GridData &columnMetadata(const DimensionProperties &value) { return wr.append<GridData &, DimensionProperties>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
    };

    /**
     * A rule that may or may not match, depending on the condition.
     */
    class BooleanRule : public BaseG4
    {

    public:
        BooleanRule() = default;

        // The condition of the rule. If the condition evaluates to true, the format is applied.
        BooleanRule &condition(const BooleanCondition &value) { return wr.set<BooleanRule &, BooleanCondition>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The format to apply. Conditional formatting can only apply a subset of formatting: bold, italic, strikethrough, foreground color and, background color.
        BooleanRule &format(const CellFormat &value) { return wr.set<BooleanRule &, CellFormat>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A single interpolation point on a gradient conditional format. These pin the gradient color scale according to the color, type and value chosen.
     */
    class InterpolationPoint : public BaseG4
    {
    public:
        InterpolationPoint() = default;

        // The color this interpolation point should use. If color is also set, this field takes precedence.
        InterpolationPoint &colorStyle(const ColorStyle &value) { return wr.set<InterpolationPoint &, ColorStyle>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // How the value should be interpreted.
        InterpolationPoint &type(Interpolation::InterpolationPointType value) { return wr.set<InterpolationPoint &, const char *>(*this, _InterpolationPointType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The value this interpolation point uses. May be a formula. Unused if type is MIN or MAX.
        InterpolationPoint &value(const String &value) { return wr.set<InterpolationPoint &, String>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * The editors of a protected range.
     */
    class Editors : public BaseG4
    {
    public:
        Editors() = default;

        // This value represents the item to add to an array.
        // The email addresses of users with edit access to the protected range.
        Editors &users(const String &value) { return wr.append<Editors &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // he email addresses of groups with edit access to the protected range.
        Editors &groups(const String &value) { return wr.append<Editors &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // True if anyone in the document's domain has edit access to the protected range. Domain protection is only supported on documents within a domain.
        Editors &domainUsersCanEdit(bool value) { return wr.set<Editors &, bool>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A protected range.
     */
    class ProtectedRange : public BaseG8
    {
    public:
        ProtectedRange() = default;

        // The range that is being protected. The range may be fully unbounded, in which case this is considered a protected sheet.
        ProtectedRange &range(const GridRange &value) { return wr.set<ProtectedRange &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The named range this protected range is backed by, if any.
        // When writing, only one of range or namedRangeId may be set.
        ProtectedRange &namedRangeId(const String &value) { return wr.set<ProtectedRange &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The description of this protected range.
        ProtectedRange &description(const String &value) { return wr.set<ProtectedRange &, String>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if this protected range will show a warning when editing. Warning-based protection means that every user can edit data in the protected range, except editing will prompt a warning asking the user to confirm the edit.
        // When writing: if this field is true, then editors are ignored. Additionally, if this field is changed from true to false and the editors field is not set (nor included in the field mask), then the editors will be set to all the editors in the document.
        ProtectedRange &warningOnly(bool value) { return wr.set<ProtectedRange &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        //  The list of unprotected ranges within a protected sheet. Unprotected ranges are only supported on protected sheets.
        ProtectedRange &unprotectedRanges(const GridRange &value) { return wr.append<ProtectedRange &, GridRange>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The users and groups with edit access to the protected range. This field is only visible to users with edit access to the protected range and the document. Editors are not supported with warningOnly protection.
        ProtectedRange &editors(const Editors &value) { return wr.set<ProtectedRange &, Editors>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }
    };

    /**
     * A rule that applies a gradient color scale format, based on the interpolation points listed. The format of a cell will vary based on its contents as compared to the values of the interpolation points.
     */
    class GradientRule : public BaseG4
    {
    public:
        GradientRule() = default;

        // The starting interpolation point.
        GradientRule &minpoint(const InterpolationPoint &value) { return wr.set<GradientRule &, InterpolationPoint>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // An optional midway interpolation point.
        GradientRule &midpoint(const InterpolationPoint &value) { return wr.set<GradientRule &, InterpolationPoint>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The final interpolation point.
        GradientRule &maxpoint(const InterpolationPoint &value) { return wr.set<GradientRule &, InterpolationPoint>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A rule describing a conditional format.
     */
    class ConditionalFormatRule : public BaseG4
    {
    public:
        ConditionalFormatRule() = default;

        // This value represents the item to add to an array.
        // The ranges that are formatted if the condition is true. All the ranges must be on the same grid.
        ConditionalFormatRule &ranges(const GridRange &value) { return wr.append<ConditionalFormatRule &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Union field rule
        // The formatting is either "on" or "off" according to the rule.
        ConditionalFormatRule &booleanRule(const BooleanRule &value) { return wr.set<ConditionalFormatRule &, BooleanRule>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field rule
        // The formatting will vary based on the gradients in the rule.
        ConditionalFormatRule &gradientRule(const GradientRule &value) { return wr.set<ConditionalFormatRule &, GradientRule>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * The default filter associated with a sheet.
     */
    class BasicFilter : public BaseG4
    {
    public:
        BasicFilter() = default;

        // The range the filter covers.
        BasicFilter &range(const GridRange &value) { return wr.set<BasicFilter &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The sort order per column. Later specifications are used when values are equal in the earlier specifications.
        BasicFilter &sortSpecs(const SortSpec &value) { return wr.append<BasicFilter &, SortSpec>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The filter criteria per column.
        // Both criteria and filterSpecs are populated in responses. If both fields are specified in an update request, this field takes precedence.
        BasicFilter &filterSpecs(const FilterSpec &value) { return wr.append<BasicFilter &, FilterSpec>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Properties referring a single dimension (either row or column). If both BandedRange.row_properties and BandedRange.column_properties are set, the fill colors are applied to cells according to the following rules:
     * headerColor and footerColor take priority over band colors.
     * firstBandColor takes priority over secondBandColor.
     * rowProperties takes priority over columnProperties.
     */
    class BandingProperties : public BaseG6
    {
    public:
        BandingProperties() = default;

        // The color of the first row or column. If this field is set, the first row or column is filled with this color and the colors alternate between firstBandColor and secondBandColor starting from the second row or column. Otherwise, the first row or column is filled with firstBandColor and the colors proceed to alternate as they normally would. If headerColor is also set, this field takes precedence.
        BandingProperties &headerColorStyle(const ColorStyle &value) { return wr.set<BandingProperties &, ColorStyle>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The first color that is alternating. (Required) If firstBandColor is also set, this field takes precedence.
        BandingProperties &firstBandColorStyle(const ColorStyle &value) { return wr.set<BandingProperties &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The second color that is alternating. (Required) If secondBandColor is also set, this field takes precedence.
        BandingProperties &secondBandColorStyle(const ColorStyle &value) { return wr.set<BandingProperties &, ColorStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The color of the last row or column. If this field is not set, the last row or column is filled with either firstBandColor or secondBandColor, depending on the color of the previous row or column. If footerColor is also set, this field takes precedence.
        BandingProperties &footerColorStyle(const ColorStyle &value) { return wr.set<BandingProperties &, ColorStyle>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A banded (alternating colors) range in a sheet.
     */
    class BandedRange : public BaseG6
    {
    public:
        BandedRange() = default;

        // The ID of the banded range.
        BandedRange &bandedRangeId(int value) { return wr.set<BandedRange &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The range over which these properties are applied.
        BandedRange &range(const GridRange &value) { return wr.set<BandedRange &, GridRange>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Properties for row bands. These properties are applied on a row-by-row basis throughout all the rows in the range. At least one of rowProperties or columnProperties must be specified.
        BandedRange &rowProperties(const BandingProperties &value) { return wr.set<BandedRange &, BandingProperties>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Properties for column bands. These properties are applied on a column- by-column basis throughout all the columns in the range. At least one of rowProperties or columnProperties must be specified.
        BandedRange &columnProperties(const BandingProperties &value) { return wr.set<BandedRange &, BandingProperties>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A group over an interval of rows or columns on a sheet, which can contain or be contained within other groups. A group can be collapsed or expanded as a unit on the sheet.
     */
    class DimensionGroup : public BaseG4
    {
    public:
        DimensionGroup() = default;

        // The range over which this group exists.
        DimensionGroup &range(const DimensionRange &value) { return wr.set<DimensionGroup &, DimensionRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The depth of the group, representing how many groups have a range that wholly contains the range of this group.
        DimensionGroup &dept(int value) { return wr.set<DimensionGroup &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // This field is true if this group is collapsed. A collapsed group remains collapsed if an overlapping group at a shallower depth is expanded.
        // A true value does not imply that all dimensions within the group are hidden, since a dimension's visibility can change independently from this group property. However, when this property is updated, all dimensions within it are set to hidden if this field is true, or set to visible if this field is false.
        DimensionGroup &collapsed(bool value) { return wr.set<DimensionGroup &, bool>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A filter view.
     */
    class FilterView : public BaseG8
    {
    public:
        FilterView() = default;

        // The ID of the filter view.
        FilterView &filterViewId(int value) { return wr.set<FilterView &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The name of the filter view.
        FilterView &title(const String &value) { return wr.set<FilterView &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The range this filter view covers.
        // When writing, only one of range or namedRangeId may be set.
        FilterView &range(const GridRange &value) { return wr.set<FilterView &, GridRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The named range this filter view is backed by, if any.
        // When writing, only one of range or namedRangeId may be set.
        FilterView &namedRangeId(const String &value) { return wr.set<FilterView &, String>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The sort order per column. Later specifications are used when values are equal in the earlier specifications.
        FilterView &sortSpecs(const SortSpec &value) { return wr.append<FilterView &, SortSpec>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The filter criteria for showing/hiding values per column.
        // Both criteria and filterSpecs are populated in responses. If both fields are specified in an update request, this field takes precedence.
        FilterView &filterSpecs(const FilterSpec &value) { return wr.append<FilterView &, FilterSpec>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }
    };

    /**
     * The specifications of a slicer.
     */
    class SlicerSpec : public BaseG12
    {
    public:
        SlicerSpec() = default;

        // The data range of the slicer.
        SlicerSpec &dataRange(const GridRange &value) { return wr.set<SlicerSpec &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The filtering criteria of the slicer.
        SlicerSpec &filterCriteria(const FilterCriteria &value) { return wr.set<SlicerSpec &, FilterCriteria>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The zero-based column index in the data table on which the filter is applied to.
        SlicerSpec &columnIndex(int value) { return wr.set<SlicerSpec &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if the filter should apply to pivot tables. If not set, default to True.
        SlicerSpec &applyToPivotTables(bool value) { return wr.set<SlicerSpec &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The title of the slicer.
        SlicerSpec &title(const String &value) { return wr.set<SlicerSpec &, String>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The text format of title in the slicer. The link field is not supported.
        SlicerSpec &textFormat(const TextFormat &value) { return wr.set<SlicerSpec &, TextFormat>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The background color of the slicer. If backgroundColor is also set, this field takes precedence.
        SlicerSpec &backgroundColorStyle(const ColorStyle &value) { return wr.set<SlicerSpec &, ColorStyle>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // The horizontal alignment of title in the slicer. If unspecified, defaults to LEFT
        SlicerSpec &horizontalAlignment(TextAlignment::HorizontalAlign value) { return wr.set<SlicerSpec &, char *>(*this, TextAlignment::_HorizontalAlign[value].text, buf, bufSize, 8, FPSTR(__func__)); }
    };

    /**
     * A slicer in a sheet.
     */
    class Slicer : public BaseG4
    {
    public:
        Slicer() = default;

        // The ID of the slicer.
        Slicer &slicerId(int value) { return wr.set<Slicer &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The specification of the slicer.
        Slicer &spec(const SlicerSpec &value) { return wr.set<Slicer &, SlicerSpec>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The position of the slicer. Note that slicer can be positioned only on existing sheet. Also, width and height of slicer can be automatically adjusted to keep it within permitted limits.
        Slicer &position(const EmbeddedObjectPosition &value) { return wr.set<Slicer &, EmbeddedObjectPosition>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A sheet in a spreadsheet.
     */
    class Sheet : public BaseG16
    {

    public:
        Sheet() = default;

        // The properties of the sheet.
        Sheet &properties(const SheetProperties &value) { return wr.set<Sheet &, SheetProperties>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Data in the grid, if this is a grid sheet.
        // The number of GridData objects returned is dependent on the number of ranges requested on this sheet. For example, if this is representing Sheet1, and the spreadsheet was requested with ranges Sheet1!A1:C10 and Sheet1!D15:E20, then the first GridData will have a startRow / startColumn of 0, while the second one will have startRow 14 (zero-based row 15), and startColumn 3 (zero-based column D).
        Sheet &data(const GridData &value) { return wr.set<Sheet &, GridData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The ranges that are merged together.
        Sheet &merges(const GridRange &value) { return wr.append<Sheet &, GridRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The conditional format rules in this sheet.
        Sheet &conditionalFormats(const ConditionalFormatRule &value) { return wr.append<Sheet &, ConditionalFormatRule>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The filter views in this sheet.
        Sheet &filterViews(const FilterView &value) { return wr.set<Sheet &, FilterView>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The protected ranges in this sheet.
        Sheet &protectedRanges(const ProtectedRange &value) { return wr.append<Sheet &, ProtectedRange>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The filter on this sheet, if any.
        Sheet &basicFilter(const BasicFilter &value) { return wr.set<Sheet &, BasicFilter>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The specifications of every chart on this sheet.
        Sheet &charts(const SheetProperties &value) { return wr.append<Sheet &, SheetProperties>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The banded (alternating colors) ranges on this sheet.
        Sheet &bandedRanges(const BandedRange &value) { return wr.append<Sheet &, BandedRange>(*this, value, buf, bufSize, 9, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The developer metadata associated with a sheet.
        Sheet &developerMetadata(const DeveloperMetadata &value) { return wr.append<Sheet &, DeveloperMetadata>(*this, value, buf, bufSize, 10, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // All row groups on this sheet, ordered by increasing range start index, then by group depth.
        Sheet &rowGroups(const DimensionGroup &value) { return wr.append<Sheet &, DimensionGroup>(*this, value, buf, bufSize, 11, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // All column groups on this sheet, ordered by increasing range start index, then by group depth.
        Sheet &columnGroups(const DimensionGroup &value) { return wr.append<Sheet &, DimensionGroup>(*this, value, buf, bufSize, 12, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The slicers on this sheet.
        Sheet &slicers(const Slicer &value) { return wr.append<Sheet &, Slicer>(*this, value, buf, bufSize, 13, FPSTR(__func__)); }
    };

}
#endif
