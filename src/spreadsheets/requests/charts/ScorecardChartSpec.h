#ifndef SCORECARD_CHART_SPEC_H
#define SCORECARD_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * SCORECARD CHART CLASS DEPENDENCIES
 *
 * ScorecardChartSpec       ChartData*
 *                          (enum) ChartAggregateType
 *                          KeyValueFormat                      TextFormat
 *                                                              TextPosition
 *
 *                          BaselineValueFormat                 (enum) ComparisonType
 *                                                              TextFormat
 *                                                              TextPosition
 *                                                              ColorStyle*
 *                          (enum) ChartNumberFormatSource
 *                          ChartCustomNumberFormatOptions
 *
 *
 * See ChartData.h, Theme.h
 */
namespace GSHEET
{
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

    // The comparison type of key value with baseline value.
    enum ComparisonType
    {
        COMPARISON_TYPE_UNDEFINED, //	Default value, do not use.
        ABSOLUTE_DIFFERENCE,       //	Use absolute difference between key and baseline value.
        PERCENTAGE_DIFFERENCE      //	Use percentage difference between key and baseline value.
    };

    // The number formatting source options for chart attributes.
    enum ChartNumberFormatSource
    {
        CHART_NUMBER_FORMAT_SOURCE_UNDEFINED, //	Default value, do not use.
        FROM_DATA,                            //	Inherit number formatting from data.
        CUSTOM                                //	Apply custom formatting as specified by ChartCustomNumberFormatOptions.
    };

    /**
     * Formatting options for key value.
     */
    class KeyValueFormat : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        KeyValueFormat &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        KeyValueFormat() {}
        // Text formatting options for key value. The link field is not supported.
        KeyValueFormat &textFormat(const TextFormat &value) { return setObject(buf[1], "textFormat", value.c_str(), false, true); }
        // Specifies the horizontal text positioning of key value. This field is optional. If not specified, default positioning is used.
        KeyValueFormat &position(const TextPosition &value) { return setObject(buf[2], "position", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Formatting options for baseline value.
     */
    class BaselineValueFormat : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BaselineValueFormat &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BaselineValueFormat() {}
        // The comparison type of key value with baseline value.
        BaselineValueFormat &comparisonType(ComparisonType value)
        {
            if (value == COMPARISON_TYPE_UNDEFINED)
                return setObject(buf[1], "comparisonType", "COMPARISON_TYPE_UNDEFINED", true, true);
            else if (value == ABSOLUTE_DIFFERENCE)
                return setObject(buf[1], "comparisonType", "ABSOLUTE_DIFFERENCE", true, true);
            else if (value == PERCENTAGE_DIFFERENCE)
                return setObject(buf[1], "comparisonType", "PERCENTAGE_DIFFERENCE", true, true);
            return *this;
        }
        // Text formatting options for baseline value. The link field is not supported.
        BaselineValueFormat &textFormat(const TextFormat &value) { return setObject(buf[2], "textFormat", value.c_str(), false, true); }
        // Specifies the horizontal text positioning of baseline value. This field is optional. If not specified, default positioning is used.
        BaselineValueFormat &position(const TextPosition &value) { return setObject(buf[3], "position", value.c_str(), false, true); }
        // /Description which is appended after the baseline value. This field is optional.
        BaselineValueFormat &description(const String &value) { return setObject(buf[4], "description", value, true, true); }
        // Color to be used, in case baseline value represents a positive change for key value. This field is optional. If positiveColor is also set, this field takes precedence.
        BaselineValueFormat &positiveColorStyle(const ColorStyle &value) { return setObject(buf[5], "positiveColorStyle", value.c_str(), false, true); }
        // Color to be used, in case baseline value represents a negative change for key value. This field is optional. If negativeColor is also set, this field takes precedence.
        BaselineValueFormat &negativeColorStyle(const ColorStyle &value) { return setObject(buf[6], "negativeColorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Custom number formatting options for chart attributes.
     */
    class ChartCustomNumberFormatOptions : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ChartCustomNumberFormatOptions &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ChartCustomNumberFormatOptions() {}
        // Custom prefix to be prepended to the chart attribute. This field is optional.
        ChartCustomNumberFormatOptions &prefix(const String &value) { return setObject(buf[1], "prefix", value, true, true); }
        // Custom suffix to be appended to the chart attribute. This field is optional.
        ChartCustomNumberFormatOptions &suffix(const String &value) { return setObject(buf[2], "suffix", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A scorecard chart. Scorecard charts are used to highlight key performance indicators, known as KPIs, on the spreadsheet. A scorecard chart can represent things like total sales, average cost, or a top selling item. You can specify a single data value, or aggregate over a range of data. Percentage or absolute difference from a baseline value can be highlighted, like changes over time.
     */
    class ScorecardChartSpec : public Printable
    {
    private:
        size_t bufSize = 9;
        String buf[9];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ScorecardChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ScorecardChartSpec() {}
        // The data for scorecard key value.
        ScorecardChartSpec &keyValueData(const ChartData &value) { return setObject(buf[1], "keyValueData", value.c_str(), false, true); }
        // The data for scorecard baseline value. This field is optional.
        ScorecardChartSpec &baselineValueData(const ChartData &value) { return setObject(buf[2], "baselineValueData", value.c_str(), false, true); }
        // The aggregation type for key and baseline chart data in scorecard chart. This field is not supported for data source charts. Use the ChartData.aggregateType field of the keyValueData or baselineValueData instead for data source charts. This field is optional.
        ScorecardChartSpec &aggregateType(ChartAggregateType value)
        {
            if (value == CHART_AGGREGATE_TYPE_UNSPECIFIED)
                return setObject(buf[3], "aggregateType", "CHART_AGGREGATE_TYPE_UNSPECIFIED", true, true);
            else if (value == AVERAGE)
                return setObject(buf[3], "aggregateType", "AVERAGE", true, true);
            else if (value == COUNT)
                return setObject(buf[3], "aggregateType", "COUNT", true, true);
            else if (value == MAX)
                return setObject(buf[3], "aggregateType", "MAX", true, true);
            else if (value == MEDIAN)
                return setObject(buf[3], "aggregateType", "MEDIAN", true, true);
            else if (value == MIN)
                return setObject(buf[3], "aggregateType", "MIN", true, true);
            else if (value == SUM)
                return setObject(buf[3], "aggregateType", "SUM", true, true);
            return *this;
        }
        // Formatting options for key value.
        ScorecardChartSpec &keyValueFormat(const KeyValueFormat &value) { return setObject(buf[4], "keyValueFormat", value.c_str(), false, true); }
        // Formatting options for baseline value. This field is needed only if baselineValueData is specified.
        ScorecardChartSpec &baselineValueFormat(const BaselineValueFormat &value) { return setObject(buf[5], "baselineValueFormat", value.c_str(), false, true); }
        // Value to scale scorecard key and baseline value. For example, a factor of 10 can be used to divide all values in the chart by 10. This field is optional.
        ScorecardChartSpec &scaleFactor(double value) { return setObject(buf[6], "scaleFactor", String(value), false, true); }
        // The number format source used in the scorecard chart. This field is optional.
        ScorecardChartSpec &numberFormatSource(ChartNumberFormatSource value)
        {
            if (value == CHART_NUMBER_FORMAT_SOURCE_UNDEFINED)
                return setObject(buf[7], "numberFormatSource", "CHART_NUMBER_FORMAT_SOURCE_UNDEFINED", true, true);
            else if (value == FROM_DATA)
                return setObject(buf[7], "numberFormatSource", "FROM_DATA", true, true);
            else if (value == CUSTOM)
                return setObject(buf[7], "numberFormatSource", "CUSTOM", true, true);
            return *this;
        }
        // Custom formatting options for numeric key/baseline values in scorecard chart. This field is used only when numberFormatSource is set to CUSTOM. This field is optional.
        ScorecardChartSpec &customFormatOptions(const ChartCustomNumberFormatOptions &value) { return setObject(buf[8], "customFormatOptions", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif