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
    class HistogramSeries : public BaseG4
    {

    public:
        HistogramSeries() {}

        // The color of the column representing this series in each bucket. This field is optional. If barColor is also set, this field takes precedence.
        HistogramSeries &barColorStyle(const ColorStyle &value) { return wr.set<HistogramSeries &, ColorStyle>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The data for this histogram series.
        HistogramSeries &data(const ChartData &value) { return wr.set<HistogramSeries &, ChartData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A histogram chart. A histogram chart groups data items into bins, displaying each bin as a column of stacked items. Histograms are used to display the distribution of a dataset. Each column of items represents a range into which those items fall. The number of bins can be chosen automatically or specified explicitly.
     */
    class HistogramChartSpec : public BaseG6
    {
    public:
        HistogramChartSpec() {}

        // This value represents the item to add to an array.
        // The series for a histogram may be either a single series of values to be bucketed or multiple series, each of the same length, containing the name of the series followed by the values to be bucketed for that series.
        HistogramChartSpec &series(const HistogramSeries &value) { return wr.append<HistogramChartSpec &, HistogramSeries>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The position of the chart legend.
        HistogramChartSpec &legendPosition(HistogramChart::LegendPosition value) { return wr.append<HistogramChartSpec &, HistogramSeries>(*this, _HistogramChartLegendPosition[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // Whether horizontal divider lines should be displayed between items in each column.
        HistogramChartSpec &showItemDividers(bool value) { return wr.set<HistogramChartSpec &, bool>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // By default the bucket size (the range of values stacked in a single column) is chosen automatically, but it may be overridden here. E.g., A bucket size of 1.5 results in buckets from 0 - 1.5, 1.5 - 3.0, etc. Cannot be negative. This field is optional.
        HistogramChartSpec &bucketSize(double value) { return wr.set<HistogramChartSpec &, double>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The outlier percentile is used to ensure that outliers do not adversely affect the calculation of bucket sizes. For example, setting an outlier percentile of 0.05 indicates that the top and bottom 5% of values when calculating buckets. The values are still included in the chart, they will be added to the first or last buckets instead of their own buckets. Must be between 0.0 and 0.5.
        HistogramChartSpec &outlierPercentile(double value) { return wr.set<HistogramChartSpec &, double>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
    };
}

#endif