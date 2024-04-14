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
        CUSTOM_FORMAT                         //	Apply custom formatting as specified by ChartCustomNumberFormatOptions.
    };

    const struct key_str_40 _ComparisonType[ComparisonType::PERCENTAGE_DIFFERENCE + 1] PROGMEM = {"COMPARISON_TYPE_UNDEFINED", "ABSOLUTE_DIFFERENCE", "PERCENTAGE_DIFFERENCE"};

    const struct key_str_40 _ChartNumberFormatSource[ChartNumberFormatSource::CUSTOM_FORMAT + 1] PROGMEM = {"CHART_NUMBER_FORMAT_SOURCE_UNDEFINED", "FROM_DATA", "CUSTOM_FORMAT"};

    /**
     * Formatting options for key value.
     */
    class KeyValueFormat : public BaseG4
    {

    public:
        KeyValueFormat() = default;

        // Text formatting options for key value. The link field is not supported.
        KeyValueFormat &textFormat(const TextFormat &value) { return wr.set<KeyValueFormat &, TextFormat>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Specifies the horizontal text positioning of key value. This field is optional. If not specified, default positioning is used.
        KeyValueFormat &position(const TextPosition &value) { return wr.set<KeyValueFormat &, TextPosition>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Formatting options for baseline value.
     */
    class BaselineValueFormat : public BaseG8
    {

    public:
        BaselineValueFormat() = default;
        // The comparison type of key value with baseline value.
        BaselineValueFormat &comparisonType(ComparisonType value) { return wr.set<BaselineValueFormat &, const char *>(*this, _ComparisonType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // Text formatting options for baseline value. The link field is not supported.
        BaselineValueFormat &textFormat(const TextFormat &value) { return wr.set<BaselineValueFormat &, TextFormat>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Specifies the horizontal text positioning of baseline value. This field is optional. If not specified, default positioning is used.
        BaselineValueFormat &position(const TextPosition &value) { return wr.set<BaselineValueFormat &, TextPosition>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
        // /Description which is appended after the baseline value. This field is optional.
        BaselineValueFormat &description(const String &value) { return wr.set<BaselineValueFormat &, String>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Color to be used, in case baseline value represents a positive change for key value. This field is optional. If positiveColor is also set, this field takes precedence.
        BaselineValueFormat &positiveColorStyle(const ColorStyle &value) { return wr.set<BaselineValueFormat &, ColorStyle>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // Color to be used, in case baseline value represents a negative change for key value. This field is optional. If negativeColor is also set, this field takes precedence.
        BaselineValueFormat &negativeColorStyle(const ColorStyle &value) { return wr.set<BaselineValueFormat &, ColorStyle>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }
    };

    /**
     * Custom number formatting options for chart attributes.
     */
    class ChartCustomNumberFormatOptions : public BaseG4
    {
    public:
        ChartCustomNumberFormatOptions() = default;

        // Custom prefix to be prepended to the chart attribute. This field is optional.
        ChartCustomNumberFormatOptions &prefix(const String &value) { return wr.set<ChartCustomNumberFormatOptions &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Custom suffix to be appended to the chart attribute. This field is optional.
        ChartCustomNumberFormatOptions &suffix(const String &value) { return wr.set<ChartCustomNumberFormatOptions &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A scorecard chart. Scorecard charts are used to highlight key performance indicators, known as KPIs, on the spreadsheet. A scorecard chart can represent things like total sales, average cost, or a top selling item. You can specify a single data value, or aggregate over a range of data. Percentage or absolute difference from a baseline value can be highlighted, like changes over time.
     */
    class ScorecardChartSpec : public BaseG12
    {

    public:
        ScorecardChartSpec() = default;

        // The data for scorecard key value.
        ScorecardChartSpec &keyValueData(const ChartData &value) { return wr.set<ScorecardChartSpec &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The data for scorecard baseline value. This field is optional.
        ScorecardChartSpec &baselineValueData(const ChartData &value) { return wr.set<ScorecardChartSpec &, ChartData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The aggregation type for key and baseline chart data in scorecard chart. This field is not supported for data source charts. Use the ChartData.aggregateType field of the keyValueData or baselineValueData instead for data source charts. This field is optional.
        ScorecardChartSpec &aggregateType(ChartAggregateType value) { return wr.set<ScorecardChartSpec &, const char *>(*this, _ChartAggregateType[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // Formatting options for key value.
        ScorecardChartSpec &keyValueFormat(const KeyValueFormat &value) { return wr.set<ScorecardChartSpec &, KeyValueFormat>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Formatting options for baseline value. This field is needed only if baselineValueData is specified.
        ScorecardChartSpec &baselineValueFormat(const BaselineValueFormat &value) { return wr.set<ScorecardChartSpec &, BaselineValueFormat>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // Value to scale scorecard key and baseline value. For example, a factor of 10 can be used to divide all values in the chart by 10. This field is optional.
        ScorecardChartSpec &scaleFactor(double value) { return wr.set<ScorecardChartSpec &, bool>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The number format source used in the scorecard chart. This field is optional.
        ScorecardChartSpec &numberFormatSource(ChartNumberFormatSource value) { return wr.set<ScorecardChartSpec &, const char *>(*this, _ChartNumberFormatSource[value].text, buf, bufSize, 7, FPSTR(__func__)); }

        // Custom formatting options for numeric key/baseline values in scorecard chart. This field is used only when numberFormatSource is set to CUSTOM. This field is optional.
        ScorecardChartSpec &customFormatOptions(const ChartCustomNumberFormatOptions &value) { return wr.set<ScorecardChartSpec &, ChartCustomNumberFormatOptions>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }
    };
}

#endif