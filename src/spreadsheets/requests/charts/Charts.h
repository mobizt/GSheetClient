#ifndef CHARTS_H
#define CHARTS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/CellFormat.h"
#include "./spreadsheets/requests/FilterSpec.h"
#include "./spreadsheets/requests/SortSpec.h"
#include "./spreadsheets/requests/charts/BasicChartSpec.h"
#include "./spreadsheets/requests/charts/BubbleChartSpec.h"
#include "./spreadsheets/requests/charts/CandlestickChartSpec.h"
#include "./spreadsheets/requests/charts/HistogramChartSpec.h"
#include "./spreadsheets/requests/charts/OrgChartSpec.h"
#include "./spreadsheets/requests/charts/PieChartSpec.h"
#include "./spreadsheets/requests/charts/ScorecardChartSpec.h"
#include "./spreadsheets/requests/charts/TreemapChartSpec.h"
#include "./spreadsheets/requests/charts/WaterfallChartSpec.h"
#include "./spreadsheets/requests/GridCoordinate.h"

namespace GSHEET
{
    // Determines how charts should handle source rows that are hidden. Hidden rows include both manually hidden and hidden by a filter.
    enum ChartHiddenDimensionStrategy
    {
        CHART_HIDDEN_DIMENSION_STRATEGY_UNSPECIFIED, //	Default value, do not use.
        SKIP_HIDDEN_ROWS_AND_COLUMNS,                //	Charts will skip hidden rows and columns.
        SKIP_HIDDEN_ROWS,                            //	Charts will skip hidden rows only.
        SKIP_HIDDEN_COLUMNS,                         //	Charts will skip hidden columns only.
        SHOW_ALL                                     //	Charts will not skip any hidden rows or columns.
    };

    const struct key_str_60 _ChartHiddenDimensionStrategy[ChartHiddenDimensionStrategy::SHOW_ALL + 1] PROGMEM = {"CHART_HIDDEN_DIMENSION_STRATEGY_UNSPECIFIED", "SKIP_HIDDEN_ROWS_AND_COLUMNS", "SKIP_HIDDEN_ROWS", "SKIP_HIDDEN_COLUMNS", "SHOW_ALL"};

    /**
     * The location an object is overlaid on top of a grid.
     */
    class OverlayPosition : public BaseG6
    {

    public:
        OverlayPosition() = default;

