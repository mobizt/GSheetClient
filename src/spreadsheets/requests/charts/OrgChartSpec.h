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

    const struct key_str_40 _OrgChartNodeSize[OrgChartNodeSize::LARGE + 1] PROGMEM = {"ORG_CHART_LABEL_SIZE_UNSPECIFIED", "SMALL", "MEDIUM", "LARGE"};

    /**
     * An org chart. Org charts require a unique set of labels in labels and may optionally include parentLabels and tooltips. parentLabels contain, for each node, the label identifying the parent node. tooltips contain, for each node, an optional tooltip.
     * For example, to describe an OrgChart with Alice as the CEO, Bob as the President (reporting to Alice) and Cathy as VP of Sales (also reporting to Alice), have labels contain "Alice", "Bob", "Cathy", parentLabels contain "", "Alice", "Alice" and tooltips contain "CEO", "President", "VP Sales".
     */
    class OrgChartSpec : public BaseG8
    {
    public:
        OrgChartSpec() = default;
        
        // The size of the org chart nodes.
        OrgChartSpec &nodeSize(OrgChartNodeSize value) { return wr.set<OrgChartSpec &, const char *>(*this, _OrgChartNodeSize[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The color of the org chart nodes. If nodeColor is also set, this field takes precedence.
        OrgChartSpec &nodeColorStyle(const ColorStyle &value) { return wr.set<OrgChartSpec &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The color of the selected org chart nodes. If selectedNodeColor is also set, this field takes precedence.
        OrgChartSpec &selectedNodeColorStyle(const ColorStyle &value) { return wr.set<OrgChartSpec &, ColorStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The data containing the labels for all the nodes in the chart. Labels must be unique.
        OrgChartSpec &labels(const ChartData &value) { return wr.set<OrgChartSpec &, ChartData>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The data containing the label of the parent for the corresponding node. A blank value indicates that the node has no parent and is a top-level node. This field is optional.
        OrgChartSpec &parentLabels(const ChartData &value) { return wr.set<OrgChartSpec &, ChartData>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // The data containing the tooltip for the corresponding node. A blank value results in no tooltip being displayed for the node. This field is optional.
        OrgChartSpec &tooltips(const ChartData &value) { return wr.set<OrgChartSpec &, ChartData>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }
    };
}

#endif