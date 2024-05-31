#ifndef FILTER_SPEC_H
#define FILTER_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/BooleanCondition.h"
#include "./spreadsheets/requests/DataSource.h"

/**
 * FILTER SPEC CLASS DEPENDENCIES
 *
 * FilterSpec   +   FilterCriteria                  +       BooleanCondition*
 *              |                                   |
 *              |                                   +       ColorStyle*
 *              |
 *              +       DataSourceColumnReference
 *
 * See Theme.h
 */

namespace GSHEET
{

    /**
     * Criteria for showing/hiding rows in a filter or filter view.
     */
    class FilterCriteria : public BaseG6
    {
    public:
        FilterCriteria() {}

        // Values that should be hidden.
        FilterCriteria &hiddenValues(const String &value) { return wr.set<FilterCriteria &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // A condition that must be true for values to be shown. (This does not override hiddenValues -- if a value is listed there, it will still be hidden.)
        FilterCriteria &condition(const BooleanCondition &value) { return wr.set<FilterCriteria &, BooleanCondition>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The background fill color to filter by; only cells with this fill color are shown. This field is mutually exclusive with visibleForegroundColor, and must be set to an RGB-type color. If visibleBackgroundColor is also set, this field takes precedence.
        FilterCriteria &visibleBackgroundColorStyle(const ColorStyle &value) { return wr.set<FilterCriteria &, ColorStyle>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The foreground color to filter by; only cells with this foreground color are shown. This field is mutually exclusive with visibleBackgroundColor, and must be set to an RGB-type color. If visibleForegroundColor is also set, this field takes precedence.
        FilterCriteria &visibleForegroundColorStyle(const ColorStyle &value) { return wr.set<FilterCriteria &, ColorStyle>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * The filter criteria associated with a specific column.
     */
    class FilterSpec : public BaseG4
    {

    public:
        FilterSpec() {}

        // The criteria for the column.
        FilterSpec &filterCriteria(const FilterCriteria &value) { return wr.set<FilterSpec &, FilterCriteria>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Union field reference
        // The zero-based column index.
        FilterSpec &columnIndex(int value) { return wr.set<FilterSpec &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Union field reference
        // Reference to a data source column.
        FilterSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return wr.set<FilterSpec &, DataSourceColumnReference>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

}
#endif