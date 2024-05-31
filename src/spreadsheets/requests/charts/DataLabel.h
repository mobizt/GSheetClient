#ifndef CHART_LABEL_SPEC_H
#define CHART_LABEL_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * DATA LABEL CLASS DEPENDENCIES
 *
 * DataLabel        (enum) DataLabelType
 *                  TextFormat
 *                  (enum) DataLabelPlacement
 *                  ChartData*
 *
 * See ChartData.h
 */

namespace GSHEET
{
    // The type of a data label.
    enum DataLabelType
    {
        DATA_LABEL_TYPE_UNSPECIFIED, //	The data label type is not specified and will be interpreted depending on the context of the data label within the chart.
        NONE_LABEL,                  //	The data label is not displayed.
        DATA,                        //	The data label is displayed using values from the series data.
        CUSTOM_LABEL                 //	The data label is displayed using values from a custom data source indicated by customLabelData.
    };

    // The placement of a data label relative to the labeled data.
    enum DataLabelPlacement
    {
        DATA_LABEL_PLACEMENT_UNSPECIFIED, //	The positioning is determined automatically by the renderer.
        CENTER_LABEL,                     //	Center within a bar or column, both horizontally and vertically.
        LEFT_LABEL,                       //	To the left of a data point.
        RIGHT_LABEL,                      //	To the right of a data point.
        ABOVE,                            //	Above a data point.
        BELOW,                            //	Below a data point.
        INSIDE_END,                       //	Inside a bar or column at the end (top if positive, bottom if negative).
        INSIDE_BASE,                      //	Inside a bar or column at the base.
        OUTSIDE_END                       //	Outside a bar or column at the end.
    };

    const struct key_str_40 _DataLabelType[DataLabelType::CUSTOM_LABEL + 1] PROGMEM = {"DATA_LABEL_TYPE_UNSPECIFIED", "NONE_LABEL", "DATA", "CUSTOM_LABEL"};

    const struct key_str_40 _DataLabelPlacement[DataLabelPlacement::OUTSIDE_END + 1] PROGMEM = {"DATA_LABEL_PLACEMENT_UNSPECIFIED", "CENTER_LABEL", "LEFT_LABEL", "RIGHT_LABEL", "ABOVE", "BELOW", "INSIDE_END", "INSIDE_BASE", "OUTSIDE_END"};

    /**
     * Settings for one set of data labels. Data labels are annotations that appear next to a set of data, such as the points on a line chart, and provide additional information about what the data represents, such as a text representation of the value behind that point on the graph.
     */
    class DataLabel : public BaseG6
    {

    public:
        DataLabel() {}

        // The type of the data label.
        DataLabel &type(DataLabelType value) { return wr.set<DataLabel &, const char *>(*this, _DataLabelType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The text format used for the data label. The link field is not supported.
        DataLabel &textFormat(const TextFormat &value) { return wr.set<DataLabel &, TextFormat>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The placement of the data label relative to the labeled data.
        DataLabel &placement(DataLabelPlacement value) { return wr.set<DataLabel &, const char *>(*this, _DataLabelPlacement[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // Data to use for custom labels. Only used if type is set to CUSTOM. This data must be the same length as the series or other element this data label is applied to. In addition, if the series is split into multiple source ranges, this source data must come from the next column in the source data. For example, if the series is B2:B4,E6:E8 then this data must come from C2:C4,F6:F8.
        DataLabel &customLabelData(const ChartData &value) { return wr.set<DataLabel &, ChartData>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };
}

#endif