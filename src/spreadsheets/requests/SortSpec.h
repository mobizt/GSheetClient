#ifndef SORT_SPEC_H
#define SORT_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/DataSource.h"
#include "./spreadsheets/requests/Theme.h"

/**
 * SORT SPEC CLASS DEPENDENCIES
 *
 * SortSpec         +       (enum) SortOrder
 *                  |
 *                  +       ColorStyle*
 *                  |
 *                  +       DataSourceColumnReference
 *
 * See Theme.h
 */

namespace GSHEET
{

    /**
     * A sort order associated with a specific column or row.
     */
    class SortSpec : public BaseG6
    {

    public:
        SortSpec() = default;

        // The order data should be sorted.
        SortSpec &sortOrder(Sort::SortOrder value) { return wr.set<SortSpec &, const char *>(*this, _SortOrder[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The foreground color to sort by; cells with this foreground color are sorted to the top. Mutually exclusive with backgroundColor, and must be an RGB-type color. If foregroundColor is also set, this field takes precedence.
        SortSpec &foregroundColorStyle(const ColorStyle &value) { return wr.set<SortSpec &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The background fill color to sort by; cells with this fill color are sorted to the top. Mutually exclusive with foregroundColor, and must be an RGB-type color. If backgroundColor is also set, this field takes precedence.
        SortSpec &backgroundColorStyle(const ColorStyle &value) { return wr.set<SortSpec &, ColorStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // Union field reference
        //  The dimension the sort should be applied to.
        SortSpec &dimensionIndex(int value) { return wr.set<SortSpec &, int>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Union field reference
        //  Reference to a data source column.
        SortSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return wr.set<SortSpec &, DataSourceColumnReference>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

}
#endif