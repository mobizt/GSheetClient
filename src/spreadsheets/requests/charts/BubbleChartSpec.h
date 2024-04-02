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
    class BubbleChartSpec : public Printable
    {
    private:
        size_t bufSize = 12;
        String buf[12];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BubbleChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BubbleChartSpec() {}
        // Where the legend of the chart should be drawn.
        BubbleChartSpec &legendPosition(ChartLegendPosition value)
        {
            if (value == BUBBLE_CHART_LEGEND_POSITION_UNSPECIFIED)
                return setObject(buf[1], "legendPosition", "BUBBLE_CHART_LEGEND_POSITION_UNSPECIFIED", true, true);
            else if (value == BOTTOM_LEGEND)
                return setObject(buf[1], "legendPosition", "BOTTOM_LEGEND", true, true);
            else if (value == LEFT_LEGEND)
                return setObject(buf[1], "legendPosition", "LEFT_LEGEND", true, true);
            else if (value == RIGHT_LEGEND)
                return setObject(buf[1], "legendPosition", "RIGHT_LEGEND", true, true);
            else if (value == TOP_LEGEND)
                return setObject(buf[1], "legendPosition", "TOP_LEGEND", true, true);
            else if (value == NO_LEGEND)
                return setObject(buf[1], "legendPosition", "NO_LEGEND", true, true);
            else if (value == INSIDE_LEGEND)
                return setObject(buf[1], "legendPosition", "INSIDE_LEGEND", true, true);
            return *this;
        }
        // The data containing the bubble labels. These do not need to be unique.
        BubbleChartSpec &bubbleLabels(const ChartData &value) { return setObject(buf[2], "bubbleLabels", value.c_str(), false, true); }
        // The data containing the bubble x-values. These values locate the bubbles in the chart horizontally.
        BubbleChartSpec &domain(const ChartData &value) { return setObject(buf[3], "domain", value.c_str(), false, true); }
        // The data containing the bubble y-values. These values locate the bubbles in the chart vertically.
        BubbleChartSpec &series(const ChartData &value) { return setObject(buf[4], "series", value.c_str(), false, true); }
        // The data containing the bubble group IDs. All bubbles with the same group ID are drawn in the same color. If bubbleSizes is specified then this field must also be specified but may contain blank values. This field is optional.
        BubbleChartSpec &groupIds(const ChartData &value) { return setObject(buf[5], "groupIds", value.c_str(), false, true); }
        // The data containing the bubble sizes. Bubble sizes are used to draw the bubbles at different sizes relative to each other. If specified, groupIds must also be specified. This field is optional.
        BubbleChartSpec &bubbleSizes(const ChartData &value) { return setObject(buf[6], "bubbleSizes", value.c_str(), false, true); }
        // The opacity of the bubbles between 0 and 1.0. 0 is fully transparent and 1 is fully opaque.
        BubbleChartSpec &bubbleOpacity(double value) { return setObject(buf[7], "bubbleOpacity", String(value), false, true); }
        // The bubble border color. If bubbleBorderColor is also set, this field takes precedence.
        BubbleChartSpec &bubbleBorderColorStyle(const ColorStyle &value) { return setObject(buf[8], "bubbleBorderColorStyle", value.c_str(), false, true); }
        // The max radius size of the bubbles, in pixels. If specified, the field must be a positive value.
        BubbleChartSpec &bubbleMaxRadiusSize(int value) { return setObject(buf[9], "bubbleMaxRadiusSize", String(value), false, true); }
        // The minimum radius size of the bubbles, in pixels. If specific, the field must be a positive value.
        BubbleChartSpec &bubbleMinRadiusSize(int value) { return setObject(buf[10], "bubbleMinRadiusSize", String(value), false, true); }
        // The format of the text inside the bubbles. Strikethrough, underline, and link are not supported.
        BubbleChartSpec &bubbleTextStyle(const TextFormat value) { return setObject(buf[11], "bubbleTextStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif