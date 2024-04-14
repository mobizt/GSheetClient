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
    class TreemapChartColorScale : public BaseG6
    {

    public:
        TreemapChartColorScale() = default;

        // The background color for cells with a color value less than or equal to minValue. Defaults to #dc3912 if not specified. If minValueColor is also set, this field takes precedence.
        TreemapChartColorScale &minValueColorStyle(const ColorStyle &value) { return wr.set<TreemapChartColorScale &, ColorStyle>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The background color for cells with a color value at the midpoint between minValue and maxValue. Defaults to #efe6dc if not specified. If midValueColor is also set, this field takes precedence.
        TreemapChartColorScale &midValueColorStyle(const ColorStyle &value) { return wr.set<TreemapChartColorScale &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The background color for cells with a color value greater than or equal to maxValue. Defaults to #109618 if not specified. If maxValueColor is also set, this field takes precedence.
        TreemapChartColorScale &maxValueColorStyle(const ColorStyle &value) { return wr.set<TreemapChartColorScale &, ColorStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The background color for cells with a color value greater than or equal to maxValue. Defaults to #109618 if not specified. If maxValueColor is also set, this field takes precedence.
        TreemapChartColorScale &noDataColorStyle(const ColorStyle &value) { return wr.set<TreemapChartColorScale &, ColorStyle>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A Treemap chart.
     */
    class TreemapChartSpec : public BaseG16
    {

    public:
        TreemapChartSpec() = default;

        // The data that contains the treemap cell labels.
        TreemapChartSpec &labels(const ChartData &value) { return wr.set<TreemapChartSpec &, ChartData>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The data the contains the treemap cells' parent labels.
        TreemapChartSpec &parentLabels(const ChartData &value) { return wr.set<TreemapChartSpec &, ChartData>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The data that determines the size of each treemap data cell. This data is expected to be numeric. The cells corresponding to non-numeric or missing data will not be rendered. If colorData is not specified, this data is used to determine data cell background colors as well.
        TreemapChartSpec &sizeData(const ChartData &value) { return wr.set<TreemapChartSpec &, ChartData>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The data that determines the background color of each treemap data cell. This field is optional. If not specified, sizeData is used to determine background colors. If specified, the data is expected to be numeric. colorScale will determine how the values in this data map to data cell background colors.
        TreemapChartSpec &colorData(const ChartData &value) { return wr.set<TreemapChartSpec &, ChartData>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The text format for all labels on the chart. The link field is not supported.
        TreemapChartSpec &textFormat(const TextFormat &value) { return wr.set<TreemapChartSpec &, TextFormat>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The number of data levels to show on the treemap chart. These levels are interactive and are shown with their labels. Defaults to 2 if not specified.
        TreemapChartSpec &levels(int value) { return wr.set<TreemapChartSpec &, int>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // The number of additional data levels beyond the labeled levels to be shown on the treemap chart. These levels are not interactive and are shown without their labels. Defaults to 0 if not specified.
        TreemapChartSpec &hintedLevels(int value) { return wr.set<TreemapChartSpec &, int>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // The minimum possible data value. Cells with values less than this will have the same color as cells with this value. If not specified, defaults to the actual minimum value from colorData, or the minimum value from sizeData if colorData is not specified.
        TreemapChartSpec &minValue(double value) { return wr.set<TreemapChartSpec &, double>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }

        // The maximum possible data value. Cells with values greater than this will have the same color as cells with this value. If not specified, defaults to the actual maximum value from colorData, or the maximum value from sizeData if colorData is not specified.
        TreemapChartSpec &maxValue(double value) { return wr.set<TreemapChartSpec &, double>(*this, value, buf, bufSize, 9, FPSTR(__func__)); }

        // The background color for header cells. If headerColor is also set, this field takes precedence.
        TreemapChartSpec &headerColorStyle(const ColorStyle &value) { return wr.set<TreemapChartSpec &, ColorStyle>(*this, value, buf, bufSize, 10, FPSTR(__func__)); }

        // The color scale for data cells in the treemap chart. Data cells are assigned colors based on their color values. These color values come from colorData, or from sizeData if colorData is not specified. Cells with color values less than or equal to minValue will have minValueColor as their background color. Cells with color values greater than or equal to maxValue will have maxValueColor as their background color. Cells with color values between minValue and maxValue will have background colors on a gradient between minValueColor and maxValueColor, the midpoint of the gradient being midValueColor. Cells with missing or non-numeric color values will have noDataColor as their background color.
        TreemapChartSpec &colorScale(const TreemapChartColorScale &value) { return wr.set<TreemapChartSpec &, TreemapChartColorScale>(*this, value, buf, bufSize, 11, FPSTR(__func__)); }

        // True to hide tooltips.
        TreemapChartSpec &hideTooltips(bool value) { return wr.set<TreemapChartSpec &, bool>(*this, value, buf, bufSize, 12, FPSTR(__func__)); }
    };
}

#endif