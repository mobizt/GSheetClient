#ifndef DATASOURCE_TABLE_H
#define DATASOURCE_TABLE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/FilterSpec.h"
#include "./spreadsheets/requests/Common.h"

namespace GSHEET
{

    /**
     * A data source table, which allows the user to import a static table of data from the DataSource into Sheets. This is also known as "Extract" in the Sheets editor.
     */
    class DataSourceTable : public O8
    {

    public:
        DataSourceTable() {}
        // The ID of the data source the data source table is associated with.
        DataSourceTable &dataSourceId(const String &value) { return wr.set<DataSourceTable &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The type to select columns for the data source table. Defaults to SELECTED.
        DataSourceTable &columnSelectionType(Selection::DataSourceTableColumnSelectionType value) { return wr.set<DataSourceTable &, const char *>(*this, _DataSourceTableColumnSelectionType[value].text, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Add to a list
        // Columns selected for the data source table. The columnSelectionType must be SELECTED.
        DataSourceTable &columns(const DataSourceColumnReference &value) { return wr.append<DataSourceTable &, DataSourceColumnReference>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // Add to a list
        //  Filter specifications in the data source table.
        DataSourceTable &filterSpecs(const FilterSpec &value) { return wr.append<DataSourceTable &, FilterSpec>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // Add to a list
        // Sort specifications in the data source table. The result of the data source table is sorted based on the sort specifications in order.
        DataSourceTable &sortSpecs(const SortSpec &value) { return wr.append<DataSourceTable &, SortSpec>(*this, value, buf, bufSize, buf[5], FPSTR(__func__)); }
        // The limit of rows to return. If not set, a default limit is applied. Please refer to the Sheets editor for the default and max limit.
        DataSourceTable &rowLimit(int value) { return wr.set<DataSourceTable &, int>(*this, value, buf, bufSize, buf[6], FPSTR(__func__)); }
    };
}

#endif