        // The cell the object is anchored to.
        OverlayPosition &anchorCell(const GridCoordinate &value) { return wr.set<OverlayPosition &, GridCoordinate>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The horizontal offset, in pixels, that the object is offset from the anchor cell.
        OverlayPosition &offsetXPixels(int value) { return wr.set<OverlayPosition &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The vertical offset, in pixels, that the object is offset from the anchor cell.
        OverlayPosition &offsetYPixels(int value) { return wr.set<OverlayPosition &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The width of the object, in pixels. Defaults to 600.
        OverlayPosition &widthPixels(int value) { return wr.set<OverlayPosition &, int>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The height of the object, in pixels. Defaults to 371.
        OverlayPosition &heightPixels(int value) { return wr.set<OverlayPosition &, int>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
    };

    /**
     * Properties of a data source chart.
     */
    class DataSourceChartProperties : public BaseG1
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceChartProperties() = default;

        // ID of the data source that the chart is associated with.
        DataSourceChartProperties &dataSourceId(const String &value) { return wr.add<DataSourceChartProperties &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * The specifications of a chart.
     */
    class ChartSpec : public BaseG16
    {

    public:
        ChartSpec() = default;

        // The title of the chart.
        ChartSpec &title(const String &value) { return wr.set<ChartSpec &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The alternative text that describes the chart. This is often used for accessibility.
        ChartSpec &altText(const String &value) { return wr.set<ChartSpec &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The title text format. Strikethrough, underline, and link are not supported.
        ChartSpec &titleTextFormat(const TextFormat &value) { return wr.set<ChartSpec &, TextFormat>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The title text position. This field is optional.
        ChartSpec &titleTextPosition(const TextPosition &value) { return wr.set<ChartSpec &, TextPosition>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The subtitle of the chart.
        ChartSpec &subtitle(const String &value) { return wr.set<ChartSpec &, String>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The subtitle text format. Strikethrough, underline, and link are not supported.
        ChartSpec &subtitleTextFormat(const TextFormat &value) { return wr.set<ChartSpec &, TextFormat>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The subtitle text position. This field is optional.
        ChartSpec &subtitleTextPosition(const TextPosition &value) { return wr.set<ChartSpec &, TextPosition>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // The name of the font to use by default for all chart text (e.g. title, axis labels, legend). If a font is specified for a specific part of the chart it will override this font name.
        ChartSpec &fontName(const String &value) { return wr.set<ChartSpec &, String>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }

        // True to make a chart fill the entire space in which it's rendered with minimum padding. False to use the default padding. (Not applicable to Geo and Org charts.)
        ChartSpec &maximized(bool value) { return wr.set<ChartSpec &, bool>(*this, value, buf, bufSize, 9, FPSTR(__func__)); }

        // The background color of the entire chart. Not applicable to Org charts. If backgroundColor is also set, this field takes precedence.
        ChartSpec &backgroundColorStyle(const ColorStyle &value) { return wr.set<ChartSpec &, ColorStyle>(*this, value, buf, bufSize, 10, FPSTR(__func__)); }

        // If present, the field contains data source chart specific properties.
        ChartSpec &dataSourceChartProperties(const DataSourceChartProperties &value) { return wr.set<ChartSpec &, DataSourceChartProperties>(*this, value, buf, bufSize, 11, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The filters applied to the source data of the chart. Only supported for data source charts.
        ChartSpec &filterSpecs(const FilterSpec &value) { return wr.append<ChartSpec &, FilterSpec>(*this, value, buf, bufSize, 12, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The order to sort the chart data by. Only a single sort spec is supported. Only supported for data source charts.
        ChartSpec &sortSpecs(const SortSpec &value) { return wr.append<ChartSpec &, SortSpec>(*this, value, buf, bufSize, 13, FPSTR(__func__)); }

        // Determines how the charts will use hidden rows or columns.
        ChartSpec &hiddenDimensionStrategy(ChartHiddenDimensionStrategy value) { return wr.set<ChartSpec &, const char *>(*this, _ChartHiddenDimensionStrategy[value].text, buf, bufSize, 14, FPSTR(__func__)); }

        // Union field chart.
        // A basic chart specification, can be one of many kinds of charts. See BasicChartType for the list of all charts this supports.
        ChartSpec &basicChart(const BasicChartSpec &value) { return wr.set<ChartSpec &, BasicChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A pie chart specification.
        ChartSpec &pieChart(const PieChartSpec &value) { return wr.set<ChartSpec &, PieChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A bubble chart specification.
        ChartSpec &bubbleChart(const BubbleChartSpec &value) { return wr.set<ChartSpec &, BubbleChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A candlestick chart specification.
        ChartSpec &candlestickChart(const CandlestickChartSpec &value) { return wr.set<ChartSpec &, CandlestickChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // An org chart specification.
        ChartSpec &orgChart(const OrgChartSpec &value) { return wr.set<ChartSpec &, OrgChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A histogram chart specification.
        ChartSpec &histogramChart(const HistogramChartSpec &value) { return wr.set<ChartSpec &, HistogramChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A waterfall chart specification.
        ChartSpec &waterfallChart(const WaterfallChartSpec &value) { return wr.set<ChartSpec &, WaterfallChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A treemap chart specification.
        ChartSpec &treemapChart(const TreemapChartSpec &value) { return wr.set<ChartSpec &, TreemapChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }

        // Union field chart.
        // A scorecard chart specification.
        ChartSpec &scorecardChart(const ScorecardChartSpec &value) { return wr.set<ChartSpec &, ScorecardChartSpec>(*this, value, buf, bufSize, 15, FPSTR(__func__)); }
    };

    /**
     * The position of an embedded object such as a chart.
     */
    class EmbeddedObjectPosition : public BaseG1
    {

    public:
        EmbeddedObjectPosition() = default;

        // Union field location.
        // The sheet this is on. Set only if the embedded object is on its own sheet. Must be non-negative.
        EmbeddedObjectPosition &sheetId(int value) { return wr.add<EmbeddedObjectPosition &, int>(*this, value, buf, FPSTR(__func__)); }

        // Union field location.
        // The position at which the object is overlaid on top of a grid.
        EmbeddedObjectPosition &overlayPosition(const OverlayPosition &value) { return wr.add<EmbeddedObjectPosition &, OverlayPosition>(*this, value, buf, FPSTR(__func__)); }

        // Union field location.
        // If true, the embedded object is put on a new sheet whose ID is chosen for you. Used only when writing.
        EmbeddedObjectPosition &newSheet(bool value) { return wr.add<EmbeddedObjectPosition &, bool>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A border along an embedded object.
     */
    class EmbeddedObjectBorder : public BaseG1
    {

    public:
        EmbeddedObjectBorder() = default;

        // The color of the border. If color is also set, this field takes precedence.
        EmbeddedObjectBorder &colorStyle(const ColorStyle &value) { return wr.add<EmbeddedObjectBorder &, ColorStyle>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A chart embedded in a sheet.
     */
    class EmbeddedChart : public BaseG6
    {
    public:
        EmbeddedChart() = default;

        // The ID of the chart.
        EmbeddedChart &chartId(int value) { return wr.set<EmbeddedChart &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The specification of the chart.
        EmbeddedChart &spec(const ChartSpec &value) { return wr.set<EmbeddedChart &, ChartSpec>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The position of the chart.
        EmbeddedChart &position(const EmbeddedObjectPosition &value) { return wr.set<EmbeddedChart &, EmbeddedObjectPosition>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The border of the chart.
        EmbeddedChart &border(const EmbeddedObjectBorder &value) { return wr.set<EmbeddedChart &, EmbeddedObjectBorder>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };
}

#endif