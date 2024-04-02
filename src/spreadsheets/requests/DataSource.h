#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/GridRange.h"
#include "./spreadsheets/requests/Common.h"

namespace GSHEET
{

    /**
     * A data source formula.
     */
    class DataSourceFormula : public O1
    {
    public:
        DataSourceFormula() {}
        // The ID of the data source the formula is associated with.
        DataSourceFormula &dataSourceId(const String &value) { return wr.add<DataSourceFormula &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A parameter in a data source's query. The parameter allows the user to pass in values from the spreadsheet into a query.
     */
    class DataSourceParameter : public O4
    {
    public:
        DataSourceParameter() {}
        // Named parameter. Must be a legitimate identifier for the DataSource that supports it. For example, BigQuery identifier.
        DataSourceParameter &name(const String &value) { return wr.set<DataSourceParameter &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Union field value
        // ID of a NamedRange. Its size must be 1x1.
        DataSourceParameter &namedRangeId(const String &value) { return wr.set<DataSourceParameter &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Union field value
        // A range that contains the value of the parameter. Its size must be 1x1.
        DataSourceParameter &range(const GridRange &value) { return wr.set<DataSourceParameter &, GridRange>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Represents a time interval, encoded as a Timestamp start (inclusive) and a Timestamp end (exclusive).
     * The start must be less than or equal to the end. When the start equals the end, the interval is empty (matches no time). When both start and end are unspecified, the interval matches any time.
     */
    class Interval : public O4
    {
    public:
        Interval() {}
        // Optional. Inclusive start of the interval.
        // If specified, a Timestamp matching this interval will have to be the same or after the start.
        Interval &startTime(const String &value) { return wr.set<Interval &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Optional. Exclusive end of the interval.
        // If specified, a Timestamp matching this interval will have to be before the end.
        Interval &endTime(const String &value) { return wr.set<Interval &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Specifies a custom BigQuery query.
     */
    class BigQueryQuerySpec : public O1
    {
    public:
        BigQueryQuerySpec() {}
        // The raw query string.
        BigQueryQuerySpec &rawQuery(const String &value) { return wr.add<BigQueryQuerySpec &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Specifies a BigQuery table definition. Only native tables are allowed.
     */
    class BigQueryTableSpec : public O4
    {
    public:
        BigQueryTableSpec() {}
        // The ID of a BigQuery project the table belongs to. If not specified, the projectId is assumed.
        BigQueryTableSpec &tableProjectId(const String &value) { return wr.set<BigQueryTableSpec &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The BigQuery table id.
        BigQueryTableSpec &tableId(const String &value) { return wr.set<BigQueryTableSpec &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The BigQuery dataset id.
        BigQueryTableSpec &datasetId(const String &value) { return wr.set<BigQueryTableSpec &, String>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

    /**
     * The specification of a BigQuery data source that's connected to a sheet.
     */
    class BigQueryDataSourceSpec : public O4
    {
    public:
        BigQueryDataSourceSpec() {}
        // The ID of a BigQuery enabled Google Cloud project with a billing account attached. For any queries executed against the data source, the project is charged.
        BigQueryDataSourceSpec &projectId(const String &value) { return wr.set<BigQueryDataSourceSpec &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Union field spec
        // A BigQueryQuerySpec.
        BigQueryDataSourceSpec &querySpec(const BigQueryQuerySpec &value) { return wr.set<BigQueryDataSourceSpec &, BigQueryQuerySpec>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Union field spec
        // A BigQueryTableSpec.
        BigQueryDataSourceSpec &tableSpec(const BigQueryTableSpec &value) { return wr.set<BigQueryDataSourceSpec &, BigQueryTableSpec>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * This specifies the details of the data source. For example, for BigQuery, this specifies information about the BigQuery source.
     */
    class DataSourceSpec : public O4
    {
    public:
        DataSourceSpec() {}
        // Add to a list
        // The parameters of the data source, used when querying the data source.
        DataSourceSpec &parameters(const DataSourceParameter &value) { return wr.append<DataSourceSpec &, DataSourceParameter>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // A BigQueryDataSourceSpec.
        DataSourceSpec &bigQuery(const BigQueryDataSourceSpec &value) { return wr.set<DataSourceSpec &, BigQueryDataSourceSpec>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * An unique identifier that references a data source column.
     */
    class DataSourceColumnReference : public O1
    {
    public:
        DataSourceColumnReference() {}
        // The display name of the column. It should be unique within a data source.
        DataSourceColumnReference &name(const String &value) { return wr.add<DataSourceColumnReference &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * A column in a data source.
     */
    class DataSourceColumn : public O4
    {

    public:
        DataSourceColumn() {}
        // The column reference.
        DataSourceColumn &reference(const DataSourceColumnReference &value) { return wr.set<DataSourceColumn &, DataSourceColumnReference>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The formula of the calculated column.
        DataSourceColumn &formula(const String &value) { return wr.set<DataSourceColumn &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Information about an external data source in the spreadsheet.
     */
    class DataSource : public O6
    {
    public:
        DataSource() {}
        // The spreadsheet-scoped unique ID that identifies the data source. Example: 1080547365.
        DataSource &dataSourceId(const String &value) { return wr.set<DataSource &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The DataSourceSpec for the data source connected with this spreadsheet.
        DataSource &spec(const DataSourceSpec &value) { return wr.set<DataSource &, DataSourceSpec>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Add to a list
        // All calculated columns in the data source.
        DataSource &calculatedColumns(const DataSourceColumn &value) { return wr.append<DataSource &, DataSourceColumn>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // The ID of the Sheet connected with the data source. The field cannot be changed once set.
        // When creating a data source, an associated DATA_SOURCE sheet is also created, if the field is not specified, the ID of the created sheet will be randomly generated.
        DataSource &sheetId(int value) { return wr.set<DataSource &, int>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
    };

}

#endif