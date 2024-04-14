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
#include "./spreadsheets/requests/Common.h"

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

    /**
     * The options that define a "view window" for a chart (such as the visible values in an axis).
     */
    class ChartAxisViewWindowOptions : public BaseG4
    {
    public:
        ChartAxisViewWindowOptions() = default;

        // The minimum numeric value to be shown in this view window. If unset, will automatically determine a minimum value that looks good for the data.
        ChartAxisViewWindowOptions &viewWindowMin(double value) { return wr.set<ChartAxisViewWindowOptions &, double>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The maximum numeric value to be shown in this view window. If unset, will automatically determine a maximum value that looks good for the data.
        ChartAxisViewWindowOptions &viewWindowMax(double value) { return wr.set<ChartAxisViewWindowOptions &, double>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The view window's mode.
        ChartAxisViewWindowOptions &viewWindowMode(Basic::ViewWindowMode value) { return wr.set<ChartAxisViewWindowOptions &, const char *>(*this, _ViewWindowMode[value].text, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * An axis of the chart. A chart may not have more than one axis per axis position.
     */
    class BasicChartAxis : public BaseG6
    {

    public:
        BasicChartAxis() = default;

        // The position of this axis.
        BasicChartAxis &position(Basic::BasicChartAxisPosition value) { return wr.set<BasicChartAxis &, const char *>(*this, _BasicChartAxisPosition[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The title of this axis. If set, this overrides any title inferred from headers of the data.
        BasicChartAxis &title(const String &value) { return wr.set<BasicChartAxis &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The format of the title. Only valid if the axis is not associated with the domain. The link field is not supported.
        BasicChartAxis &format(const TextFormat &value) { return wr.set<BasicChartAxis &, TextFormat>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The axis title text position.
        BasicChartAxis &titleTextPosition(const TextPosition &value) { return wr.set<BasicChartAxis &, TextPosition>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The view window options for this axis.
        BasicChartAxis &viewWindowOptions(const ChartAxisViewWindowOptions &value) { return wr.set<BasicChartAxis &, ChartAxisViewWindowOptions>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
    };

    /**
     * The domain of a chart. For example, if charting stock prices over time, this would be the date.
     */
    class BasicChartDomain : public BaseG4
    {

    public:
        BasicChartDomain() = default;

        // The data of the domain. For example, if charting stock prices over time, this is the data representing the dates.
        BasicChartDomain &domain(const ChartData &value) { return wr.set<BasicChartDomain &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // True to reverse the order of the domain values (horizontal axis).
        BasicChartDomain &reversed(bool value) { return wr.set<BasicChartDomain &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * The style of a point on the chart.
     */
    class PointStyle : public BaseG4
    {

    public:
        PointStyle() = default;
        
        // The point size. If empty, a default size is used.
        PointStyle &size(double value) { return wr.set<PointStyle &, double>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
        
        // The point shape. If empty or unspecified, a default shape is used.
        PointStyle &shape(Basic::PointShape value) { return wr.set<PointStyle &, const char *>(*this, _PointShape[value].text, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Style override settings for a single series data point.
     */
    class BasicSeriesDataPointStyleOverride : public BaseG4
    {

    public:
        BasicSeriesDataPointStyleOverride() = default;
        
        // The zero-based index of the series data point.
        BasicSeriesDataPointStyleOverride &index(int value) { return wr.set<BasicSeriesDataPointStyleOverride &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
       
        // Color of the series data point. If empty, the series default is used. If color is also set, this field takes precedence.
        BasicSeriesDataPointStyleOverride &colorStyle(const ColorStyle &value) { return wr.set<BasicSeriesDataPointStyleOverride &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
        
        // Point style of the series data point. Valid only if the chartType is AREA, LINE, or SCATTER. COMBO charts are also supported if the series chart type is AREA, LINE, or SCATTER. If empty, the series default is used.
        BasicSeriesDataPointStyleOverride &pointStyle(const PointStyle &value) { return wr.set<BasicSeriesDataPointStyleOverride &, PointStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A single series of data in a chart. For example, if charting stock prices over time, multiple series may exist, one for the "Open Price", "High Price", "Low Price" and "Close Price".
     */
    class BasicChartSeries : public BaseG12
    {
    public:
        BasicChartSeries() = default;
        
        // The data being visualized in this chart series.
        BasicChartSeries &series(const ChartData &value) { return wr.set<BasicChartSeries &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
        
        // The minor axis that will specify the range of values for this series. For example, if charting stocks over time, the "Volume" series may want to be pinned to the right with the prices pinned to the left, because the scale of trading volume is different than the scale of prices. It is an error to specify an axis that isn't a valid minor axis for the chart's type.
        BasicChartSeries &targetAxis(Basic::BasicChartAxisPosition value) { return wr.set<BasicChartSeries &, const char *>(*this, _BasicChartAxisPosition[value].text, buf, bufSize, 2, FPSTR(__func__)); }
        
        // The type of this series. Valid only if the chartType is COMBO. Different types will change the way the series is visualized. Only LINE, AREA, and COLUMN are supported.
        BasicChartSeries &type(Basic::BasicChartType value) { return wr.set<BasicChartSeries &, const char *>(*this, _BasicChartType[value].text, buf, bufSize, 3, FPSTR(__func__)); }
        
        // The line style of this series. Valid only if the chartType is AREA, LINE, or SCATTER. COMBO charts are also supported if the series chart type is AREA or LINE.
        BasicChartSeries &lineStyle(const LineStyle &value) { return wr.set<BasicChartSeries &, LineStyle>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
        
        // Information about the data labels for this series.
        BasicChartSeries &dataLabel(const DataLabel &value) { return wr.set<BasicChartSeries &, DataLabel>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
        
        // The color for elements (such as bars, lines, and points) associated with this series. If empty, a default color is used. If color is also set, this field takes precedence.
        BasicChartSeries &colorStyle(const ColorStyle &value) { return wr.set<BasicChartSeries &, ColorStyle>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }
        
        // The style for points associated with this series. Valid only if the chartType is AREA, LINE, or SCATTER. COMBO charts are also supported if the series chart type is AREA, LINE, or SCATTER. If empty, a default point style is used.
        BasicChartSeries &pointStyle(const PointStyle &value) { return wr.set<BasicChartSeries &, PointStyle>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }
        
        // This value represents the item to add to an array.
        //  Style override settings for series data points.
        BasicChartSeries &styleOverrides(const BasicSeriesDataPointStyleOverride &value) { return wr.append<BasicChartSeries &, BasicSeriesDataPointStyleOverride>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }
    };

    /**
     * The specification for a basic chart. See BasicChartType for the list of charts this supports.
     */
    class BasicChartSpec : public BaseG16
    {

    public:
        BasicChartSpec() = default;

        // The type of the chart.
        BasicChartSpec &chartType(Basic::BasicChartType value) { return wr.set<BasicChartSpec &, ChartData>(*this, _BasicChartType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The position of the chart legend.
        BasicChartSpec &legendPosition(BasicChart::LegendPosition value) { return wr.set<BasicChartSpec &, const char *>(*this, _BasicChartLegendPosition[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The axis on the chart.
        BasicChartSpec &axis(const BasicChartAxis &value) { return wr.append<BasicChartSpec &, BasicChartAxis>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The domain of data this is charting. Only a single domain is supported.
        BasicChartSpec &domains(const BasicChartDomain &value) { return wr.append<BasicChartSpec &, BasicChartDomain>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The data this chart is visualizing.
        BasicChartSpec &series(const BasicChartSeries &value) { return wr.append<BasicChartSpec &, BasicChartSeries>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The number of rows or columns in the data that are "headers". If not set, Google Sheets will guess how many rows are headers based on the data.
        //(Note that BasicChartAxis.title may override the axis title inferred from the header values.)
        BasicChartSpec &headerCount(int value) { return wr.set<BasicChartSpec &, int>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // True to make the chart 3D. Applies to Bar and Column charts.
        BasicChartSpec &threeDimensional(bool value) { return wr.set<BasicChartSpec &, bool>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // If some values in a series are missing, gaps may appear in the chart (e.g, segments of lines in a line chart will be missing). To eliminate these gaps set this to true. Applies to Line, Area, and Combo charts.
        BasicChartSpec &interpolateNulls(bool value) { return wr.set<BasicChartSpec &, bool>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }

        // The stacked type for charts that support vertical stacking. Applies to Area, Bar, Column, Combo, and Stepped Area charts.
        BasicChartSpec &stackedType(Basic::BasicChartStackedType value){ return wr.set<BasicChartSpec &, const char*>(*this, _BasicChartStackedType[value].text, buf, bufSize, 9, FPSTR(__func__)); }

        // Gets whether all lines should be rendered smooth or straight by default. Applies to Line charts.
        BasicChartSpec &lineSmoothing(bool value) { return wr.set<BasicChartSpec &, bool>(*this, value, buf, bufSize, 10, FPSTR(__func__)); }

        // The behavior of tooltips and data highlighting when hovering on data and chart area.
        BasicChartSpec &compareMode(Basic::BasicChartCompareMode value){ return wr.set<BasicChartSpec &, const char*>(*this, _BasicChartCompareMode[value].text, buf, bufSize, 11, FPSTR(__func__)); }
        
        // Controls whether to display additional data labels on stacked charts which sum the total value of all stacked values at each value along the domain axis. These data labels can only be set when chartType is one of AREA, BAR, COLUMN, COMBO or STEPPED_AREA and stackedType is either STACKED or PERCENT_STACKED. In addition, for COMBO, this will only be supported if there is only one type of stackable series type or one type has more series than the others and each of the other types have no more than one series. For example, if a chart has two stacked bar series and one area series, the total data labels will be supported. If it has three bar series and two area series, total data labels are not allowed. Neither CUSTOM nor placement can be set on the totalDataLabel.
        BasicChartSpec &totalDataLabel(const DataLabel &value) { return wr.set<BasicChartSpec &, DataLabel>(*this, value, buf, bufSize, 12, FPSTR(__func__)); }
    };

}

#endif