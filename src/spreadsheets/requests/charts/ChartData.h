#ifndef CHART_DATA_SPEC_H
#define CHART_DATA_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/GridRange.h"

/**
 * CHART DATA CLASS DEPENDENCIES
 *
 * ChartData        ChartGroupRule                  ChartDateTimeRule       (enum) ChartDateTimeRuleType
 *                                                  ChartHistogramRule
 *
 *                  (enum) ChartAggregateType
 *                  ChartSourceRange                GridRange
 *                  DataSourceColumnReference
 *
 * See Theme.h
 */

namespace GSHEET
{
    namespace HistogramChart
    {
        // Where the legend of the chart should be positioned.
        enum LegendPosition
        {
            HISTOGRAM_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
            BOTTOM_LEGEND,                               //	The legend is rendered on the bottom of the chart.
            LEFT_LEGEND,                                 //	The legend is rendered on the left of the chart.
            RIGHT_LEGEND,                                //	The legend is rendered on the right of the chart.
            TOP_LEGEND,                                  //	The legend is rendered on the top of the chart.
            NO_LEGEND,                                   //	No legend is rendered.
            INSIDE_LEGEND                                //	The legend is rendered inside the chart area.
        };
    }
    namespace BubbleChart
    {
        enum LegendPosition
        {
            BUBBLE_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
            BOTTOM_LEGEND,                            //	The legend is rendered on the bottom of the chart.
            LEFT_LEGEND,                              //	The legend is rendered on the left of the chart.
            RIGHT_LEGEND,                             //	The legend is rendered on the right of the chart.
            TOP_LEGEND,                               //	The legend is rendered on the top of the chart.
            NO_LEGEND,                                //	No legend is rendered.
            INSIDE_LEGEND                             //	The legend is rendered inside the chart area.
        };
    }

    namespace PieChart
    {
        enum LegendPosition
        {
            PIE_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
            BOTTOM_LEGEND,                         //	The legend is rendered on the bottom of the chart.
            LEFT_LEGEND,                           //	The legend is rendered on the left of the chart.
            RIGHT_LEGEND,                          //	The legend is rendered on the right of the chart.
            TOP_LEGEND,                            //	The legend is rendered on the top of the chart.
            NO_LEGEND,                             //	No legend is rendered.
            LABELED_LEGEND                         // Each pie slice has a label attached to it.
        };
    }
    namespace BasicChart
    {
        enum LegendPosition
        {
            BASIC_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
            BOTTOM_LEGEND,                           //	The legend is rendered on the bottom of the chart.
            LEFT_LEGEND,                             //	The legend is rendered on the left of the chart.
            RIGHT_LEGEND,                            //	The legend is rendered on the right of the chart.
            TOP_LEGEND,                              //	The legend is rendered on the top of the chart.
            NO_LEGEND                                //	No legend is rendered.
        };
    }

    // The type of aggregation for chart series.
    enum ChartAggregateType
    {
        CHART_AGGREGATE_TYPE_UNSPECIFIED, //	Default value, do not use.
        AVERAGE,                          //	Average aggregate function.
        COUNT,                            //	Count aggregate function.
        MAX,                              //	Maximum aggregate function.
        MEDIAN,                           //	Median aggregate function.
        MIN,                              //	Minimum aggregate function.
        SUM                               //	Sum aggregate function.
    };

    // The available types of date-time grouping rules.
    enum ChartDateTimeRuleType
    {
        CHART_DATE_TIME_RULE_TYPE_UNSPECIFIED, //	The default type, do not use.
        SECOND,                                //	Group dates by second, from 0 to 59.
        MINUTE,                                //	Group dates by minute, from 0 to 59.
        HOUR,                                  //	Group dates by hour using a 24-hour system, from 0 to 23.
        HOUR_MINUTE,                           //	Group dates by hour and minute using a 24-hour system, for example 19:45.
        HOUR_MINUTE_AMPM,                      //	Group dates by hour and minute using a 12-hour system, for example 7:45 PM. The AM/PM designation is translated based on the spreadsheet locale.
        DAY_OF_WEEK,                           //	Group dates by day of week, for example Sunday. The days of the week will be translated based on the spreadsheet locale.
        DAY_OF_YEAR,                           //	Group dates by day of year, from 1 to 366. Note that dates after Feb. 29 fall in different buckets in leap years than in non-leap years.
        DAY_OF_MONTH,                          //	Group dates by day of month, from 1 to 31.
        DAY_MONTH,                             //	Group dates by day and month, for example 22-Nov. The month is translated based on the spreadsheet locale.
        MONTH,                                 //	Group dates by month, for example Nov. The month is translated based on the spreadsheet locale.
        QUARTER,                               //	Group dates by quarter, for example Q1 (which represents Jan-Mar).
        YEAR,                                  //	Group dates by year, for example 2008.
        YEAR_MONTH,                            //	Group dates by year and month, for example 2008-Nov. The month is translated based on the spreadsheet locale.
        YEAR_QUARTER,                          //	Group dates by year and quarter, for example 2008 Q4.
        YEAR_MONTH_DAY                         //	Group dates by year, month, and day, for example 2008-11-22.
    };

    const struct key_str_60 _HistogramChartLegendPosition[HistogramChart::LegendPosition::INSIDE_LEGEND + 1] PROGMEM = {"HISTOGRAM_CHART_LEGEND_POSITION_UNSPECIFIED", "BOTTOM_LEGEND", "LEFT_LEGEND", "RIGHT_LEGEND", "TOP_LEGEND", "NO_LEGEND", "INSIDE_LEGEND"};
    const struct key_str_60 _BubbleChartLegendPosition[BubbleChart::LegendPosition::INSIDE_LEGEND + 1] PROGMEM = {"BUBBLE_CHART_LEGEND_POSITION_UNSPECIFIED", "BOTTOM_LEGEND", "LEFT_LEGEND", "RIGHT_LEGEND", "TOP_LEGEND", "NO_LEGEND", "INSIDE_LEGEND"};
    const struct key_str_60 _PieChartLegendPosition[PieChart::LegendPosition::LABELED_LEGEND + 1] PROGMEM = {"PIE_CHART_LEGEND_POSITION_UNSPECIFIED", "BOTTOM_LEGEND", "LEFT_LEGEND", "RIGHT_LEGEND", "TOP_LEGEND", "NO_LEGEND", "LABELED_LEGEND"};
    const struct key_str_60 _BasicChartLegendPosition[BasicChart::LegendPosition::NO_LEGEND + 1] PROGMEM = {"BASIC_CHART_LEGEND_POSITION_UNSPECIFIED", "BOTTOM_LEGEND", "LEFT_LEGEND", "RIGHT_LEGEND", "TOP_LEGEND", "NO_LEGEND"};
    const struct key_str_40 _ChartAggregateType[ChartAggregateType::SUM + 1] PROGMEM = {"CHART_AGGREGATE_TYPE_UNSPECIFIED", "AVERAGE", "COUNT", "MAX", "MEDIAN", "MIN", "SUM"};
    const struct key_str_60 _ChartDateTimeRuleType[ChartDateTimeRuleType::YEAR_MONTH_DAY + 1] PROGMEM = {"CHART_DATE_TIME_RULE_TYPE_UNSPECIFIED", "SECOND", "MINUTE", "HOUR", "HOUR_MINUTE", "HOUR_MINUTE_AMPM", "DAY_OF_WEEK", "DAY_OF_YEAR", "DAY_OF_MONTH", "DAY_MONTH", "MONTH", "QUARTER", "YEAR", "YEAR_MONTH", "YEAR_QUARTER", "YEAR_MONTH_DAY"};

    /**
     * Allows you to organize the date-time values in a source data column into buckets based on selected parts of their date or time values.
     */
    class ChartDateTimeRule : public BaseG1
    {

    public:
        ChartDateTimeRule() = default;
        
        // The type of date-time grouping to apply.
        ChartDateTimeRule &type(ChartDateTimeRuleType value) { return wr.add<ChartDateTimeRule &, const char *>(*this, _ChartDateTimeRuleType[value].text, buf, FPSTR(__func__)); }
        
    };

    /**
     * Allows you to organize numeric values in a source data column into buckets of constant size.
     */
    class ChartHistogramRule : public BaseG4
    {

    public:
        ChartHistogramRule() = default;

        // The minimum value at which items are placed into buckets. Values that are less than the minimum are grouped into a single bucket. If omitted, it is determined by the minimum item value.
        ChartHistogramRule &minValue(double value) { return wr.set<ChartHistogramRule &, double>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The maximum value at which items are placed into buckets. Values greater than the maximum are grouped into a single bucket. If omitted, it is determined by the maximum item value.
        ChartHistogramRule &maxValue(double value) { return wr.set<ChartHistogramRule &, double>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The size of the buckets that are created. Must be positive.
        ChartHistogramRule &intervalSize(double value) { return wr.set<ChartHistogramRule &, double>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * An optional setting on the ChartData of the domain of a data source chart that defines buckets for the values in the domain rather than breaking out each individual value.
     */
    class ChartGroupRule : public BaseG1
    {
    private:
        String buf;
        GSheetJSONUtil jut;
        GSheetObjectWriter owriter;

    public:
        ChartGroupRule() = default;

        // Union field rule
        // A ChartDateTimeRule.
        ChartGroupRule &dateTimeRule(const ChartDateTimeRule &value) { return wr.add<ChartGroupRule &, ChartDateTimeRule>(*this, value, buf, FPSTR(__func__)); }

        // Union field rule
        // A ChartHistogramRule
        ChartGroupRule &histogramRule(const ChartHistogramRule &value) { return wr.add<ChartGroupRule &, ChartHistogramRule>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Source ranges for a chart.
     */
    class ChartSourceRange : public BaseG2
    {

    public:
        ChartSourceRange() = default;

        // This value represents the item to add to an array.
        // The ranges of data for a series or domain. Exactly one dimension must have a length of 1, and all sources in the list must have the same dimension with length 1. The domain (if it exists) & all series must have the same number of source ranges. If using more than one source range, then the source range at a given offset must be in order and contiguous across the domain and series.
        ChartSourceRange &sources(const GridRange &value) { return wr.append<ChartSourceRange &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }
    };

    /**
     * The data included in a domain or series.
     */
    class ChartData : public BaseG4
    {

    public:
        ChartData() = default;

        // The rule to group the data by if the ChartData backs the domain of a data source chart. Only supported for data source charts.
        ChartData &groupRule(const ChartGroupRule &value) { return wr.set<ChartData &, ChartGroupRule>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The aggregation type for the series of a data source chart. Only supported for data source charts.
        ChartData &aggregateType(ChartAggregateType value) { return wr.set<ChartData &, const char *>(*this, _ChartAggregateType[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field type
        // The source ranges of the data.
        ChartData &sourceRange(const ChartSourceRange &value) { return wr.set<ChartData &, ChartSourceRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Union field type
        // The reference to the data source column that the data reads from.
        ChartData &columnReference(const DataSourceColumnReference &value) { return wr.set<ChartData &, DataSourceColumnReference>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };
}

#endif