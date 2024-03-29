#ifndef PIE_CHART_SPEC_H
#define PIE_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"

/**
 * PIE CHART CLASS DEPENDENCIES
 *
 * PieChartSpec         (enum) PieChartLegendPosition
 *                      ChartData*
 *
 * See ChartData.h
 */

namespace GSHEET
{
    // Where the legend of the chart should be positioned.
    enum PieChartLegendPosition
    {
        PIE_CHART_LEGEND_POSITION_UNSPECIFIED, //	Default value, do not use.
        BOTTOM_LEGEND,                         //	The legend is rendered on the bottom of the chart.
        LEFT_LEGEND,                           //	The legend is rendered on the left of the chart.
        RIGHT_LEGEND,                          //	The legend is rendered on the right of the chart.
        TOP_LEGEND,                            //	The legend is rendered on the top of the chart.
        NO_LEGEND,                             //	No legend is rendered.
        LABELED_LEGEND                         //	Each pie slice has a label attached to it.
    };

    /**
     * A pie chart.
     */
    class PieChartSpec : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PieChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PieChartSpec() {}
        // Where the legend of the pie chart should be drawn.
        PieChartSpec &legendPosition(PieChartLegendPosition value)
        {
            if (value == PIE_CHART_LEGEND_POSITION_UNSPECIFIED)
                return setObject(buf[1], "legendPosition", "PIE_CHART_LEGEND_POSITION_UNSPECIFIED", true, true);
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
            else if (value == LABELED_LEGEND)
                return setObject(buf[1], "legendPosition", "LABELED_LEGEND", true, true);
            return *this;
        }
        // The data that covers the domain of the pie chart.
        PieChartSpec &domain(const ChartData &value) { return setObject(buf[2], "domain", value.c_str(), false, true); }
        // The data that covers the one and only series of the pie chart.
        PieChartSpec &series(const ChartData &value) { return setObject(buf[3], "series", value.c_str(), false, true); }
        // True if the pie is three dimensional.
        PieChartSpec &threeDimensional(bool value) { return setObject(buf[4], "threeDimensional", owriter.getBoolStr(value), false, true); }
        // The size of the hole in the pie chart.
        PieChartSpec &pieHole(double value) { return setObject(buf[5], "pieHole", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif