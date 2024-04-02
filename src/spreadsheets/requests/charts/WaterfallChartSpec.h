#ifndef WATERFALL_CHART_SPEC_H
#define WATERFALL_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/charts/DataLabel.h"
#include "./spreadsheets/requests/charts/LineStyle.h"

/**
 * WATERFALL CHART CLASS DEPENDENCIES
 *
 * WaterfallChartSpec       WaterfallChartDomain                    ChartData*
 *
 *                          WaterfallChartSeries                    ChartData*
 *                                                                  WaterfallChartColumnStyle           ColorStyle*
 *                                                                  WaterfallChartCustomSubtotal
 *                                                                  DataLabel*
 *
 *                          (enum) WaterfallChartStackedType
 *                          LineStyle
 *                          DataLabel*
 *
 *
 * See ChartData.h, Theme.h
 */

namespace GSHEET
{
    // Stacked type options for waterfall charts.
    enum WaterfallChartStackedType
    {
        WATERFALL_STACKED_TYPE_UNSPECIFIED, //	Default value, do not use.
        TYPE_STACKED,                            //	Values corresponding to the same domain (horizontal axis) value will be stacked vertically.
        TYPE_SEQUENTIAL                          //	Series will spread out along the horizontal axis.
    };

    /**
     * The domain of a waterfall chart.
     */
    class WaterfallChartDomain : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        WaterfallChartDomain &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        WaterfallChartDomain() {}
        // The data of the WaterfallChartDomain.
        WaterfallChartDomain &data(const ChartData &value) { return setObject(buf[1], "data", value.c_str(), false, true); }
        // True to reverse the order of the domain values (horizontal axis).
        WaterfallChartDomain &reversed(bool value) { return setObject(buf[2], "reversed", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Styles for a waterfall chart column.
     */
    class WaterfallChartColumnStyle : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        WaterfallChartColumnStyle &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        WaterfallChartColumnStyle() {}
        // The label of the column's legend.
        WaterfallChartColumnStyle &label(const String &value) { return setObject(buf[1], "label", value, true, true); }
        // The color of the column. If color is also set, this field takes precedence.
        WaterfallChartColumnStyle &colorStyle(const ColorStyle &value) { return setObject(buf[2], "colorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A custom subtotal column for a waterfall chart series.
     */
    class WaterfallChartCustomSubtotal : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        WaterfallChartCustomSubtotal &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        WaterfallChartCustomSubtotal() {}
        // The zero-based index of a data point within the series. If dataIsSubtotal is true, the data point at this index is the subtotal. Otherwise, the subtotal appears after the data point with this index. A series can have multiple subtotals at arbitrary indices, but subtotals do not affect the indices of the data points. For example, if a series has three data points, their indices will always be 0, 1, and 2, regardless of how many subtotals exist on the series or what data points they are associated with., String(value), false, true); }
        // The format of this run. Absent values inherit the cell's format.
        WaterfallChartCustomSubtotal &subtotalIndex(int value) { return setObject(buf[1], "subtotalIndex", String(value), false, true); }
        // A label for the subtotal column.
        WaterfallChartCustomSubtotal &label(const String &value) { return setObject(buf[2], "label", value.c_str(), false, true); }
        // True if the data point at subtotalIndex is the subtotal. If false, the subtotal will be computed and appear after the data point.
        WaterfallChartCustomSubtotal &dataIsSubtotal(bool value) { return setObject(buf[3], "dataIsSubtotal", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A single series of data for a waterfall chart.
     */
    class WaterfallChartSeries : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        WaterfallChartSeries &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        WaterfallChartSeries() {}
        // The data being visualized in this series.
        WaterfallChartSeries &data(const ChartData &value) { return setObject(buf[1], "data", value.c_str(), false, true); }
        // Styles for all columns in this series with positive values.
        WaterfallChartSeries &positiveColumnsStyle(const WaterfallChartColumnStyle &value) { return setObject(buf[2], "positiveColumnsStyle", value.c_str(), false, true); }
        // Styles for all columns in this series with negative values.
        WaterfallChartSeries &negativeColumnsStyle(const WaterfallChartColumnStyle &value) { return setObject(buf[3], "negativeColumnsStyle", value.c_str(), false, true); }
        // Styles for all subtotal columns in this series.
        WaterfallChartSeries &subtotalColumnsStyle(const WaterfallChartColumnStyle &value) { return setObject(buf[4], "subtotalColumnsStyle", value.c_str(), false, true); }
        // True to hide the subtotal column from the end of the series. By default, a subtotal column will appear at the end of each series. Setting this field to true will hide that subtotal column for this series.
        WaterfallChartSeries &hideTrailingSubtotal(bool value) { return setObject(buf[5], "hideTrailingSubtotal", owriter.getBoolStr(value), false, true); }
        // Custom subtotal columns appearing in this series. The order in which subtotals are defined is not significant. Only one subtotal may be defined for each data point.
        WaterfallChartSeries &addCustomSubtotals(const WaterfallChartCustomSubtotal &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[6], FPSTR("customSubtotals"), value.c_str(), false);
            return *this;
        }
        // Information about the data labels for this series.
        WaterfallChartSeries &dataLabel(const DataLabel &value) { return setObject(buf[7], "dataLabel", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A waterfall chart.
     */
    class WaterfallChartSpec : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        WaterfallChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        WaterfallChartSpec() {}
        // The domain data (horizontal axis) for the waterfall chart.
        WaterfallChartSpec &domain(const WaterfallChartDomain &value) { return setObject(buf[1], "domain", value.c_str(), false, true); }
        // The data this waterfall chart is visualizing.
        WaterfallChartSpec &addSeries(const WaterfallChartSeries &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("series"), value.c_str(), false);
            return *this;
        }
        // The stacked type.
        WaterfallChartSpec &stackedType(WaterfallChartStackedType value)
        {
            if (value == WATERFALL_STACKED_TYPE_UNSPECIFIED)
                return setObject(buf[3], "stackedType", "WATERFALL_STACKED_TYPE_UNSPECIFIED", true, true);
            else if (value == TYPE_STACKED)
                return setObject(buf[3], "stackedType", "STACKED", true, true);
            else if (value == TYPE_SEQUENTIAL)
                return setObject(buf[3], "stackedType", "SEQUENTIAL", true, true);
            return *this;
        }
        // True to interpret the first value as a total.
        WaterfallChartSpec &firstValueIsTotal(bool value) { return setObject(buf[4], "firstValueIsTotal", owriter.getBoolStr(value), false, true); }
        // True to hide connector lines between columns.
        WaterfallChartSpec &hideConnectorLines(bool value) { return setObject(buf[5], "hideConnectorLines", owriter.getBoolStr(value), false, true); }
        // The line style for the connector lines.
        WaterfallChartSpec &connectorLineStyle(const LineStyle &value) { return setObject(buf[6], "connectorLineStyle", value.c_str(), false, true); }
        // Controls whether to display additional data labels on stacked charts which sum the total value of all stacked values at each value along the domain axis. stackedType must be STACKED and neither CUSTOM nor placement can be set on the totalDataLabel.
        WaterfallChartSpec &totalDataLabel(const DataLabel &value) { return setObject(buf[7], "totalDataLabel", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif