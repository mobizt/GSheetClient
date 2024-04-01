#ifndef SHEETS_H
#define SHEETS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/Charts.h"
#include "./spreadsheets/requests/Dimension.h"
#include "./spreadsheets/requests/Cell.h"

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

    // The kind of sheet.
    enum SheetType
    {
        SHEET_TYPE_UNSPECIFIED, //	Default value, do not use.
        GRID,                   //	The sheet is a grid.
        OBJECT,                 //	The sheet has no grid and instead has an object like a chart or image.
        DATA_SOURCE             //	The sheet connects with an external DataSource and shows the preview of data.
    };

    // The kind of interpolation point.
    enum InterpolationPointType
    {
        INTERPOLATION_POINT_TYPE_UNSPECIFIED, //	The default value, do not use.
        MIN,                                  //	The interpolation point uses the minimum value in the cells over the range of the conditional format.
        MAX,                                  //	The interpolation point uses the maximum value in the cells over the range of the conditional format.
        NUMBER,                               //	The interpolation point uses exactly the value in InterpolationPoint.value.
        PERCENT,                              //	The interpolation point is the given percentage over all the cells in the range of the conditional format. This is equivalent to NUMBER if the value was: =(MAX(FLATTEN(range)) * (value / 100)) + (MIN(FLATTEN(range)) * (1 - (value / 100))) (where errors in the range are ignored when flattening).
        PERCENTILE                            //	The interpolation point is the given percentile over all the cells in the range of the conditional format. This is equivalent to NUMBER if the value was: =PERCENTILE(FLATTEN(range), value / 100) (where errors in the range are ignored when flattening).
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
     * Data about each cell in a row.
     */
    class RowData : public Printable
    {
    private:
        size_t bufSize = 2;
        String buf[2];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        RowData &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        RowData() {}
        // The values in the row, one per column.
        RowData &addValues(const CellData &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("values"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Data in the grid, as well as metadata about the dimensions.
     */
    class GridData : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GridData &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GridData() {}
        // The first row this GridData refers to, zero-based.
        GridData &startRow(int value) { return setObject(buf[1], "startRow", String(value), false, true); }
        // The first column this GridData refers to, zero-based.
        GridData &startColumn(int value) { return setObject(buf[2], "startColumn", String(value), false, true); }
        // The data in the grid, one entry per row, starting with the row in startRow. The values in RowData will correspond to columns starting at startColumn.
        GridData &addRowData(const RowData &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("rowData"), value.c_str(), false);
            return *this;
        }
        // Metadata about the requested rows in the grid, starting with the row in startRow.
        GridData &addRowMetadata(const DimensionProperties &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("rowMetadata"), value.c_str(), false);
            return *this;
        }
        // Metadata about the requested columns in the grid, starting with the column in startColumn.
        GridData &addColumnMetadata(const DimensionProperties &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("columnMetadata"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A rule that may or may not match, depending on the condition.
     */
    class BooleanRule : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BooleanRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BooleanRule() {}
        // The condition of the rule. If the condition evaluates to true, the format is applied.
        BooleanRule &condition(const BooleanCondition &value) { return setObject(buf[1], "condition", value.c_str(), false, true); }
        // The format to apply. Conditional formatting can only apply a subset of formatting: bold, italic, strikethrough, foreground color and, background color.
        BooleanRule &format(const CellFormat &value) { return setObject(buf[2], "format", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A single interpolation point on a gradient conditional format. These pin the gradient color scale according to the color, type and value chosen.
     */
    class InterpolationPoint : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        InterpolationPoint &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        InterpolationPoint() {}
        // The color this interpolation point should use. If color is also set, this field takes precedence.
        InterpolationPoint &colorStyle(const ColorStyle &value) { return setObject(buf[1], "colorStyle", value.c_str(), false, true); }
        // How the value should be interpreted.
        InterpolationPoint &type(InterpolationPointType value)
        {
            if (value == INTERPOLATION_POINT_TYPE_UNSPECIFIED)
                return setObject(buf[2], "type", "INTERPOLATION_POINT_TYPE_UNSPECIFIED", true, true);
            else if (value == MIN)
                return setObject(buf[2], "type", "MIN", true, true);
            else if (value == MAX)
                return setObject(buf[2], "type", "MAX", true, true);
            else if (value == NUMBER)
                return setObject(buf[2], "type", "NUMBER", true, true);
            else if (value == PERCENT)
                return setObject(buf[2], "type", "PERCENT", true, true);
            else if (value == PERCENTILE)
                return setObject(buf[2], "type", "PERCENTILE", true, true);
            return *this;
        }
        // The value this interpolation point uses. May be a formula. Unused if type is MIN or MAX.
        InterpolationPoint &value(const String &value) { return setObject(buf[3], "value", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The editors of a protected range.
     */
    class Editors : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Editors &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Editors() {}
        // The email addresses of users with edit access to the protected range.
        Editors &addUsers(const String &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("users"), value.c_str(), false);
            return *this;
        }
        // he email addresses of groups with edit access to the protected range.
        Editors &addGroups(const String &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("groups"), value.c_str(), false);
            return *this;
        }
        // True if anyone in the document's domain has edit access to the protected range. Domain protection is only supported on documents within a domain.
        Editors &domainUsersCanEdit(bool value) { return setObject(buf[3], "domainUsersCanEdit", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A protected range.
     */
    class ProtectedRange : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ProtectedRange &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ProtectedRange() {}
        // The range that is being protected. The range may be fully unbounded, in which case this is considered a protected sheet.
        ProtectedRange &range(const GridRange &value) { return setObject(buf[1], "range", value.c_str(), false, true); }
        // The named range this protected range is backed by, if any.
        // When writing, only one of range or namedRangeId may be set.
        ProtectedRange &namedRangeId(const String &value) { return setObject(buf[2], "namedRangeId", value, true, true); }
        // The description of this protected range.
        ProtectedRange &description(const String &value) { return setObject(buf[3], "description", value, true, true); }
        // True if this protected range will show a warning when editing. Warning-based protection means that every user can edit data in the protected range, except editing will prompt a warning asking the user to confirm the edit.
        // When writing: if this field is true, then editors are ignored. Additionally, if this field is changed from true to false and the editors field is not set (nor included in the field mask), then the editors will be set to all the editors in the document.
        ProtectedRange &warningOnly(bool value) { return setObject(buf[4], "warningOnly", owriter.getBoolStr(value), false, true); }
        // The list of unprotected ranges within a protected sheet. Unprotected ranges are only supported on protected sheets.
        ProtectedRange &addUnprotectedRanges(const GridRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("unprotectedRanges"), value.c_str(), false);
            return *this;
        }
        // The users and groups with edit access to the protected range. This field is only visible to users with edit access to the protected range and the document. Editors are not supported with warningOnly protection.
        ProtectedRange &editors(const Editors &value) { return setObject(buf[6], "editors", value.c_str(), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A rule that applies a gradient color scale format, based on the interpolation points listed. The format of a cell will vary based on its contents as compared to the values of the interpolation points.
     */
    class GradientRule : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GradientRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GradientRule() {}
        // The starting interpolation point.
        GradientRule &minpoint(const InterpolationPoint &value) { return setObject(buf[1], "minpoint", value.c_str(), false, true); }
        // An optional midway interpolation point.
        GradientRule &midpoint(const InterpolationPoint &value) { return setObject(buf[2], "midpoint", value.c_str(), false, true); }
        // The final interpolation point.
        GradientRule &maxpoint(const InterpolationPoint &value) { return setObject(buf[3], "maxpoint", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A rule describing a conditional format.
     */
    class ConditionalFormatRule : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ConditionalFormatRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ConditionalFormatRule() {}
        // The ranges that are formatted if the condition is true. All the ranges must be on the same grid.
        ConditionalFormatRule &addRanges(const GridRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("ranges"), value.c_str(), false);
            return *this;
        }
        // Union field rule
        // The formatting is either "on" or "off" according to the rule.
        ConditionalFormatRule &booleanRule(const BooleanRule &value) { return setObject(buf[2], "booleanRule", value.c_str(), false, true); }
        // Union field rule
        // The formatting will vary based on the gradients in the rule.
        ConditionalFormatRule &gradientRule(const GradientRule &value) { return setObject(buf[2], "gradientRule", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The default filter associated with a sheet.
     */
    class BasicFilter : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BasicFilter &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BasicFilter() {}
        // The range the filter covers.
        BasicFilter &range(const GridRange &value) { return setObject(buf[1], "range", value.c_str(), false, true); }
        // The sort order per column. Later specifications are used when values are equal in the earlier specifications.
        BasicFilter &addSortSpecs(const SortSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("sortSpecs"), value.c_str(), false);
            return *this;
        }
        // The filter criteria per column.
        // Both criteria and filterSpecs are populated in responses. If both fields are specified in an update request, this field takes precedence.
        BasicFilter &addFilterSpecs(const FilterSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("filterSpecs"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Properties referring a single dimension (either row or column). If both BandedRange.row_properties and BandedRange.column_properties are set, the fill colors are applied to cells according to the following rules:
     * headerColor and footerColor take priority over band colors.
     * firstBandColor takes priority over secondBandColor.
     * rowProperties takes priority over columnProperties.
     */
    class BandingProperties : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BandingProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BandingProperties() {}
        // The color of the first row or column. If this field is set, the first row or column is filled with this color and the colors alternate between firstBandColor and secondBandColor starting from the second row or column. Otherwise, the first row or column is filled with firstBandColor and the colors proceed to alternate as they normally would. If headerColor is also set, this field takes precedence.
        BandingProperties &headerColorStyle(const ColorStyle &value) { return setObject(buf[1], "headerColorStyle", value.c_str(), false, true); }
        // The first color that is alternating. (Required) If firstBandColor is also set, this field takes precedence.
        BandingProperties &firstBandColorStyle(const ColorStyle &value) { return setObject(buf[2], "firstBandColorStyle", value.c_str(), false, true); }
        // The second color that is alternating. (Required) If secondBandColor is also set, this field takes precedence.
        BandingProperties &secondBandColorStyle(const ColorStyle &value) { return setObject(buf[3], "secondBandColorStyle", value.c_str(), false, true); }
        // The color of the last row or column. If this field is not set, the last row or column is filled with either firstBandColor or secondBandColor, depending on the color of the previous row or column. If footerColor is also set, this field takes precedence.
        BandingProperties &footerColorStyle(const ColorStyle &value) { return setObject(buf[4], "footerColorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A banded (alternating colors) range in a sheet.
     */
    class BandedRange : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BandedRange &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BandedRange() {}
        // The ID of the banded range.
        BandedRange &bandedRangeId(int value) { return setObject(buf[1], "bandedRangeId", String(value), false, true); }
        // The range over which these properties are applied.
        BandedRange &range(const GridRange &value) { return setObject(buf[2], "range", value.c_str(), false, true); }
        // Properties for row bands. These properties are applied on a row-by-row basis throughout all the rows in the range. At least one of rowProperties or columnProperties must be specified.
        BandedRange &rowProperties(const BandingProperties &value) { return setObject(buf[3], "rowProperties", value.c_str(), false, true); }
        // Properties for column bands. These properties are applied on a column- by-column basis throughout all the columns in the range. At least one of rowProperties or columnProperties must be specified.
        BandedRange &columnProperties(const BandingProperties &value) { return setObject(buf[4], "columnProperties", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A group over an interval of rows or columns on a sheet, which can contain or be contained within other groups. A group can be collapsed or expanded as a unit on the sheet.
     */
    class DimensionGroup : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DimensionGroup &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DimensionGroup() {}
        // The range over which this group exists.
        DimensionGroup &range(const DimensionRange &value) { return setObject(buf[1], "range", value.c_str(), false, true); }
        // The depth of the group, representing how many groups have a range that wholly contains the range of this group.
        DimensionGroup &dept(int value) { return setObject(buf[2], "dept", String(value), false, true); }
        // This field is true if this group is collapsed. A collapsed group remains collapsed if an overlapping group at a shallower depth is expanded.
        // A true value does not imply that all dimensions within the group are hidden, since a dimension's visibility can change independently from this group property. However, when this property is updated, all dimensions within it are set to hidden if this field is true, or set to visible if this field is false.
        DimensionGroup &collapsed(bool value) { return setObject(buf[3], "collapsed", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A filter view.
     */
    class FilterView : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        FilterView &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        FilterView() {}
        // The ID of the filter view.
        FilterView &filterViewId(int value) { return setObject(buf[1], "filterViewId", String(value), false, true); }
        // The name of the filter view.
        FilterView &title(const String &value) { return setObject(buf[2], "title", value, true, true); }
        // The range this filter view covers.
        // When writing, only one of range or namedRangeId may be set.
        FilterView &range(const GridRange &value) { return setObject(buf[3], "range", value.c_str(), false, true); }
        // The named range this filter view is backed by, if any.
        // When writing, only one of range or namedRangeId may be set.
        FilterView &namedRangeId(const String &value) { return setObject(buf[4], "namedRangeId", value, true, true); }
        // The sort order per column. Later specifications are used when values are equal in the earlier specifications.
        FilterView &addSortSpecs(const SortSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("sortSpecs"), value.c_str(), false);
            return *this;
        }
        // The filter criteria for showing/hiding values per column.
        // Both criteria and filterSpecs are populated in responses. If both fields are specified in an update request, this field takes precedence.
        FilterView &addFilterSpecs(const FilterSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[6], FPSTR("filterSpecs"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The specifications of a slicer.
     */
    class SlicerSpec : public Printable
    {
    private:
        size_t bufSize = 9;
        String buf[9];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SlicerSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SlicerSpec() {}
        // The data range of the slicer.
        SlicerSpec &dataRange(const GridRange &value) { return setObject(buf[1], "dataRange", value.c_str(), false, true); }
        // The filtering criteria of the slicer.
        SlicerSpec &filterCriteria(const FilterCriteria &value) { return setObject(buf[2], "filterCriteria", value.c_str(), false, true); }
        // The zero-based column index in the data table on which the filter is applied to.
        SlicerSpec &columnIndex(int value) { return setObject(buf[3], "columnIndex", String(value), false, true); }
        // True if the filter should apply to pivot tables. If not set, default to True.
        SlicerSpec &applyToPivotTables(bool value) { return setObject(buf[4], "applyToPivotTables", owriter.getBoolStr(value), false, true); }
        // The title of the slicer.
        SlicerSpec &title(const String &value) { return setObject(buf[5], "title", value, true, true); }
        // The text format of title in the slicer. The link field is not supported.
        SlicerSpec &textFormat(const TextFormat &value) { return setObject(buf[6], "textFormat", value.c_str(), false, true); }
        // The background color of the slicer. If backgroundColor is also set, this field takes precedence.
        SlicerSpec &backgroundColorStyle(const ColorStyle &value) { return setObject(buf[7], "backgroundColorStyle", value.c_str(), false, true); }
        // The horizontal alignment of title in the slicer. If unspecified, defaults to LEFT
        SlicerSpec &horizontalAlignment(HorizontalAlign value)
        {
            clear();
            if (value == HORIZONTAL_ALIGN_UNSPECIFIED)
                return setObject(buf[8], "horizontalAlignment", "HORIZONTAL_ALIGN_UNSPECIFIED", true, true);
            else if (value == LEFT)
                return setObject(buf[8], "horizontalAlignment", "LEFT", true, true);
            else if (value == CENTER)
                return setObject(buf[8], "horizontalAlignment", "CENTER", true, true);
            else if (value == RIGHT)
                return setObject(buf[8], "horizontalAlignment", "RIGHT", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A slicer in a sheet.
     */
    class Slicer : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Slicer &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Slicer() {}
        // The ID of the slicer.
        Slicer &slicerId(int value) { return setObject(buf[1], "slicerId", String(value), false, true); }
        // The specification of the slicer.
        Slicer &spec(const SlicerSpec &value) { return setObject(buf[2], "spec", value.c_str(), false, true); }
        // The position of the slicer. Note that slicer can be positioned only on existing sheet. Also, width and height of slicer can be automatically adjusted to keep it within permitted limits.
        Slicer &position(const EmbeddedObjectPosition &value) { return setObject(buf[3], "position", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A sheet in a spreadsheet.
     */
    class Sheet : public Printable
    {
    private:
        size_t bufSize = 14;
        String buf[14];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Sheet &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Sheet() {}
        // The properties of the sheet.
        Sheet &properties(const SheetProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // Data in the grid, if this is a grid sheet.
        // The number of GridData objects returned is dependent on the number of ranges requested on this sheet. For example, if this is representing Sheet1, and the spreadsheet was requested with ranges Sheet1!A1:C10 and Sheet1!D15:E20, then the first GridData will have a startRow / startColumn of 0, while the second one will have startRow 14 (zero-based row 15), and startColumn 3 (zero-based column D).
        Sheet &data(const GridData &value) { return setObject(buf[2], "data", value.c_str(), false, true); }
        // The ranges that are merged together.
        Sheet &addMerges(const GridRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("merges"), value.c_str(), false);
            return *this;
        }
        // The conditional format rules in this sheet.
        Sheet &addConditionalFormats(const ConditionalFormatRule &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("conditionalFormats"), value.c_str(), false);
            return *this;
        }
        // The filter views in this sheet.
        Sheet &addFilterViews(const FilterView &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("filterViews"), value.c_str(), false);
            return *this;
        }
        // The protected ranges in this sheet.
        Sheet &addProtectedRanges(const ProtectedRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[6], FPSTR("protectedRanges"), value.c_str(), false);
            return *this;
        }
        // The filter on this sheet, if any.
        Sheet &basicFilter(const BasicFilter &value) { return setObject(buf[7], "basicFilter", value.c_str(), false, true); }
        // The specifications of every chart on this sheet.
        Sheet &addCharts(const SheetProperties &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[8], FPSTR("charts"), value.c_str(), false);
            return *this;
        }
        // The banded (alternating colors) ranges on this sheet.
        Sheet &addBandedRanges(const BandedRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[9], FPSTR("bandedRanges"), value.c_str(), false);
            return *this;
        }
        // The developer metadata associated with a sheet.
        Sheet &addDeveloperMetadata(const DeveloperMetadata &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[10], FPSTR("developerMetadata"), value.c_str(), false);
            return *this;
        }
        // All row groups on this sheet, ordered by increasing range start index, then by group depth.
        Sheet &addRowGroups(const DimensionGroup &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[11], FPSTR("rowGroups"), value.c_str(), false);
            return *this;
        }
        // All column groups on this sheet, ordered by increasing range start index, then by group depth.
        Sheet &addColumnGroups(const DimensionGroup &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[12], FPSTR("columnGroups"), value.c_str(), false);
            return *this;
        }
        // The slicers on this sheet.
        Sheet &addSlicers(const Slicer &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[13], FPSTR("slicers"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}
#endif
