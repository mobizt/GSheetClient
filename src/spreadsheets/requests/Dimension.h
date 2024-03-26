#ifndef DIMENSION_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{
    // An enumeration of possible metadata visibilities.
    enum DeveloperMetadataVisibility
    {
        DEVELOPER_METADATA_VISIBILITY_UNSPECIFIED, //	Default value.
        DOCUMENT,                                  //	Document-visible metadata is accessible from any developer project with access to the document.
        PROJECT,                                   //	Project-visible metadata is only visible to and accessible by the developer project that created the metadata.
    };
    // Indicates which dimension an operation should apply to.
    enum Dimension
    {
        DIMENSION_UNSPECIFIED, //	The default value, do not use.
        ROWS,                  //	Operates on the rows of a sheet.
        COLUMNS                //	Operates on the columns of a sheet.
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
            jut.addObject(buf, "name", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * A range along a single dimension on a DATA_SOURCE sheet.
     */
    class DataSourceSheetDimensionRange : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSourceSheetDimensionRange &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSourceSheetDimensionRange() {}
        // The ID of the data source sheet the range is on.
        DataSourceSheetDimensionRange &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        DataSourceSheetDimensionRange &addColumnReferences(DataSourceColumnReference value)
        {

            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("columnReferences"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A range along a single dimension on a sheet. All indexes are zero-based. Indexes are half open: the start index is inclusive and the end index is exclusive. Missing indexes indicate the range is unbounded on that side.
     */
    class DimensionRange : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DimensionRange &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DimensionRange() {}
        // The sheet this span is on.
        DimensionRange &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        // The dimension of the span.
        DimensionRange &dimension(Dimension value)
        {
            if (value == DIMENSION_UNSPECIFIED)
                return setObject(buf[2], "dimension", "DIMENSION_UNSPECIFIED", true, true);
            else if (value == ROWS)
                return setObject(buf[2], "dimension", "ROWS", true, true);
            else if (value == COLUMNS)
                return setObject(buf[2], "dimension", "COLUMNS", true, true);
            return *this;
        }
        // The start (inclusive) of the span, or not set if unbounded.
        DimensionRange &startIndex(int value) { return setObject(buf[3], "startIndex", String(value), false, true); }
        // The end (exclusive) of the span, or not set if unbounded.
        DimensionRange &endIndex(int value) { return setObject(buf[4], "endIndex", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A location where metadata may be associated in a spreadsheet.
     */
    class DeveloperMetadataLocation : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DeveloperMetadataLocation &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DeveloperMetadataLocation() {}
        // True when metadata is associated with an entire spreadsheet.
        DeveloperMetadataLocation &spreadsheet(bool value) { return setObject(buf[1], "spreadsheet", owriter.getBoolStr(value), false, true); }
        // The ID of the sheet when metadata is associated with an entire sheet.
        DeveloperMetadataLocation &sheetId(int value) { return setObject(buf[2], "sheetId", String(value), false, true); }
        // Represents the row or column when metadata is associated with a dimension. The specified DimensionRange must represent a single row or column; it cannot be unbounded or span multiple rows or columns.
        DeveloperMetadataLocation &dimensionRange(const DimensionRange value) { return setObject(buf[3], "dimensionRange", value.c_str(), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Developer metadata associated with a location or object in a spreadsheet.
     */
    class DeveloperMetadata : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DeveloperMetadata &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DeveloperMetadata() {}
        // The spreadsheet-scoped unique ID that identifies the metadata. IDs may be specified when metadata is created, otherwise one will be randomly generated and assigned. Must be positive.
        DeveloperMetadata &metadataId(int value) { return setObject(buf[1], "metadataId", String(value), false, true); }
        // The metadata key. There may be multiple metadata in a spreadsheet with the same key. Developer metadata must always have a key specified.
        DeveloperMetadata &metadataKey(const String &value) { return setObject(buf[2], "metadataKey", value, true, true); }
        // Data associated with the metadata's key.
        DeveloperMetadata &metadataValue(const String &value) { return setObject(buf[3], "metadataValue", value, true, true); }
        // The location where the metadata is associated.
        DeveloperMetadata &location(const DeveloperMetadataLocation &value) { return setObject(buf[4], "location", value.c_str(), true, true); }

        // The metadata visibility. Developer metadata must always have a visibility specified.
        DeveloperMetadata &visibility(DeveloperMetadataVisibility value)
        {

            if (value == DEVELOPER_METADATA_VISIBILITY_UNSPECIFIED)
                return setObject(buf[5], "visibility", "DEVELOPER_METADATA_VISIBILITY_UNSPECIFIED", true, true);
            else if (value == DOCUMENT)
                return setObject(buf[5], "visibility", "DOCUMENT", true, true);
            else if (value == PROJECT)
                return setObject(buf[5], "visibility", "PROJECT", true, true);
            return *this;
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Properties about a dimension.
     */
    class DimensionProperties : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DimensionProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DimensionProperties() {}
        // True if this dimension is being filtered. This field is read-only.
        DimensionProperties &hiddenByFilter(bool value) { return setObject(buf[1], "hiddenByFilter", owriter.getBoolStr(value), false, true); }
        // True if this dimension is explicitly hidden.
        DimensionProperties &hiddenByUser(bool value) { return setObject(buf[2], "hiddenByUser", owriter.getBoolStr(value), false, true); }
        // The height (if a row) or width (if a column) of the dimension in pixels.
        DimensionProperties &pixelSize(int value) { return setObject(buf[3], "pixelSize", String(value), false, true); }
        // The developer metadata associated with a single row or column.
        DimensionProperties &addDeveloperMetadata(const DeveloperMetadata value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("developerMetadata"), value.c_str(), false);
            return *this;
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates properties of dimensions within the specified range.
     */
    class UpdateDimensionPropertiesRequest : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateDimensionPropertiesRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateDimensionPropertiesRequest() {}
        // Properties to update.
        UpdateDimensionPropertiesRequest &properties(const DimensionProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateDimensionPropertiesRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }
        // The rows or columns to update.
        UpdateDimensionPropertiesRequest &range(const DimensionRange &value)
        {
            // Union field dimension_range
            if (buf[4].length() == 0)
                return setObject(buf[3], "range", value.c_str(), false, true);
            return *this;
        }
        // The columns on a data source sheet to update.
        UpdateDimensionPropertiesRequest &dataSourceSheetRange(const DataSourceSheetDimensionRange &value)
        {
            // Union field dimension_range
            if (buf[3].length() == 0)
                return setObject(buf[4], "dataSourceSheetRange", value.c_str(), false, true);
            return *this;
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}

#endif