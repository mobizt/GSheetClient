#ifndef BUBBLE_CHART_SPEC_H
#define BUBBLE_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * BUBBLE CHART CLASS DEPENDENCIES
 *
 * BubbleChartSpec          (enum) BubbleChartLegendPosition
 *                          ChartData*
 *                          ColorStyle*
 *                          TextFormat
 *
 * See ChartData.h, Theme.h
 */

namespace GSHEET
{

    /**
     * A bubble chart.
     */
    class BubbleChartSpec : public BaseG12
    {

    public:
        BubbleChartSpec() {}

        // Where the legend of the chart should be drawn.
        BubbleChartSpec &legendPosition(BubbleChart::LegendPosition value) { return wr.set<BubbleChartSpec &, const char *>(*this, _BubbleChartLegendPosition[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The data containing the bubble labels. These do not need to be unique.
        BubbleChartSpec &bubbleLabels(const ChartData &value) { return wr.set<BubbleChartSpec &, ChartData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The data containing the bubble x-values. These values locate the bubbles in the chart horizontally.
        BubbleChartSpec &domain(const ChartData &value) { return wr.set<BubbleChartSpec &, ChartData>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The data containing the bubble y-values. These values locate the bubbles in the chart vertically.
        BubbleChartSpec &series(const ChartData &value) { return wr.set<BubbleChartSpec &, ChartData>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The data containing the bubble group IDs. All bubbles with the same group ID are drawn in the same color. If bubbleSizes is specified then this field must also be specified but may contain blank values. This field is optional.
        BubbleChartSpec &groupIds(const ChartData &value) { return wr.set<BubbleChartSpec &, ChartData>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The data containing the bubble sizes. Bubble sizes are used to draw the bubbles at different sizes relative to each other. If specified, groupIds must also be specified. This field is optional.
        BubbleChartSpec &bubbleSizes(const ChartData &value) { return wr.set<BubbleChartSpec &, ChartData>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The opacity of the bubbles between 0 and 1.0. 0 is fully transparent and 1 is fully opaque.
        BubbleChartSpec &bubbleOpacity(double value) { return wr.set<BubbleChartSpec &, double>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // The bubble border color. If bubbleBorderColor is also set, this field takes precedence.
        BubbleChartSpec &bubbleBorderColorStyle(const ColorStyle &value) { return wr.set<BubbleChartSpec &, ColorStyle>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }

        // The max radius size of the bubbles, in pixels. If specified, the field must be a positive value.
        BubbleChartSpec &bubbleMaxRadiusSize(int value) { return wr.set<BubbleChartSpec &, int>(*this, value, buf, bufSize, 9, FPSTR(__func__)); }

        // The minimum radius size of the bubbles, in pixels. If specific, the field must be a positive value.
        BubbleChartSpec &bubbleMinRadiusSize(int value) { return wr.set<BubbleChartSpec &, int>(*this, value, buf, bufSize, 10, FPSTR(__func__)); }

        // The format of the text inside the bubbles. Strikethrough, underline, and link are not supported.
        BubbleChartSpec &bubbleTextStyle(const TextFormat value) { return wr.set<BubbleChartSpec &, TextFormat>(*this, value, buf, bufSize, 11, FPSTR(__func__)); }
    };
}

#endif