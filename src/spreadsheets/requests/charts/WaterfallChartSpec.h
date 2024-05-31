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
        TYPE_STACKED,                       //	Values corresponding to the same domain (horizontal axis) value will be stacked vertically.
        TYPE_SEQUENTIAL                     //	Series will spread out along the horizontal axis.
    };

    const struct key_str_40 _WaterfallChartStackedType[WaterfallChartStackedType::TYPE_SEQUENTIAL + 1] PROGMEM = {"WATERFALL_STACKED_TYPE_UNSPECIFIED", "TYPE_STACKED", "TYPE_SEQUENTIAL"};

    /**
     * The domain of a waterfall chart.
     */
    class WaterfallChartDomain : public BaseG4
    {

    public:
        WaterfallChartDomain() {}

        // The data of the WaterfallChartDomain.
        WaterfallChartDomain &data(const ChartData &value) { return wr.set<WaterfallChartDomain &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // True to reverse the order of the domain values (horizontal axis).
        WaterfallChartDomain &reversed(bool value) { return wr.set<WaterfallChartDomain &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Styles for a waterfall chart column.
     */
    class WaterfallChartColumnStyle : public BaseG4
    {

    public:
        WaterfallChartColumnStyle() {}

        // The label of the column's legend.
        WaterfallChartColumnStyle &label(const String &value) { return wr.set<WaterfallChartColumnStyle &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The color of the column. If color is also set, this field takes precedence.
        WaterfallChartColumnStyle &colorStyle(const ColorStyle &value) { return wr.set<WaterfallChartColumnStyle &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A custom subtotal column for a waterfall chart series.
     */
    class WaterfallChartCustomSubtotal : public BaseG4
    {
    public:
        WaterfallChartCustomSubtotal() {}

        // The zero-based index of a data point within the series. If dataIsSubtotal is true, the data point at this index is the subtotal. Otherwise, the subtotal appears after the data point with this index. A series can have multiple subtotals at arbitrary indices, but subtotals do not affect the indices of the data points. For example, if a series has three data points, their indices will always be 0, 1, and 2, regardless of how many subtotals exist on the series or what data points they are associated with., String(value), false, true); }
        // The format of this run. Absent values inherit the cell's format.
        WaterfallChartCustomSubtotal &subtotalIndex(int value) { return wr.set<WaterfallChartCustomSubtotal &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // A label for the subtotal column.
        WaterfallChartCustomSubtotal &label(const String &value) { return wr.set<WaterfallChartCustomSubtotal &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // True if the data point at subtotalIndex is the subtotal. If false, the subtotal will be computed and appear after the data point.
        WaterfallChartCustomSubtotal &dataIsSubtotal(bool value) { return wr.set<WaterfallChartCustomSubtotal &, bool>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * A single series of data for a waterfall chart.
     */
    class WaterfallChartSeries : public BaseG8
    {

    public:
        WaterfallChartSeries() {}

        // The data being visualized in this series.
        WaterfallChartSeries &data(const ChartData &value) { return wr.set<WaterfallChartSeries &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Styles for all columns in this series with positive values.
        WaterfallChartSeries &positiveColumnsStyle(const WaterfallChartColumnStyle &value) { return wr.set<WaterfallChartSeries &, WaterfallChartColumnStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Styles for all columns in this series with negative values.
        WaterfallChartSeries &negativeColumnsStyle(const WaterfallChartColumnStyle &value) { return wr.set<WaterfallChartSeries &, WaterfallChartColumnStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Styles for all subtotal columns in this series.
        WaterfallChartSeries &subtotalColumnsStyle(const WaterfallChartColumnStyle &value) { return wr.set<WaterfallChartSeries &, WaterfallChartColumnStyle>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // True to hide the subtotal column from the end of the series. By default, a subtotal column will appear at the end of each series. Setting this field to true will hide that subtotal column for this series.
        WaterfallChartSeries &hideTrailingSubtotal(bool value) { return wr.set<WaterfallChartSeries &, bool>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // Custom subtotal columns appearing in this series. The order in which subtotals are defined is not significant. Only one subtotal may be defined for each data point.
        WaterfallChartSeries &customSubtotals(const WaterfallChartCustomSubtotal &value) { return wr.append<WaterfallChartSeries &, WaterfallChartCustomSubtotal>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // Information about the data labels for this series.
        WaterfallChartSeries &dataLabel(const DataLabel &value) { return wr.set<WaterfallChartSeries &, DataLabel>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }
    };

    /**
     * A waterfall chart.
     */
    class WaterfallChartSpec : public BaseG8
    {

    public:
        WaterfallChartSpec() {}

        // The domain data (horizontal axis) for the waterfall chart.
        WaterfallChartSpec &domain(const WaterfallChartDomain &value) { return wr.set<WaterfallChartSpec &, WaterfallChartDomain>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The data this waterfall chart is visualizing.
        WaterfallChartSpec &series(const WaterfallChartSeries &value) { return wr.append<WaterfallChartSpec &, WaterfallChartSeries>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The stacked type.
        WaterfallChartSpec &stackedType(WaterfallChartStackedType value) { return wr.set<WaterfallChartSpec &, const char *>(*this, _WaterfallChartStackedType[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // True to interpret the first value as a total.
        WaterfallChartSpec &firstValueIsTotal(bool value) { return wr.set<WaterfallChartSpec &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // True to hide connector lines between columns.
        WaterfallChartSpec &hideConnectorLines(bool value) { return wr.set<WaterfallChartSpec &, bool>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The line style for the connector lines.
        WaterfallChartSpec &connectorLineStyle(const LineStyle &value) { return wr.set<WaterfallChartSpec &, LineStyle>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // Controls whether to display additional data labels on stacked charts which sum the total value of all stacked values at each value along the domain axis. stackedType must be STACKED and neither CUSTOM nor placement can be set on the totalDataLabel.
        WaterfallChartSpec &totalDataLabel(const DataLabel &value) { return wr.set<WaterfallChartSpec &, DataLabel>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }
    };
}

#endif