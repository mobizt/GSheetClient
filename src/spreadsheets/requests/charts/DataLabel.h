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
        NONE_LABEL,                        //	The data label is not displayed.
        DATA,                        //	The data label is displayed using values from the series data.
        CUSTOM_LABEL                       //	The data label is displayed using values from a custom data source indicated by customLabelData.
    };

    // The placement of a data label relative to the labeled data.
    enum DataLabelPlacement
    {
        DATA_LABEL_PLACEMENT_UNSPECIFIED, //	The positioning is determined automatically by the renderer.
        CENTER_LABEL,                           //	Center within a bar or column, both horizontally and vertically.
        LEFT_LABEL,                             //	To the left of a data point.
        RIGHT_LABEL,                            //	To the right of a data point.
        ABOVE,                            //	Above a data point.
        BELOW,                            //	Below a data point.
        INSIDE_END,                       //	Inside a bar or column at the end (top if positive, bottom if negative).
        INSIDE_BASE,                      //	Inside a bar or column at the base.
        OUTSIDE_END                       //	Outside a bar or column at the end.
    };

    /**
     * Settings for one set of data labels. Data labels are annotations that appear next to a set of data, such as the points on a line chart, and provide additional information about what the data represents, such as a text representation of the value behind that point on the graph.
     */
    class DataLabel : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataLabel &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataLabel() {}
        // The type of the data label.
        DataLabel &type(DataLabelType value)
        {
            if (value == DATA_LABEL_TYPE_UNSPECIFIED)
                return setObject(buf[1], "type", "DATA_LABEL_TYPE_UNSPECIFIED", true, true);
            else if (value == NONE_LABEL)
                return setObject(buf[1], "type", "NONE", true, true);
            else if (value == DATA)
                return setObject(buf[1], "type", "DATA", true, true);
            else if (value == CUSTOM_LABEL)
                return setObject(buf[1], "type", "CUSTOM", true, true);
            return *this;
        }
        // The text format used for the data label. The link field is not supported.
        DataLabel &textFormat(const TextFormat &value) { return setObject(buf[2], "textFormat", value.c_str(), false, true); }
        // The placement of the data label relative to the labeled data.
        DataLabel &placement(DataLabelPlacement value)
        {
            if (value == DATA_LABEL_PLACEMENT_UNSPECIFIED)
                return setObject(buf[3], "placement", "DATA_LABEL_PLACEMENT_UNSPECIFIED", true, true);
            else if (value == CENTER_LABEL)
                return setObject(buf[3], "placement", "CENTER", true, true);
            else if (value == LEFT_LABEL)
                return setObject(buf[3], "placement", "LEFT", true, true);
            else if (value == RIGHT_LABEL)
                return setObject(buf[3], "placement", "RIGHT", true, true);
            else if (value == ABOVE)
                return setObject(buf[3], "placement", "ABOVE", true, true);
            else if (value == BELOW)
                return setObject(buf[3], "placement", "BELOW", true, true);
            else if (value == INSIDE_END)
                return setObject(buf[3], "placement", "INSIDE_END", true, true);
            else if (value == INSIDE_BASE)
                return setObject(buf[3], "placement", "INSIDE_BASE", true, true);
            else if (value == OUTSIDE_END)
                return setObject(buf[3], "placement", "OUTSIDE_END", true, true);
            return *this;
        }
        // Data to use for custom labels. Only used if type is set to CUSTOM. This data must be the same length as the series or other element this data label is applied to. In addition, if the series is split into multiple source ranges, this source data must come from the next column in the source data. For example, if the series is B2:B4,E6:E8 then this data must come from C2:C4,F6:F8.
        DataLabel &customLabelData(const ChartData &value) { return setObject(buf[4], "customLabelData", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif