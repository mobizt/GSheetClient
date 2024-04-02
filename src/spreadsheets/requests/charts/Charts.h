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

    /**
     * The location an object is overlaid on top of a grid.
     */
    class OverlayPosition : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        OverlayPosition &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        OverlayPosition() {}
        // The cell the object is anchored to.
        OverlayPosition &anchorCell(const GridCoordinate &value) { return setObject(buf[1], "anchorCell", value.c_str(), false, true); }
        // The horizontal offset, in pixels, that the object is offset from the anchor cell.
        OverlayPosition &offsetXPixels(int value) { return setObject(buf[2], "offsetXPixels", String(value), false, true); }
        // The vertical offset, in pixels, that the object is offset from the anchor cell.
        OverlayPosition &offsetYPixels(int value) { return setObject(buf[3], "offsetYPixels", String(value), false, true); }
        // The width of the object, in pixels. Defaults to 600.
        OverlayPosition &widthPixels(int value) { return setObject(buf[4], "widthPixels", String(value), false, true); }
        // The height of the object, in pixels. Defaults to 371.
        OverlayPosition &heightPixels(int value) { return setObject(buf[5], "heightPixels", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Properties of a data source chart.
     */
    class DataSourceChartProperties : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceChartProperties() {}
        // ID of the data source that the chart is associated with.
        DataSourceChartProperties &dataSourceId(const String &value)
        {
            clear();
            jut.addObject(buf, "dataSourceId", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * The specifications of a chart.
     */
    class ChartSpec : public Printable
    {
    private:
        size_t bufSize = 16;
        String buf[16];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ChartSpec() {}
        // The title of the chart.
        ChartSpec &title(const String &value) { return setObject(buf[1], "title", value, true, true); }
        // The alternative text that describes the chart. This is often used for accessibility.
        ChartSpec &altText(const String &value) { return setObject(buf[2], "altText", value, true, true); }
        // The title text format. Strikethrough, underline, and link are not supported.
        ChartSpec &titleTextFormat(const TextFormat &value) { return setObject(buf[3], "titleTextFormat", value.c_str(), false, true); }
        // The title text position. This field is optional.
        ChartSpec &titleTextPosition(const TextPosition &value) { return setObject(buf[4], "titleTextPosition", value.c_str(), false, true); }
        // The subtitle of the chart.
        ChartSpec &subtitle(const String &value) { return setObject(buf[5], "subtitle", value, true, true); }
        // The subtitle text format. Strikethrough, underline, and link are not supported.
        ChartSpec &subtitleTextFormat(const TextFormat &value) { return setObject(buf[6], "subtitleTextFormat", value.c_str(), false, true); }
        // The subtitle text position. This field is optional.
        ChartSpec &subtitleTextPosition(const TextPosition &value) { return setObject(buf[7], "subtitleTextPosition", value.c_str(), false, true); }
        // The name of the font to use by default for all chart text (e.g. title, axis labels, legend). If a font is specified for a specific part of the chart it will override this font name.
        ChartSpec &fontName(const String &value) { return setObject(buf[8], "fontName", value, true, true); }
        // True to make a chart fill the entire space in which it's rendered with minimum padding. False to use the default padding. (Not applicable to Geo and Org charts.)
        ChartSpec &maximized(bool value) { return setObject(buf[9], "maximized", owriter.getBoolStr(value), false, true); }
        // The background color of the entire chart. Not applicable to Org charts. If backgroundColor is also set, this field takes precedence.
        ChartSpec &backgroundColorStyle(const ColorStyle &value) { return setObject(buf[10], "backgroundColorStyle", value.c_str(), false, true); }
        // If present, the field contains data source chart specific properties.
        ChartSpec &dataSourceChartProperties(const DataSourceChartProperties &value) { return setObject(buf[11], "dataSourceChartProperties", value.c_str(), false, true); }
        // The filters applied to the source data of the chart. Only supported for data source charts.
        ChartSpec &addFilterSpecs(const FilterSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[12], FPSTR("filterSpecs"), value.c_str(), false);
            return *this;
        }
        // The order to sort the chart data by. Only a single sort spec is supported. Only supported for data source charts.
        ChartSpec &addSortSpecs(const SortSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[13], FPSTR("sortSpecs"), value.c_str(), false);
            return *this;
        }
        // Determines how the charts will use hidden rows or columns.
        ChartSpec &hiddenDimensionStrategy(ChartHiddenDimensionStrategy value)
        {
            if (value == CHART_HIDDEN_DIMENSION_STRATEGY_UNSPECIFIED)
                return setObject(buf[14], "hiddenDimensionStrategy", "CHART_HIDDEN_DIMENSION_STRATEGY_UNSPECIFIED", true, true);
            else if (value == SKIP_HIDDEN_ROWS_AND_COLUMNS)
                return setObject(buf[14], "hiddenDimensionStrategy", "SKIP_HIDDEN_ROWS_AND_COLUMNS", true, true);
            else if (value == SKIP_HIDDEN_ROWS)
                return setObject(buf[14], "hiddenDimensionStrategy", "SKIP_HIDDEN_ROWS", true, true);
            else if (value == SKIP_HIDDEN_COLUMNS)
                return setObject(buf[14], "hiddenDimensionStrategy", "SKIP_HIDDEN_COLUMNS", true, true);
            else if (value == SHOW_ALL)
                return setObject(buf[14], "hiddenDimensionStrategy", "SHOW_ALL", true, true);
            return *this;
        }
        // Union field chart.
        // A basic chart specification, can be one of many kinds of charts. See BasicChartType for the list of all charts this supports.
        ChartSpec &basicChart(const BasicChartSpec &value) { return setObject(buf[15], "basicChart", value.c_str(), false, true); }
        // Union field chart.
        // A pie chart specification.
        ChartSpec &pieChart(const PieChartSpec &value) { return setObject(buf[15], "pieChart", value.c_str(), false, true); }
        // Union field chart.
        // A bubble chart specification.
        ChartSpec &bubbleChart(const BubbleChartSpec &value) { return setObject(buf[15], "bubbleChart", value.c_str(), false, true); }
        // Union field chart.
        // A candlestick chart specification.
        ChartSpec &candlestickChart(const CandlestickChartSpec &value) { return setObject(buf[15], "candlestickChart", value.c_str(), false, true); }
        // Union field chart.
        // An org chart specification.
        ChartSpec &orgChart(const OrgChartSpec &value) { return setObject(buf[15], "orgChart", value.c_str(), false, true); }
        // Union field chart.
        // A histogram chart specification.
        ChartSpec &histogramChart(const HistogramChartSpec &value) { return setObject(buf[15], "histogramChart", value.c_str(), false, true); }
        // Union field chart.
        // A waterfall chart specification.
        ChartSpec &waterfallChart(const WaterfallChartSpec &value) { return setObject(buf[15], "waterfallChart", value.c_str(), false, true); }
        // Union field chart.
        // A treemap chart specification.
        ChartSpec &treemapChart(const TreemapChartSpec &value) { return setObject(buf[15], "treemapChart", value.c_str(), false, true); }
        // Union field chart.
        // A scorecard chart specification.
        ChartSpec &scorecardChart(const ScorecardChartSpec &value) { return setObject(buf[15], "scorecardChart", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The position of an embedded object such as a chart.
     */
    class EmbeddedObjectPosition : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;
        GSheetObjectWriter owriter;

    public:
        EmbeddedObjectPosition() {}
        // Union field location.
        // The sheet this is on. Set only if the embedded object is on its own sheet. Must be non-negative.
        EmbeddedObjectPosition &sheetId(int value)
        {
            clear();
            jut.addObject(buf, "sheetId", String(value), false, true);
            return *this;
        }
        // Union field location.
        // The position at which the object is overlaid on top of a grid.
        EmbeddedObjectPosition &overlayPosition(const OverlayPosition &value)
        {
            clear();
            jut.addObject(buf, "overlayPosition", value.c_str(), false, true);
            return *this;
        }
        // Union field location.
        // If true, the embedded object is put on a new sheet whose ID is chosen for you. Used only when writing.
        EmbeddedObjectPosition &newSheet(bool value)
        {
            clear();
            jut.addObject(buf, "newSheet", owriter.getBoolStr(value), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * A border along an embedded object.
     */
    class EmbeddedObjectBorder : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;
        GSheetObjectWriter owriter;

    public:
        EmbeddedObjectBorder() {}
        // The color of the border. If color is also set, this field takes precedence.
        EmbeddedObjectBorder &colorStyle(const ColorStyle &value)
        {
            clear();
            jut.addObject(buf, "colorStyle", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * A chart embedded in a sheet.
     */
    class EmbeddedChart : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        EmbeddedChart &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        EmbeddedChart() {}
        // The ID of the chart.
        EmbeddedChart &chartId(int value) { return setObject(buf[1], "chartId", String(value), false, true); }
        // The specification of the chart.
        EmbeddedChart &spec(const ChartSpec &value) { return setObject(buf[2], "spec", value.c_str(), false, true); }
        // The position of the chart.
        EmbeddedChart &position(const EmbeddedObjectPosition &value) { return setObject(buf[3], "position", value.c_str(), false, true); }
        // The border of the chart.
        EmbeddedChart &border(const EmbeddedObjectBorder &value) { return setObject(buf[4], "border", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif