#ifndef DIMENSION_H
#define DIMENSION_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/DataSource.h"
#include "./spreadsheets/requests/GridRange.h"
#include "./spreadsheets/requests/Common.h"

namespace GSHEET
{

    /**
     * A range along a single dimension on a DATA_SOURCE sheet.
     */
    class DataSourceSheetDimensionRange : public BaseG4
    {
    public:
        DataSourceSheetDimensionRange() = default;

        // The ID of the data source sheet the range is on.
        DataSourceSheetDimensionRange &sheetId(int value) { return wr.set<DataSourceSheetDimensionRange &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The columns on the data source sheet.
        DataSourceSheetDimensionRange &columnReferences(DataSourceColumnReference value) { return wr.append<DataSourceSheetDimensionRange &, DataSourceColumnReference>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A range along a single dimension on a sheet. All indexes are zero-based. Indexes are half open: the start index is inclusive and the end index is exclusive. Missing indexes indicate the range is unbounded on that side.
     */
    class DimensionRange : public BaseG6
    {

    public:
        DimensionRange() = default;

        // The sheet this span is on.
        DimensionRange &sheetId(int value) { return wr.set<DimensionRange &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The dimension of the span.
        DimensionRange &dimension(Dimensions::Dimension value) { return wr.set<DimensionRange &, const char *>(*this, _Dimension[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // The start (inclusive) of the span, or not set if unbounded.
        DimensionRange &startIndex(int value) { return wr.set<DimensionRange &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The end (exclusive) of the span, or not set if unbounded.
        DimensionRange &endIndex(int value) { return wr.set<DimensionRange &, int>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A location where metadata may be associated in a spreadsheet.
     */
    class DeveloperMetadataLocation : public BaseG4
    {
    public:
        DeveloperMetadataLocation() = default;

        // True when metadata is associated with an entire spreadsheet.
        DeveloperMetadataLocation &spreadsheet(bool value) { return wr.set<DeveloperMetadataLocation &, bool>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The ID of the sheet when metadata is associated with an entire sheet.
        DeveloperMetadataLocation &sheetId(int value) { return wr.set<DeveloperMetadataLocation &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Represents the row or column when metadata is associated with a dimension. The specified DimensionRange must represent a single row or column; it cannot be unbounded or span multiple rows or columns.
        DeveloperMetadataLocation &dimensionRange(const DimensionRange value) { return wr.set<DeveloperMetadataLocation &, DimensionRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

    /**
     * Developer metadata associated with a location or object in a spreadsheet.
     */
    class DeveloperMetadata : public BaseG6
    {
    public:
        DeveloperMetadata() = default;

        // The spreadsheet-scoped unique ID that identifies the metadata. IDs may be specified when metadata is created, otherwise one will be randomly generated and assigned. Must be positive.
        DeveloperMetadata &metadataId(int value) { return wr.set<DeveloperMetadata &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The metadata key. There may be multiple metadata in a spreadsheet with the same key. Developer metadata must always have a key specified.
        DeveloperMetadata &metadataKey(const String &value) { return wr.set<DeveloperMetadata &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Data associated with the metadata's key.
        DeveloperMetadata &metadataValue(const String &value) { return wr.set<DeveloperMetadata &, String>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The location where the metadata is associated.
        DeveloperMetadata &location(const DeveloperMetadataLocation &value) { return wr.set<DeveloperMetadata &, DeveloperMetadataLocation>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The metadata visibility. Developer metadata must always have a visibility specified.
        DeveloperMetadata &visibility(Metadata::DeveloperMetadataVisibility value) { return wr.set<DeveloperMetadata &, const char *>(*this, _DeveloperMetadataVisibility[value].text, buf, bufSize, 5, FPSTR(__func__)); }
    };

    /**
     * Properties about a dimension.
     */
    class DimensionProperties : public BaseG6
    {
    public:
        DimensionProperties() = default;

        // True if this dimension is being filtered. This field is read-only.
        DimensionProperties &hiddenByFilter(bool value) { return wr.set<DimensionProperties &, bool>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // True if this dimension is explicitly hidden.
        DimensionProperties &hiddenByUser(bool value) { return wr.set<DimensionProperties &, bool>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The height (if a row) or width (if a column) of the dimension in pixels.
        DimensionProperties &pixelSize(int value) { return wr.set<DimensionProperties &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // This value represents the item to add to an array
        //  The developer metadata associated with a single row or column.
        DimensionProperties &developerMetadata(const DeveloperMetadata value) { return wr.append<DimensionProperties &, DeveloperMetadata>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A combination of a source range and how to extend that source.
     */
    class SourceAndDestination : public BaseG4
    {
    public:
        SourceAndDestination() = default;

        // The location of the data to use as the source of the autofill.
        SourceAndDestination &source(const GridRange &value) { return wr.set<SourceAndDestination &, GridRange>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The location of the data to use as the source of the autofill.
        SourceAndDestination &dimension(Dimensions::Dimension value) { return wr.set<SourceAndDestination &, const char *>(*this, _Dimension[value].text, buf, bufSize, 2, FPSTR(__func__)); }

        // The number of rows or columns that data should be filled into. Positive numbers expand beyond the last row or last column of the source. Negative numbers expand before the first row or first column of the source.
        SourceAndDestination &fillLength(int value) { return wr.set<SourceAndDestination &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

}

#endif