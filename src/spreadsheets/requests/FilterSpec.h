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
    class FilterCriteria : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        FilterCriteria &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        FilterCriteria() {}
        // Values that should be hidden.
        FilterCriteria &hiddenValues(const String &value) { return setObject(buf[1], "hiddenValues", value, true, true); }
        // A condition that must be true for values to be shown. (This does not override hiddenValues -- if a value is listed there, it will still be hidden.)
        FilterCriteria &condition(const BooleanCondition &value) { return setObject(buf[2], "condition", value.c_str(), false, true); }
        // The background fill color to filter by; only cells with this fill color are shown. This field is mutually exclusive with visibleForegroundColor, and must be set to an RGB-type color. If visibleBackgroundColor is also set, this field takes precedence.
        FilterCriteria &visibleBackgroundColorStyle(const ColorStyle &value) { return setObject(buf[3], "visibleBackgroundColorStyle", value.c_str(), false, true); }
        // The foreground color to filter by; only cells with this foreground color are shown. This field is mutually exclusive with visibleBackgroundColor, and must be set to an RGB-type color. If visibleForegroundColor is also set, this field takes precedence.
        FilterCriteria &visibleForegroundColorStyle(const Color &value) { return setObject(buf[4], "visibleForegroundColorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The filter criteria associated with a specific column.
     */
    class FilterSpec : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        FilterSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        FilterSpec() {}
        // The criteria for the column.
        FilterSpec &filterCriteria(const FilterCriteria &value) { return setObject(buf[1], "filterCriteria", value.c_str(), false, true); }
        // Union field reference
        // The zero-based column index.
        FilterSpec &columnIndex(int value) { return setObject(buf[2], "columnIndex", String(value), false, true); }
        // Union field reference
        // Reference to a data source column.
        FilterSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return setObject(buf[2], "dataSourceColumnReference", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };


}
#endif