#ifndef SORT_SPEC_H
#define SORT_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/DataSourceColumnReference.h"
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

    // A sort order.
    enum SortOrder
    {
        SORT_ORDER_UNSPECIFIED, //	Default value, do not use this.
        ASCENDING,              //	Sort ascending.
        DESCENDING              //	Sort descending.
    };

        /**
     * A sort order associated with a specific column or row.
     */
    class SortSpec : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SortSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SortSpec() {}
        // The order data should be sorted.
        SortSpec &sortOrder(SortOrder value)
        {
            if (value == SORT_ORDER_UNSPECIFIED)
                return setObject(buf[1], "sortOrder", "SORT_ORDER_UNSPECIFIED", true, true);
            else if (value == ASCENDING)
                return setObject(buf[1], "sortOrder", "ASCENDING", true, true);
            else if (value == DESCENDING)
                return setObject(buf[1], "sortOrder", "DESCENDING", true, true);
            return *this;
        }
        // The foreground color to sort by; cells with this foreground color are sorted to the top. Mutually exclusive with backgroundColor, and must be an RGB-type color. If foregroundColor is also set, this field takes precedence.
        SortSpec &foregroundColorStyle(const ColorStyle &value) { return setObject(buf[2], "foregroundColorStyle", value.c_str(), false, true); }
        // The background fill color to sort by; cells with this fill color are sorted to the top. Mutually exclusive with foregroundColor, and must be an RGB-type color. If backgroundColor is also set, this field takes precedence.
        SortSpec &backgroundColorStyle(const ColorStyle &value) { return setObject(buf[3], "backgroundColorStyle", value.c_str(), false, true); }
        // The dimension the sort should be applied to.
        SortSpec &dimensionIndex(int value) { return buf[5].length() == 0 ? setObject(buf[4], "dimensionIndex", String(value), false, true) : *this; }
        // Reference to a data source column.
        SortSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return buf[4].length() == 0 ? setObject(buf[5], "dataSourceColumnReference", value.c_str(), false, true) : *this; }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}
#endif