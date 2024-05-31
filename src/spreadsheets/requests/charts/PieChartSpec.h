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

    /**
     * A pie chart.
     */
    class PieChartSpec : public BaseG6
    {

    public:
        PieChartSpec() {}
        
        // Where the legend of the pie chart should be drawn.
        PieChartSpec &legendPosition(PieChart::LegendPosition value) { return wr.set<PieChartSpec &, const char *>(*this, _PieChartLegendPosition[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The data that covers the domain of the pie chart.
        PieChartSpec &domain(const ChartData &value) { return wr.set<PieChartSpec &, ChartData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The data that covers the one and only series of the pie chart.
        PieChartSpec &series(const ChartData &value) { return wr.set<PieChartSpec &, ChartData>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if the pie is three dimensional.
        PieChartSpec &threeDimensional(bool value) { return wr.set<PieChartSpec &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The size of the hole in the pie chart.
        PieChartSpec &pieHole(double value) { return wr.set<PieChartSpec &, double>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
    };
}

#endif