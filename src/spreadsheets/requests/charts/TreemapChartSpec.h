#ifndef TREEMAP_CHART_SPEC_H
#define TREEMAP_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * TREEMAP CHART CLASS DEPENDENCIES
 *
 * TreemapChartSpec         ChartData*
 *                          TextFormat
 *                          ColorStyle*
 *                          TreemapChartColorScale      ColorStyle*
 *
 * See ChartData.h
 */
namespace GSHEET
{
    /**
     * A color scale for a treemap chart.
     */
    class TreemapChartColorScale : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        TreemapChartColorScale &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        TreemapChartColorScale() {}
        // The background color for cells with a color value less than or equal to minValue. Defaults to #dc3912 if not specified. If minValueColor is also set, this field takes precedence.
        TreemapChartColorScale &minValueColorStyle(const ColorStyle &value) { return setObject(buf[1], "minValueColorStyle", value.c_str(), false, true); }
        // The background color for cells with a color value at the midpoint between minValue and maxValue. Defaults to #efe6dc if not specified. If midValueColor is also set, this field takes precedence.
        TreemapChartColorScale &midValueColorStyle(const ColorStyle &value) { return setObject(buf[2], "midValueColorStyle", value.c_str(), false, true); }
        // The background color for cells with a color value greater than or equal to maxValue. Defaults to #109618 if not specified. If maxValueColor is also set, this field takes precedence.
        TreemapChartColorScale &maxValueColorStyle(const ColorStyle &value) { return setObject(buf[3], "maxValueColorStyle", value.c_str(), false, true); }
        // The background color for cells with a color value greater than or equal to maxValue. Defaults to #109618 if not specified. If maxValueColor is also set, this field takes precedence.
        TreemapChartColorScale &noDataColorStyle(const ColorStyle &value) { return setObject(buf[4], "noDataColorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A Treemap chart.
     */
    class TreemapChartSpec : public Printable
    {
    private:
        size_t bufSize = 13;
        String buf[13];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        TreemapChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        TreemapChartSpec() {}
        // The data that contains the treemap cell labels.
        TreemapChartSpec &labels(const ChartData &value) { return setObject(buf[1], "labels", value.c_str(), false, true); }
        // The data the contains the treemap cells' parent labels.
        TreemapChartSpec &parentLabels(const ChartData &value) { return setObject(buf[2], "parentLabels", value.c_str(), false, true); }
        // The data that determines the size of each treemap data cell. This data is expected to be numeric. The cells corresponding to non-numeric or missing data will not be rendered. If colorData is not specified, this data is used to determine data cell background colors as well.
        TreemapChartSpec &sizeData(const ChartData &value) { return setObject(buf[3], "sizeData", value.c_str(), false, true); }
        // The data that determines the background color of each treemap data cell. This field is optional. If not specified, sizeData is used to determine background colors. If specified, the data is expected to be numeric. colorScale will determine how the values in this data map to data cell background colors.
        TreemapChartSpec &colorData(const ChartData &value) { return setObject(buf[4], "colorData", value.c_str(), false, true); }
        // The text format for all labels on the chart. The link field is not supported.
        TreemapChartSpec &textFormat(const TextFormat &value) { return setObject(buf[5], "textFormat", value.c_str(), false, true); }
        // The number of data levels to show on the treemap chart. These levels are interactive and are shown with their labels. Defaults to 2 if not specified.
        TreemapChartSpec &levels(int value) { return setObject(buf[6], "levels", String(value), false, true); }
        // The number of additional data levels beyond the labeled levels to be shown on the treemap chart. These levels are not interactive and are shown without their labels. Defaults to 0 if not specified.
        TreemapChartSpec &hintedLevels(int value) { return setObject(buf[7], "hintedLevels", String(value), false, true); }
        // The minimum possible data value. Cells with values less than this will have the same color as cells with this value. If not specified, defaults to the actual minimum value from colorData, or the minimum value from sizeData if colorData is not specified.
        TreemapChartSpec &minValue(double value) { return setObject(buf[8], "minValue", String(value), false, true); }
        // The maximum possible data value. Cells with values greater than this will have the same color as cells with this value. If not specified, defaults to the actual maximum value from colorData, or the maximum value from sizeData if colorData is not specified.
        TreemapChartSpec &maxValue(double value) { return setObject(buf[9], "maxValue", String(value), false, true); }
        // The background color for header cells. If headerColor is also set, this field takes precedence.
        TreemapChartSpec &headerColorStyle(const ColorStyle &value) { return setObject(buf[10], "headerColorStyle", value.c_str(), false, true); }
        // The color scale for data cells in the treemap chart. Data cells are assigned colors based on their color values. These color values come from colorData, or from sizeData if colorData is not specified. Cells with color values less than or equal to minValue will have minValueColor as their background color. Cells with color values greater than or equal to maxValue will have maxValueColor as their background color. Cells with color values between minValue and maxValue will have background colors on a gradient between minValueColor and maxValueColor, the midpoint of the gradient being midValueColor. Cells with missing or non-numeric color values will have noDataColor as their background color.
        TreemapChartSpec &colorScale(const TreemapChartColorScale &value) { return setObject(buf[11], "colorScale", value.c_str(), false, true); }
        // True to hide tooltips.
        TreemapChartSpec &hideTooltips(bool value) { return setObject(buf[12], "hideTooltips", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif