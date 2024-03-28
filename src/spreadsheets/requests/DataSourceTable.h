#ifndef DATASOURCE_TABLE_H
#define DATASOURCE_TABLE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/FilterSpec.h"

namespace GSHEET
{

    // The data source table column selection types.
    enum DataSourceTableColumnSelectionType
    {
        DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED, //	The default column selection type, do not use.
        SELECTED,                                            //	Select columns specified by columns field.
        SYNC_ALL                                             //	Sync all current and future columns in the data source.
        // If set, the data source table fetches all the columns in the data source at the time of refresh.
    };

    /**
     * A data source table, which allows the user to import a static table of data from the DataSource into Sheets. This is also known as "Extract" in the Sheets editor.
     */
    class DataSourceTable : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSourceTable &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSourceTable() {}
        // The ID of the data source the data source table is associated with.
        DataSourceTable &dataSourceId(const String &value) { return setObject(buf[1], "dataSourceId", value, true, true); }
        // The type to select columns for the data source table. Defaults to SELECTED.
        DataSourceTable &columnSelectionType(DataSourceTableColumnSelectionType value)
        {
            if (value == DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED)
                return setObject(buf[2], "columnSelectionType", "DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED", true, true);
            else if (value == SELECTED)
                return setObject(buf[2], "columnSelectionType", "SELECTED", true, true);
            else if (value == SYNC_ALL)
                return setObject(buf[2], "columnSelectionType", "SYNC_ALL", true, true);
            return *this;
        }
        // Columns selected for the data source table. The columnSelectionType must be SELECTED.
        DataSourceTable &addColumns(const DataSourceColumnReference &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("columns"), value.c_str(), false);
            return *this;
        }
        // Filter specifications in the data source table.
        DataSourceTable &addFilterSpecs(const FilterSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("filterSpecs"), value.c_str(), false);
            return *this;
        }
        // Sort specifications in the data source table. The result of the data source table is sorted based on the sort specifications in order.
        DataSourceTable &addSortSpecs(const SortSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("sortSpecs"), value.c_str(), false);
            return *this;
        }
        // The limit of rows to return. If not set, a default limit is applied. Please refer to the Sheets editor for the default and max limit.
        DataSourceTable &rowLimit(int value) { return setObject(buf[6], "rowLimit", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif