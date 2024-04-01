#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/GridRange.h"

namespace GSHEET
{

    // Represents a day of the week.
    enum DayOfWeek
    {
        DAY_OF_WEEK_UNSPECIFIED, //	The day of the week is unspecified.
        MONDAY,                  //	Monday
        TUESDAY,                 //	Tuesday
        WEDNESDAY,               //	Wednesday
        THURSDAY,                //	Thursday
        FRIDAY,                  //	Friday
        SATURDAY,                //	Saturday
        SUNDAY                   //	Sunday
    };

    /**
     * A data source formula.
     */
    class DataSourceFormula : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceFormula() {}
        // The ID of the data source the formula is associated with.
        DataSourceFormula &dataSourceId(const String &value)
        {
            clear();
            jut.addObject(buf, "dataSourceId", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * A parameter in a data source's query. The parameter allows the user to pass in values from the spreadsheet into a query.
     */
    class DataSourceParameter : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSourceParameter &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSourceParameter() {}
        // Named parameter. Must be a legitimate identifier for the DataSource that supports it. For example, BigQuery identifier.
        DataSourceParameter &name(const String &value) { return setObject(buf[1], "name", value, true, true); }
        // Union field value
        // ID of a NamedRange. Its size must be 1x1.
        DataSourceParameter &namedRangeId(const String &value) { return setObject(buf[2], "namedRangeId", value, true, true); }
        // Union field value
        // A range that contains the value of the parameter. Its size must be 1x1.
        DataSourceParameter &range(const GridRange &value) { return setObject(buf[2], "range", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Represents a time interval, encoded as a Timestamp start (inclusive) and a Timestamp end (exclusive).
     * The start must be less than or equal to the end. When the start equals the end, the interval is empty (matches no time). When both start and end are unspecified, the interval matches any time.
     */
    class Interval : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Interval &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Interval() {}
        // Optional. Inclusive start of the interval.
        // If specified, a Timestamp matching this interval will have to be the same or after the start.
        Interval &startTime(const String &value) { return setObject(buf[1], "startTime", value, true, true); }
        // Optional. Exclusive end of the interval.
        // If specified, a Timestamp matching this interval will have to be before the end.
        Interval &endTime(const String &value) { return setObject(buf[2], "endTime", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Specifies a custom BigQuery query.
     */
    class BigQueryQuerySpec : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        BigQueryQuerySpec() {}
        // The raw query string.
        BigQueryQuerySpec &rawQuery(const String &value)
        {
            clear();
            jut.addObject(buf, "rawQuery", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Specifies a BigQuery table definition. Only native tables are allowed.
     */
    class BigQueryTableSpec : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BigQueryTableSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BigQueryTableSpec() {}
        // The ID of a BigQuery project the table belongs to. If not specified, the projectId is assumed.
        BigQueryTableSpec &tableProjectId(const String &value) { return setObject(buf[1], "tableProjectId", value, true, true); }
        // The BigQuery table id.
        BigQueryTableSpec &tableId(const String &value) { return setObject(buf[2], "tableId", value, true, true); }
        // The BigQuery dataset id.
        BigQueryTableSpec &datasetId(const String &value) { return setObject(buf[3], "datasetId", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The specification of a BigQuery data source that's connected to a sheet.
     */
    class BigQueryDataSourceSpec : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BigQueryDataSourceSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BigQueryDataSourceSpec() {}
        // The ID of a BigQuery enabled Google Cloud project with a billing account attached. For any queries executed against the data source, the project is charged.
        BigQueryDataSourceSpec &projectId(const String &value) { return setObject(buf[1], "projectId", value, true, true); }
        // Union field spec
        // A BigQueryQuerySpec.
        BigQueryDataSourceSpec &querySpec(const BigQueryQuerySpec &value) { return setObject(buf[2], "querySpec", value.c_str(), false, true); }
        // Union field spec
        // A BigQueryTableSpec.
        BigQueryDataSourceSpec &tableSpec(const BigQueryTableSpec &value) { return setObject(buf[2], "tableSpec", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * This specifies the details of the data source. For example, for BigQuery, this specifies information about the BigQuery source.
     */
    class DataSourceSpec : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSourceSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSourceSpec() {}
        // The parameters of the data source, used when querying the data source.
        DataSourceSpec &addParameters(const DataSourceParameter &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("parameters"), value.c_str(), false);
            return *this;
        }
        // A BigQueryDataSourceSpec.
        DataSourceSpec &bigQuery(const BigQueryDataSourceSpec &value) { return setObject(buf[2], "bigQuery", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * An unique identifier that references a data source column.
     */
    class DataSourceColumnReference : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceColumnReference() {}
        // The display name of the column. It should be unique within a data source.
        DataSourceColumnReference &name(const String &value)
        {
            clear();
            jut.addObject(buf, "name", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * A column in a data source.
     */
    class DataSourceColumn : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSourceColumn &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSourceColumn() {}
        // The column reference.
        DataSourceColumn &reference(const DataSourceColumnReference &value) { return setObject(buf[1], "reference", value.c_str(), false, true); }
        // The formula of the calculated column.
        DataSourceColumn &formula(const String &value) { return setObject(buf[2], "formula", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Information about an external data source in the spreadsheet.
     */
    class DataSource : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSource &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSource() {}
        // The spreadsheet-scoped unique ID that identifies the data source. Example: 1080547365.
        DataSource &dataSourceId(const String &value) { return setObject(buf[1], "dataSourceId", value, true, true); }
        // The DataSourceSpec for the data source connected with this spreadsheet.
        DataSource &spec(const DataSourceSpec &value) { return setObject(buf[2], "spec", value.c_str(), false, true); }
        // All calculated columns in the data source.
        DataSource &addCalculatedColumns(const DataSourceColumn &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("calculatedColumns"), value.c_str(), false);
            return *this;
        }
        // The ID of the Sheet connected with the data source. The field cannot be changed once set.
        // When creating a data source, an associated DATA_SOURCE sheet is also created, if the field is not specified, the ID of the created sheet will be randomly generated.
        DataSource &sheetId(int value) { return setObject(buf[4], "sheetId", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}

#endif