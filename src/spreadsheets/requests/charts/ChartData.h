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
    // Where the legend of the chart should be positioned.
    enum ChartLegendPosition
    {
        HISTOGRAM_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
        BUBBLE_CHART_LEGEND_POSITION_UNSPECIFIED,    //	Default value, do not use.
        PIE_CHART_LEGEND_POSITION_UNSPECIFIED,       //	Default value, do not use.
        BASIC_CHART_LEGEND_POSITION_UNSPECIFIED,     //	Default value, do not use.
        BOTTOM_LEGEND,                               //	The legend is rendered on the bottom of the chart.
        LEFT_LEGEND,                                 //	The legend is rendered on the left of the chart.
        RIGHT_LEGEND,                                //	The legend is rendered on the right of the chart.
        TOP_LEGEND,                                  //	The legend is rendered on the top of the chart.
        NO_LEGEND,                                   //	No legend is rendered.
        INSIDE_LEGEND,                               //	The legend is rendered inside the chart area.
        LABELED_LEGEND                               // Each pie slice has a label attached to it.
    };

    // The type of aggregation for chart series.
    enum ChartAggregateType
    {
        CHART_AGGREGATE_TYPE_UNSPECIFIED, //	Default value, do not use.
        AVERAGE_AGGREGATE,                          //	Average aggregate function.
        COUNT_AGGREGATE,                            //	Count aggregate function.
        MAX_AGGREGATE,                              //	Maximum aggregate function.
        MEDIAN_AGGREGATE,                           //	Median aggregate function.
        MIN_AGGREGATE,                              //	Minimum aggregate function.
        SUM_AGGREGATE                               //	Sum aggregate function.
    };

    // The available types of date-time grouping rules.
    enum ChartDateTimeRuleType
    {
        CHART_DATE_TIME_RULE_TYPE_UNSPECIFIED, //	The default type, do not use.
        BY_SECOND,                                //	Group dates by second, from 0 to 59.
        BY_MINUTE,                                //	Group dates by minute, from 0 to 59.
        BY_HOUR,                                  //	Group dates by hour using a 24-hour system, from 0 to 23.
        BY_HOUR_MINUTE,                           //	Group dates by hour and minute using a 24-hour system, for example 19:45.
        BY_HOUR_MINUTE_AMPM,                      //	Group dates by hour and minute using a 12-hour system, for example 7:45 PM. The AM/PM designation is translated based on the spreadsheet locale.
        BY_DAY_OF_WEEK,                           //	Group dates by day of week, for example Sunday. The days of the week will be translated based on the spreadsheet locale.
        BY_DAY_OF_YEAR,                           //	Group dates by day of year, from 1 to 366. Note that dates after Feb. 29 fall in different buckets in leap years than in non-leap years.
        BY_DAY_OF_MONTH,                          //	Group dates by day of month, from 1 to 31.
        BY_DAY_MONTH,                             //	Group dates by day and month, for example 22-Nov. The month is translated based on the spreadsheet locale.
        BY_MONTH,                                 //	Group dates by month, for example Nov. The month is translated based on the spreadsheet locale.
        BY_QUARTER,                               //	Group dates by quarter, for example Q1 (which represents Jan-Mar).
        BY_YEAR,                                  //	Group dates by year, for example 2008.
        BY_YEAR_MONTH,                            //	Group dates by year and month, for example 2008-Nov. The month is translated based on the spreadsheet locale.
        BY_YEAR_QUARTER,                          //	Group dates by year and quarter, for example 2008 Q4.
        BY_YEAR_MONTH_DAY                         //	Group dates by year, month, and day, for example 2008-11-22.
    };

    /**
     * Allows you to organize the date-time values in a source data column into buckets based on selected parts of their date or time values.
     */
    class ChartDateTimeRule : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        ChartDateTimeRule() {}
        // The type of date-time grouping to apply.
        ChartDateTimeRule &type(ChartDateTimeRuleType value)
        {
            clear();
            if (value == CHART_DATE_TIME_RULE_TYPE_UNSPECIFIED)
                jut.addObject(buf, "type", "CHART_DATE_TIME_RULE_TYPE_UNSPECIFIED", true, true);
            else if (value == BY_SECOND)
                jut.addObject(buf, "type", "SECOND", true, true);
            else if (value == BY_MINUTE)
                jut.addObject(buf, "type", "MINUTE", true, true);
            else if (value == BY_HOUR)
                jut.addObject(buf, "type", "HOUR", true, true);
            else if (value == BY_HOUR_MINUTE)
                jut.addObject(buf, "type", "HOUR_MINUTE", true, true);
            else if (value == BY_HOUR_MINUTE_AMPM)
                jut.addObject(buf, "type", "HOUR_MINUTE_AMPM", true, true);
            else if (value == BY_DAY_OF_WEEK)
                jut.addObject(buf, "type", "DAY_OF_WEEK", true, true);
            else if (value == BY_DAY_OF_YEAR)
                jut.addObject(buf, "type", "DAY_OF_YEAR", true, true);
            else if (value == BY_DAY_OF_MONTH)
                jut.addObject(buf, "type", "DAY_OF_MONTH", true, true);
            else if (value == BY_DAY_MONTH)
                jut.addObject(buf, "type", "DAY_MONTH", true, true);
            else if (value == BY_MONTH)
                jut.addObject(buf, "type", "MONTH", true, true);
            else if (value == BY_QUARTER)
                jut.addObject(buf, "type", "QUARTER", true, true);
            else if (value == BY_YEAR)
                jut.addObject(buf, "type", "YEAR", true, true);
            else if (value == BY_YEAR_MONTH)
                jut.addObject(buf, "type", "YEAR_MONTH", true, true);
            else if (value == BY_YEAR_QUARTER)
                jut.addObject(buf, "type", "YEAR_QUARTER", true, true);
            else if (value == BY_YEAR_MONTH_DAY)
                jut.addObject(buf, "type", "YEAR_MONTH_DAY", true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Allows you to organize numeric values in a source data column into buckets of constant size.
     */
    class ChartHistogramRule : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ChartHistogramRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ChartHistogramRule() {}
        // The minimum value at which items are placed into buckets. Values that are less than the minimum are grouped into a single bucket. If omitted, it is determined by the minimum item value.
        ChartHistogramRule &minValue(double value) { return setObject(buf[1], "minValue", String(value), false, true); }
        // The maximum value at which items are placed into buckets. Values greater than the maximum are grouped into a single bucket. If omitted, it is determined by the maximum item value.
        ChartHistogramRule &maxValue(double value) { return setObject(buf[2], "maxValue", String(value), false, true); }
        // The size of the buckets that are created. Must be positive.
        ChartHistogramRule &intervalSize(double value) { return setObject(buf[3], "intervalSize", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * An optional setting on the ChartData of the domain of a data source chart that defines buckets for the values in the domain rather than breaking out each individual value.
     */
    class ChartGroupRule : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;
        GSheetObjectWriter owriter;

    public:
        ChartGroupRule() {}
        // Union field rule
        // A ChartDateTimeRule.
        ChartGroupRule &dateTimeRule(const ChartDateTimeRule &value)
        {
            clear();
            jut.addObject(buf, "dateTimeRule", value.c_str(), false, true);
            return *this;
        }
        // Union field rule
        // A ChartHistogramRule
        ChartGroupRule &histogramRule(const ChartHistogramRule &value)
        {
            clear();
            jut.addObject(buf, "histogramRule", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Source ranges for a chart.
     */
    class ChartSourceRange : public Printable
    {
    private:
        size_t bufSize = 2;
        String buf[2];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

    public:
        ChartSourceRange() {}
        // The ranges of data for a series or domain. Exactly one dimension must have a length of 1, and all sources in the list must have the same dimension with length 1. The domain (if it exists) & all series must have the same number of source ranges. If using more than one source range, then the source range at a given offset must be in order and contiguous across the domain and series.
        ChartSourceRange &addSources(const GridRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("sources"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The data included in a domain or series.
     */
    class ChartData : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ChartData &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ChartData() {}
        // The rule to group the data by if the ChartData backs the domain of a data source chart. Only supported for data source charts.
        ChartData &groupRule(const ChartGroupRule &value) { return setObject(buf[1], "groupRule", value.c_str(), false, true); }
        // The aggregation type for the series of a data source chart. Only supported for data source charts.
        ChartData &aggregateType(ChartAggregateType value)
        {
            if (value == CHART_AGGREGATE_TYPE_UNSPECIFIED)
                return setObject(buf[2], "aggregateType", "CHART_AGGREGATE_TYPE_UNSPECIFIED", true, true);
            else if (value == AVERAGE_AGGREGATE)
                return setObject(buf[2], "aggregateType", "AVERAGE", true, true);
            else if (value == COUNT_AGGREGATE)
                return setObject(buf[2], "aggregateType", "COUNT", true, true);
            else if (value == MAX_AGGREGATE)
                return setObject(buf[2], "aggregateType", "MAX", true, true);
            else if (value == MEDIAN_AGGREGATE)
                return setObject(buf[2], "aggregateType", "MEDIAN", true, true);
            else if (value == MIN_AGGREGATE)
                return setObject(buf[2], "aggregateType", "MIN", true, true);
            else if (value == SUM_AGGREGATE)
                return setObject(buf[2], "aggregateType", "SUM", true, true);
            return *this;
        }
        // Union field type
        // The source ranges of the data.
        ChartData &sourceRange(const ChartSourceRange &value) { return setObject(buf[3], "sourceRange", value.c_str(), false, true); }
        // Union field type
        // The reference to the data source column that the data reads from.
        ChartData &columnReference(const DataSourceColumnReference &value) { return setObject(buf[3], "columnReference", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif