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
    class CandlestickDomain : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CandlestickDomain &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CandlestickDomain() {}
        // The data of the CandlestickDomain.
        CandlestickDomain &data(const ChartData &value) { return setObject(buf[1], "data", value.c_str(), false, true); }
        // True to reverse the order of the domain values (horizontal axis).
        CandlestickDomain &reversed(bool value) { return setObject(buf[2], "reversed", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The series of a CandlestickData.
     */
    class CandlestickSeries : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        CandlestickSeries() {}
        // The data of the CandlestickSeries.
        CandlestickSeries &data(const ChartData &value)
        {
            clear();
            jut.addObject(buf, "data", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * The Candlestick chart data, each containing the low, open, close, and high values for a series.
     */
    class CandlestickData : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CandlestickData &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CandlestickData() {}
        // The range data (vertical axis) for the low/minimum value for each candle. This is the bottom of the candle's center line.
        CandlestickData &lowSeries(const CandlestickSeries &value) { return setObject(buf[1], "lowSeries", value.c_str(), false, true); }
        // The range data (vertical axis) for the open/initial value for each candle. This is the bottom of the candle body. If less than the close value the candle will be filled. Otherwise the candle will be hollow.
        CandlestickData &openSeries(const CandlestickSeries &value) { return setObject(buf[2], "openSeries", value.c_str(), false, true); }
        // The range data (vertical axis) for the close/final value for each candle. This is the top of the candle body. If greater than the open value the candle will be filled. Otherwise the candle will be hollow.
        CandlestickData &closeSeries(const CandlestickSeries &value) { return setObject(buf[3], "closeSeries", value.c_str(), false, true); }
        // The range data (vertical axis) for the high/maximum value for each candle. This is the top of the candle's center line.
        CandlestickData &highSeries(const CandlestickSeries &value) { return setObject(buf[4], "highSeries", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A candlestick chart.
     */
    class CandlestickChartSpec : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CandlestickChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CandlestickChartSpec() {}
        // The domain data (horizontal axis) for the candlestick chart. String data will be treated as discrete labels, other data will be treated as continuous values.
        CandlestickChartSpec &domain(const CandlestickDomain &value) { return setObject(buf[1], "domain", value.c_str(), false, true); }
        // The Candlestick chart data. Only one CandlestickData is supported.
        CandlestickChartSpec &data(const CandlestickData &value) { return setObject(buf[2], "data", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif