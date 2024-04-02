#ifndef HISTROGRAM_CHART_SPEC_H
#define HISTROGRAM_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/Common.h"

/**
 * HISTROGRAM CHART CLASS DEPENDENCIES
 *
 * HistogramChartSpec       HistogramSeries                         ColorStyle*
 *                                                                  ChartData*
 *                          (enum) HistogramChartLegendPosition
 *
 *
 * See ChartData.h, Theme.h
 */

namespace GSHEET
{


    /**
     * A histogram series containing the series color and data.
     */
    class HistogramSeries : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        HistogramSeries &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        HistogramSeries() {}
        // The color of the column representing this series in each bucket. This field is optional. If barColor is also set, this field takes precedence.
        HistogramSeries &barColorStyle(const ColorStyle &value) { return setObject(buf[1], "barColorStyle", value.c_str(), false, true); }
        // The data for this histogram series.
        HistogramSeries &data(const ChartData &value) { return setObject(buf[2], "data", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A histogram chart. A histogram chart groups data items into bins, displaying each bin as a column of stacked items. Histograms are used to display the distribution of a dataset. Each column of items represents a range into which those items fall. The number of bins can be chosen automatically or specified explicitly.
     */
    class HistogramChartSpec : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        HistogramChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        HistogramChartSpec() {}
        // The series for a histogram may be either a single series of values to be bucketed or multiple series, each of the same length, containing the name of the series followed by the values to be bucketed for that series.
        HistogramChartSpec &addSeries(const HistogramSeries &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("series"), value.c_str(), false);
            return *this;
        }
        // The position of the chart legend.
        HistogramChartSpec &legendPosition(ChartLegendPosition value)
        {
            if (value == HISTOGRAM_CHART_LEGEND_POSITION_UNSPECIFIED)
                return setObject(buf[2], "legendPosition", "HISTOGRAM_CHART_LEGEND_POSITION_UNSPECIFIED", true, true);
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
            else if (value == INSIDE_LEGEND)
                return setObject(buf[2], "legendPosition", "INSIDE_LEGEND", true, true);
            return *this;
        }
        // Whether horizontal divider lines should be displayed between items in each column.
        HistogramChartSpec &showItemDividers(bool value) { return setObject(buf[3], "showItemDividers", owriter.getBoolStr(value), false, true); }
        // By default the bucket size (the range of values stacked in a single column) is chosen automatically, but it may be overridden here. E.g., A bucket size of 1.5 results in buckets from 0 - 1.5, 1.5 - 3.0, etc. Cannot be negative. This field is optional.
        HistogramChartSpec &bucketSize(double value) { return setObject(buf[4], "bucketSize", String(value), false, true); }
        // The outlier percentile is used to ensure that outliers do not adversely affect the calculation of bucket sizes. For example, setting an outlier percentile of 0.05 indicates that the top and bottom 5% of values when calculating buckets. The values are still included in the chart, they will be added to the first or last buckets instead of their own buckets. Must be between 0.0 and 0.5.
        HistogramChartSpec &outlierPercentile(double value) { return setObject(buf[5], "outlierPercentile", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif