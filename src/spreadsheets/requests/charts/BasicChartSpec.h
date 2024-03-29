#ifndef BASIC_CHART_SPEC_H
#define BASIC_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"
#include "./spreadsheets/requests/DataSourceTable.h"
#include "./spreadsheets/requests/charts/DataLabel.h"
#include "./spreadsheets/requests/charts/LineStyle.h"

/**
 * BASIC CHART CLASS DEPENDENCIES
 *
 * BasicChartSpec       (enum) BasicChartType
 *                      (enum) BasicChartLegendPosition
 *
 *                      BasicChartAxis                          (enum) BasicChartAxisPosition
 *                                                              TextFormat
 *                                                              TextPosition
 *                                                              ChartAxisViewWindowOptions              (enum) ViewWindowMode
 *
 *                      BasicChartDomain                        ChartData*
 *
 *                      BasicChartSeries                        ChartData*
 *                                                              (enum) BasicChartAxisPosition
 *                                                              (enum) BasicChartType
 *                                                              LineStyle                               (enum) LineDashType
 *
 *                                                              DataLabel*
 *                                                              ColorStyle*
 *                                                              PointStyle                              (enum) PointShape
 *
 *                                                              BasicSeriesDataPointStyleOverride       ColorStyle*
 *                                                                                                      PointStyle                      (enum) PointShape
 *                      (enum) BasicChartStackedType
 *                      (enum) BasicChartCompareMode
 *                      DataLabel*
 *
 * See Theme.h, ChartData.h, DataLabel.h
 */
namespace GSHEET
{
    // How the chart should be visualized.
    enum BasicChartType
    {
        BASIC_CHART_TYPE_UNSPECIFIED, //	Default value, do not use.
        BAR,                          //	A bar chart.
        LINE,                         //	A line chart.
        AREA,                         //	An area chart.
        COLUMN,                       //	A column chart.
        SCATTER,                      //	A scatter chart.
        COMBO,                        //	A combo chart.
        STEPPED_AREA                  //	A stepped area chart.
    };

    // Where the legend of the chart should be positioned.
    enum BasicChartLegendPosition
    {
        BASIC_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
        BOTTOM_LEGEND,                           //	The legend is rendered on the bottom of the chart.
        LEFT_LEGEND,                             //	The legend is rendered on the left of the chart.
        RIGHT_LEGEND,                            //	The legend is rendered on the right of the chart.
        TOP_LEGEND,                              //	The legend is rendered on the top of the chart.
        NO_LEGEND                                //	No legend is rendered.
    };

    // The position of a chart axis.
    enum BasicChartAxisPosition
    {
        BASIC_CHART_AXIS_POSITION_UNSPECIFIED, //	Default value, do not use.
        BOTTOM_AXIS,                           //	The axis rendered at the bottom of a chart. For most charts, this is the standard major axis. For bar charts, this is a minor axis.
        LEFT_AXIS,                             //	The axis rendered at the left of a chart. For most charts, this is a minor axis. For bar charts, this is the standard major axis.
        RIGHT_AXIS                             //	The axis rendered at the right of a chart. For most charts, this is a minor axis. For bar charts, this is an unusual major axis.
    };

    // The view window's mode. It defines how to treat the min and max of the view window.
    enum ViewWindowMode
    {
        DEFAULT_VIEW_WINDOW_MODE,     //	The default view window mode used in the Sheets editor for this chart type. In most cases, if set, the default mode is equivalent to PRETTY.
        VIEW_WINDOW_MODE_UNSUPPORTED, //	Do not use. Represents that the currently set mode is not supported by the API.
        EXPLICIT,                     //	Follows the min and max exactly if specified. If a value is unspecified, it will fall back to the PRETTY value.
        PRETTY                        //	Chooses a min and max that make the chart look good. Both min and max are ignored in this mode.
    };

    // The shape of a point.
    enum PointShape
    {
        POINT_SHAPE_UNSPECIFIED, //	Default value.
        CIRCLE,                  //	A circle shape.
        DIAMOND,                 //	A diamond shape.
        HEXAGON,                 //	A hexagon shape.
        PENTAGON,                //	A pentagon shape.
        SQUARE,                  //	A square shape.
        STAR,                    //	A star shape.
        TRIANGLE,                //	A triangle shape.
        X_MARK                   //	An x-mark shape.
    };

    // When charts are stacked, range (vertical axis) values are rendered on top of one another rather than from the horizontal axis. For example, the two values 20 and 80 would be drawn from 0, with 80 being 80 units away from the horizontal axis. If they were stacked, 80 would be rendered from 20, putting it 100 units away from the horizontal axis.
    enum BasicChartStackedType
    {
        BASIC_CHART_STACKED_TYPE_UNSPECIFIED, //	Default value, do not use.
        NOT_STACKED,                          //	Series are not stacked.
        STACKED,                              //	Series values are stacked, each value is rendered vertically beginning from the top of the value below it.
        PERCENT_STACKED                       //	Vertical stacks are stretched to reach the top of the chart, with values laid out as percentages of each other.
    };

    // The compare mode type, which describes the behavior of tooltips and data highlighting when hovering on data and chart area.
    enum BasicChartCompareMode
    {
        BASIC_CHART_COMPARE_MODE_UNSPECIFIED, //	Default value, do not use.
        DATUM,                                //	Only the focused data element is highlighted and shown in the tooltip.
        CATEGORY                              //	All data elements with the same category (e.g., domain value) are highlighted and shown in the tooltip.
    };

    /**
     * The options that define a "view window" for a chart (such as the visible values in an axis).
     */
    class ChartAxisViewWindowOptions : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ChartAxisViewWindowOptions &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ChartAxisViewWindowOptions() {}
        ChartAxisViewWindowOptions &viewWindowMin(double value) { return setObject(buf[1], "viewWindowMin", String(value), false, true); }
        ChartAxisViewWindowOptions &viewWindowMax(double value) { return setObject(buf[2], "viewWindowMax", String(value), false, true); }
        ChartAxisViewWindowOptions &viewWindowMode(ViewWindowMode value)
        {
            if (value == DEFAULT_VIEW_WINDOW_MODE)
                return setObject(buf[3], "viewWindowMode", "DEFAULT_VIEW_WINDOW_MODE", true, true);
            else if (value == VIEW_WINDOW_MODE_UNSUPPORTED)
                return setObject(buf[3], "viewWindowMode", "VIEW_WINDOW_MODE_UNSUPPORTED", true, true);
            else if (value == EXPLICIT)
                return setObject(buf[3], "viewWindowMode", "EXPLICIT", true, true);
            else if (value == PRETTY)
                return setObject(buf[3], "viewWindowMode", "PRETTY", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * An axis of the chart. A chart may not have more than one axis per axis position.
     */
    class BasicChartAxis : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BasicChartAxis &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BasicChartAxis() {}
        // The position of this axis.
        BasicChartAxis &position(BasicChartAxisPosition value)
        {
            if (value == BASIC_CHART_AXIS_POSITION_UNSPECIFIED)
                return setObject(buf[1], "position", "BASIC_CHART_AXIS_POSITION_UNSPECIFIED", true, true);
            else if (value == BOTTOM_AXIS)
                return setObject(buf[1], "position", "BOTTOM_AXIS", true, true);
            else if (value == LEFT_AXIS)
                return setObject(buf[1], "position", "LEFT_AXIS", true, true);
            else if (value == RIGHT_AXIS)
                return setObject(buf[1], "position", "RIGHT_AXIS", true, true);
            return *this;
        }
        // The title of this axis. If set, this overrides any title inferred from headers of the data.
        BasicChartAxis &title(const String &value) { return setObject(buf[2], "title", value, true, true); }
        // The format of the title. Only valid if the axis is not associated with the domain. The link field is not supported.
        BasicChartAxis &format(const TextFormat &value) { return setObject(buf[3], "format", value.c_str(), false, true); }
        // The axis title text position.
        BasicChartAxis &titleTextPosition(const TextPosition &value) { return setObject(buf[4], "titleTextPosition", value.c_str(), false, true); }
        // The view window options for this axis.
        BasicChartAxis &viewWindowOptions(const ChartAxisViewWindowOptions &value) { return setObject(buf[5], "viewWindowOptions", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The domain of a chart. For example, if charting stock prices over time, this would be the date.
     */
    class BasicChartDomain : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BasicChartDomain &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BasicChartDomain() {}
        BasicChartDomain &domain(const ChartData &value) { return setObject(buf[1], "domain", value.c_str(), false, true); }
        BasicChartDomain &reversed(bool value) { return setObject(buf[2], "format", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    
    /**
     * The style of a point on the chart.
     */
    class PointStyle : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PointStyle &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PointStyle() {}
        // The point size. If empty, a default size is used.
        PointStyle &size(double value) { return setObject(buf[1], "size", String(value), false, true); }
        // The point shape. If empty or unspecified, a default shape is used.
        PointStyle &shape(PointShape value)
        {
            if (value == POINT_SHAPE_UNSPECIFIED)
                return setObject(buf[2], "shape", "POINT_SHAPE_UNSPECIFIED", true, true);
            else if (value == CIRCLE)
                return setObject(buf[2], "shape", "CIRCLE", true, true);
            else if (value == DIAMOND)
                return setObject(buf[2], "shape", "DIAMOND", true, true);
            else if (value == HEXAGON)
                return setObject(buf[2], "shape", "HEXAGON", true, true);
            else if (value == PENTAGON)
                return setObject(buf[2], "shape", "PENTAGON", true, true);
            else if (value == SQUARE)
                return setObject(buf[2], "shape", "SQUARE", true, true);
            else if (value == STAR)
                return setObject(buf[2], "shape", "STAR", true, true);
            else if (value == TRIANGLE)
                return setObject(buf[2], "shape", "TRIANGLE", true, true);
            else if (value == X_MARK)
                return setObject(buf[2], "shape", "X_MARK", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Style override settings for a single series data point.
     */
    class BasicSeriesDataPointStyleOverride : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BasicSeriesDataPointStyleOverride &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BasicSeriesDataPointStyleOverride() {}
        // The zero-based index of the series data point.
        BasicSeriesDataPointStyleOverride &index(int value) { return setObject(buf[1], "index", String(value), false, true); }
        // Color of the series data point. If empty, the series default is used. If color is also set, this field takes precedence.
        BasicSeriesDataPointStyleOverride &colorStyle(const ColorStyle &value) { return setObject(buf[2], "colorStyle", value.c_str(), false, true); }
        // Point style of the series data point. Valid only if the chartType is AREA, LINE, or SCATTER. COMBO charts are also supported if the series chart type is AREA, LINE, or SCATTER. If empty, the series default is used.
        BasicSeriesDataPointStyleOverride &pointStyle(const PointStyle &value) { return setObject(buf[3], "pointStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A single series of data in a chart. For example, if charting stock prices over time, multiple series may exist, one for the "Open Price", "High Price", "Low Price" and "Close Price".
     */
    class BasicChartSeries : public Printable
    {
    private:
        size_t bufSize = 9;
        String buf[9];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BasicChartSeries &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BasicChartSeries() {}
        // The data being visualized in this chart series.
        BasicChartSeries &series(const ChartData &value) { return setObject(buf[1], "series", value.c_str(), false, true); }
        // The minor axis that will specify the range of values for this series. For example, if charting stocks over time, the "Volume" series may want to be pinned to the right with the prices pinned to the left, because the scale of trading volume is different than the scale of prices. It is an error to specify an axis that isn't a valid minor axis for the chart's type.
        BasicChartSeries &targetAxis(BasicChartAxisPosition value)
        {
            if (value == BASIC_CHART_AXIS_POSITION_UNSPECIFIED)
                return setObject(buf[2], "targetAxis", "BASIC_CHART_AXIS_POSITION_UNSPECIFIED", true, true);
            else if (value == BOTTOM_AXIS)
                return setObject(buf[2], "targetAxis", "BOTTOM_AXIS", true, true);
            else if (value == LEFT_AXIS)
                return setObject(buf[2], "targetAxis", "LEFT_AXIS", true, true);
            else if (value == RIGHT_AXIS)
                return setObject(buf[2], "targetAxis", "RIGHT_AXIS", true, true);
            return *this;
        }
        // The type of this series. Valid only if the chartType is COMBO. Different types will change the way the series is visualized. Only LINE, AREA, and COLUMN are supported.
        BasicChartSeries &type(BasicChartType value)
        {
            if (value == BASIC_CHART_TYPE_UNSPECIFIED)
                return setObject(buf[3], "type", "BASIC_CHART_TYPE_UNSPECIFIED", true, true);
            else if (value == BAR)
                return setObject(buf[3], "type", "BAR", true, true);
            else if (value == LINE)
                return setObject(buf[3], "type", "LINE", true, true);
            else if (value == AREA)
                return setObject(buf[3], "type", "AREA", true, true);
            else if (value == COLUMN)
                return setObject(buf[3], "type", "COLUMN", true, true);
            else if (value == SCATTER)
                return setObject(buf[3], "type", "SCATTER", true, true);
            else if (value == COMBO)
                return setObject(buf[3], "type", "COMBO", true, true);
            else if (value == STEPPED_AREA)
                return setObject(buf[3], "type", "STEPPED_AREA", true, true);
            return *this;
        }
        // The line style of this series. Valid only if the chartType is AREA, LINE, or SCATTER. COMBO charts are also supported if the series chart type is AREA or LINE.
        BasicChartSeries &lineStyle(const LineStyle &value) { return setObject(buf[4], "lineStyle", value.c_str(), false, true); }
        // Information about the data labels for this series.
        BasicChartSeries &dataLabel(const DataLabel &value) { return setObject(buf[5], "dataLabel", value.c_str(), false, true); }
        // The color for elements (such as bars, lines, and points) associated with this series. If empty, a default color is used. If color is also set, this field takes precedence.
        BasicChartSeries &colorStyle(const ColorStyle &value) { return setObject(buf[6], "colorStyle", value.c_str(), false, true); }
        // The style for points associated with this series. Valid only if the chartType is AREA, LINE, or SCATTER. COMBO charts are also supported if the series chart type is AREA, LINE, or SCATTER. If empty, a default point style is used.
        BasicChartSeries &pointStyle(const PointStyle &value) { return setObject(buf[7], "pointStyle", value.c_str(), false, true); }
        // Style override settings for series data points.
        BasicChartSeries &addStyleOverrides(const BasicSeriesDataPointStyleOverride &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[8], FPSTR("styleOverrides"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The specification for a basic chart. See BasicChartType for the list of charts this supports.
     */
    class BasicChartSpec : public Printable
    {
    private:
        size_t bufSize = 13;
        String buf[13];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BasicChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BasicChartSpec() {}
        // The type of the chart.
        BasicChartSpec &chartType(BasicChartType value)
        {
            if (value == BASIC_CHART_TYPE_UNSPECIFIED)
                return setObject(buf[1], "chartType", "BASIC_CHART_TYPE_UNSPECIFIED", true, true);
            else if (value == BAR)
                return setObject(buf[1], "chartType", "BAR", true, true);
            else if (value == LINE)
                return setObject(buf[1], "chartType", "LINE", true, true);
            else if (value == AREA)
                return setObject(buf[1], "chartType", "AREA", true, true);
            else if (value == COLUMN)
                return setObject(buf[1], "chartType", "COLUMN", true, true);
            else if (value == SCATTER)
                return setObject(buf[1], "chartType", "SCATTER", true, true);
            else if (value == COMBO)
                return setObject(buf[1], "chartType", "COMBO", true, true);
            else if (value == STEPPED_AREA)
                return setObject(buf[1], "chartType", "STEPPED_AREA", true, true);
            return *this;
        }
        // The position of the chart legend.
        BasicChartSpec &legendPosition(BasicChartLegendPosition value)
        {
            if (value == BASIC_CHART_LEGEND_POSITION_UNSPECIFIED)
                return setObject(buf[2], "legendPosition", "BASIC_CHART_LEGEND_POSITION_UNSPECIFIED", true, true);
            else if (value == BOTTOM_LEGEND)
                return setObject(buf[2], "legendPosition", "BOTTOM_LEGEND", true, true);
            else if (value == LEFT_LEGEND)
                return setObject(buf[2], "legendPosition", "LEFT_LEGEND", true, true);
            else if (value == RIGHT_LEGEND)
                return setObject(buf[2], "legendPosition", "RIGHT_LEGEND", true, true);
            else if (value == TOP_LEGEND)
                return setObject(buf[2], "legendPosition", "TOP_LEGEND", true, true);
            else if (value == NO_LEGEND)
                return setObject(buf[2], "legendPosition", "NO_LEGEND", true, true);
            return *this;
        }
        // The axis on the chart.
        BasicChartSpec &addAxis(const BasicChartAxis &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("axis"), value.c_str(), false);
            return *this;
        }
        // The domain of data this is charting. Only a single domain is supported.
        BasicChartSpec &addDomains(const BasicChartDomain &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("domains"), value.c_str(), false);
            return *this;
        }
        // The data this chart is visualizing.
        BasicChartSpec &addSeries(const BasicChartSeries &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("series"), value.c_str(), false);
            return *this;
        }
        // The number of rows or columns in the data that are "headers". If not set, Google Sheets will guess how many rows are headers based on the data.
        //(Note that BasicChartAxis.title may override the axis title inferred from the header values.)
        BasicChartSpec &headerCount(int value) { return setObject(buf[6], "headerCount", String(value), false, true); }
        // True to make the chart 3D. Applies to Bar and Column charts.
        BasicChartSpec &threeDimensional(bool value) { return setObject(buf[7], "threeDimensional", owriter.getBoolStr(value), false, true); }
        // If some values in a series are missing, gaps may appear in the chart (e.g, segments of lines in a line chart will be missing). To eliminate these gaps set this to true. Applies to Line, Area, and Combo charts.
        BasicChartSpec &interpolateNulls(bool value) { return setObject(buf[8], "interpolateNulls", owriter.getBoolStr(value), false, true); }
        // The stacked type for charts that support vertical stacking. Applies to Area, Bar, Column, Combo, and Stepped Area charts.
        BasicChartSpec &stackedType(BasicChartStackedType value)
        {
            if (value == BASIC_CHART_STACKED_TYPE_UNSPECIFIED)
                return setObject(buf[9], "stackedType", "BASIC_CHART_STACKED_TYPE_UNSPECIFIED", true, true);
            else if (value == NOT_STACKED)
                return setObject(buf[9], "stackedType", "NOT_STACKED", true, true);
            else if (value == STACKED)
                return setObject(buf[9], "stackedType", "STACKED", true, true);
            else if (value == PERCENT_STACKED)
                return setObject(buf[9], "stackedType", "PERCENT_STACKED", true, true);
            return *this;
        }
        // Gets whether all lines should be rendered smooth or straight by default. Applies to Line charts.
        BasicChartSpec &lineSmoothing(bool value) { return setObject(buf[10], "lineSmoothing", owriter.getBoolStr(value), false, true); }
        // The behavior of tooltips and data highlighting when hovering on data and chart area.
        BasicChartSpec &compareMode(BasicChartCompareMode value)
        {
            if (value == BASIC_CHART_COMPARE_MODE_UNSPECIFIED)
                return setObject(buf[11], "compareMode", "BASIC_CHART_COMPARE_MODE_UNSPECIFIED", true, true);
            else if (value == DATUM)
                return setObject(buf[11], "compareMode", "DATUM", true, true);
            else if (value == CATEGORY)
                return setObject(buf[11], "compareMode", "CATEGORY", true, true);
            return *this;
        }
        // Controls whether to display additional data labels on stacked charts which sum the total value of all stacked values at each value along the domain axis. These data labels can only be set when chartType is one of AREA, BAR, COLUMN, COMBO or STEPPED_AREA and stackedType is either STACKED or PERCENT_STACKED. In addition, for COMBO, this will only be supported if there is only one type of stackable series type or one type has more series than the others and each of the other types have no more than one series. For example, if a chart has two stacked bar series and one area series, the total data labels will be supported. If it has three bar series and two area series, total data labels are not allowed. Neither CUSTOM nor placement can be set on the totalDataLabel.
        BasicChartSpec &totalDataLabel(const DataLabel &value) { return setObject(buf[12], "totalDataLabel", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}

#endif