#ifndef CANDLESTICK_CHART_SPEC_H
#define CANDLESTICK_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"

/**
 * CANDLESTICK CHART CLASS DEPENDENCIES
 *
 * CandlestickChartSpec             (enum) CandlestickDomain
 *                                  CandlestickData             CandlestickSeries       ChartData*
 *
 * See ChartData.h
 */

namespace GSHEET
{
    /**
     * The domain of a CandlestickChart.
     */
    class CandlestickDomain : public BaseG4
    {
    public:
        CandlestickDomain() = default;

        // The data of the CandlestickDomain.
        CandlestickDomain &data(const ChartData &value) { return wr.set<CandlestickDomain &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // True to reverse the order of the domain values (horizontal axis).
        CandlestickDomain &reversed(bool value) { return wr.set<CandlestickDomain &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * The series of a CandlestickData.
     */
    class CandlestickSeries : public BaseG1
    {

    public:
        CandlestickSeries() = default;

        // The data of the CandlestickSeries.
        CandlestickSeries &data(const ChartData &value) { return wr.add<CandlestickSeries &, ChartData>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * The Candlestick chart data, each containing the low, open, close, and high values for a series.
     */
    class CandlestickData : public BaseG6
    {

    public:
        CandlestickData() = default;

        // The range data (vertical axis) for the low/minimum value for each candle. This is the bottom of the candle's center line.
        CandlestickData &lowSeries(const CandlestickSeries &value) { return wr.set<CandlestickData &, CandlestickSeries>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The range data (vertical axis) for the open/initial value for each candle. This is the bottom of the candle body. If less than the close value the candle will be filled. Otherwise the candle will be hollow.
        CandlestickData &openSeries(const CandlestickSeries &value) { return wr.set<CandlestickData &, CandlestickSeries>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The range data (vertical axis) for the close/final value for each candle. This is the top of the candle body. If greater than the open value the candle will be filled. Otherwise the candle will be hollow.
        CandlestickData &closeSeries(const CandlestickSeries &value) { return wr.set<CandlestickData &, CandlestickSeries>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The range data (vertical axis) for the high/maximum value for each candle. This is the top of the candle's center line.
        CandlestickData &highSeries(const CandlestickSeries &value) { return wr.set<CandlestickData &, CandlestickSeries>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A candlestick chart.
     */
    class CandlestickChartSpec : public BaseG4
    {

    public:
        CandlestickChartSpec() = default;

        // The domain data (horizontal axis) for the candlestick chart. String data will be treated as discrete labels, other data will be treated as continuous values.
        CandlestickChartSpec &domain(const CandlestickDomain &value) { return wr.set<CandlestickChartSpec &, CandlestickDomain>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The Candlestick chart data. Only one CandlestickData is supported.
        CandlestickChartSpec &data(const CandlestickData &value) { return wr.set<CandlestickChartSpec &, CandlestickData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };
}

#endif