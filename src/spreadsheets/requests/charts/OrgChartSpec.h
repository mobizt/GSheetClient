#ifndef ORG_CHART_SPEC_H
#define ORG_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"

/**
 * ORG CHART CLASS DEPENDENCIES
 *
 * OrgChartSpec             (enum) OrgChartNodeSize
 *                          ColorStyle*
 *                          ChartData*
 *
 * See ChartData.h, Theme.h
 */

namespace GSHEET
{
    // The size of the org chart nodes.
    enum OrgChartNodeSize
    {
        ORG_CHART_LABEL_SIZE_UNSPECIFIED, //	Default value, do not use.
        SMALL,                            //	The small org chart node size.
        MEDIUM,                           //	The medium org chart node size.
        LARGE                             //	The large org chart node size.
    };

    /**
     * An org chart. Org charts require a unique set of labels in labels and may optionally include parentLabels and tooltips. parentLabels contain, for each node, the label identifying the parent node. tooltips contain, for each node, an optional tooltip.
     * For example, to describe an OrgChart with Alice as the CEO, Bob as the President (reporting to Alice) and Cathy as VP of Sales (also reporting to Alice), have labels contain "Alice", "Bob", "Cathy", parentLabels contain "", "Alice", "Alice" and tooltips contain "CEO", "President", "VP Sales".
     */
    class OrgChartSpec : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        OrgChartSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        OrgChartSpec() {}
        // Where the legend of the pie chart should be drawn.
        OrgChartSpec &legendPosition(OrgChartNodeSize value)
        {
            if (value == ORG_CHART_LABEL_SIZE_UNSPECIFIED)
                return setObject(buf[1], "legendPosition", "ORG_CHART_LABEL_SIZE_UNSPECIFIED", true, true);
            else if (value == SMALL)
                return setObject(buf[1], "legendPosition", "SMALL", true, true);
            else if (value == MEDIUM)
                return setObject(buf[1], "legendPosition", "MEDIUM", true, true);
            else if (value == LARGE)
                return setObject(buf[1], "legendPosition", "LARGE", true, true);
            return *this;
        }
        // The color of the org chart nodes. If nodeColor is also set, this field takes precedence.
        OrgChartSpec &nodeColorStyle(const ColorStyle &value) { return setObject(buf[2], "nodeColorStyle", value.c_str(), false, true); }
        // The color of the selected org chart nodes. If selectedNodeColor is also set, this field takes precedence.
        OrgChartSpec &selectedNodeColorStyle(const ColorStyle &value) { return setObject(buf[3], "selectedNodeColorStyle", value.c_str(), false, true); }
        // The data containing the labels for all the nodes in the chart. Labels must be unique.
        OrgChartSpec &labels(const ChartData &value) { return setObject(buf[4], "labels", value.c_str(), false, true); }
        // The data containing the label of the parent for the corresponding node. A blank value indicates that the node has no parent and is a top-level node. This field is optional.
        OrgChartSpec &parentLabels(const ChartData &value) { return setObject(buf[5], "parentLabels", value.c_str(), false, true); }
        // The data containing the tooltip for the corresponding node. A blank value results in no tooltip being displayed for the node. This field is optional.
        OrgChartSpec &tooltips(const ChartData &value) { return setObject(buf[6], "tooltips", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif