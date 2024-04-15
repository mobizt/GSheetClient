#ifndef DATA_FILTER_H
#define DATA_FILTER_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/GridRange.h"
#include "./spreadsheets/requests/Common.h"
#include "./spreadsheets/requests/Dimension.h"

namespace GSHEET
{

    /**
     * Selects DeveloperMetadata that matches all of the specified fields. For example, if only a metadata ID is specified this considers the DeveloperMetadata with that particular unique ID. If a metadata key is specified, this considers all developer metadata with that key. If a key, visibility, and location type are all specified, this considers all developer metadata with that key and visibility that are associated with a location of that type. In general, this selects all DeveloperMetadata that matches the intersection of all the specified fields; any field or combination of fields may be specified.
     */
    class DeveloperMetadataLookup : public BaseG6
    {
    public:
        DeveloperMetadataLookup() = default;

        // Limits the selected developer metadata to those entries which are associated with locations of the specified type. For example, when this field is specified as ROW this lookup only considers developer metadata associated on rows. If the field is left unspecified, all location types are considered. This field cannot be specified as SPREADSHEET when the locationMatchingStrategy is specified as INTERSECTING or when the metadataLocation is specified as a non-spreadsheet location: spreadsheet metadata cannot intersect any other developer metadata location. This field also must be left unspecified when the locationMatchingStrategy is specified as EXACT.
        DeveloperMetadataLookup &locationType(Metadata::DeveloperMetadataLocationType value) { return wr.set<DeveloperMetadataLookup &, const char *>(*this, _DeveloperMetadataLocationType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // Limits the selected developer metadata to those entries associated with the specified location. This field either matches exact locations or all intersecting locations according the specified locationMatchingStrategy.
        DeveloperMetadataLookup &metadataLocation(const DeveloperMetadataLocation &value) { return wr.set<DeveloperMetadataLookup &, DeveloperMetadataLocation>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Determines how this lookup matches the location. If this field is specified as EXACT, only developer metadata associated on the exact location specified is matched. If this field is specified to INTERSECTING, developer metadata associated on intersecting locations is also matched. If left unspecified, this field assumes a default value of INTERSECTING. If this field is specified, a metadataLocation must also be specified.
        DeveloperMetadataLookup &locationMatchingStrategy(Metadata::DeveloperMetadataLocationMatchingStrategy value) { return wr.set<DeveloperMetadataLookup &, const char *>(*this, _DeveloperMetadataLocationMatchingStrategy[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // Limits the selected developer metadata to that which has a matching DeveloperMetadata.metadata_id.
        DeveloperMetadataLookup &metadataId(int value) { return wr.set<DeveloperMetadataLookup &, int>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Limits the selected developer metadata to that which has a matching DeveloperMetadata.metadata_key.
        DeveloperMetadataLookup &metadataKey(const String &value) { return wr.set<DeveloperMetadataLookup &, String>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // Limits the selected developer metadata to that which has a matching DeveloperMetadata.metadata_value.
        DeveloperMetadataLookup &metadataValue(const String &value) { return wr.set<DeveloperMetadataLookup &, String>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // Limits the selected developer metadata to that which has a matching DeveloperMetadata.visibility. If left unspecified, all developer metadata visibile to the requesting project is considered.
        DeveloperMetadataLookup &visibility(Metadata::DeveloperMetadataVisibility value) { return wr.set<DeveloperMetadataLookup &, const char *>(*this, _DeveloperMetadataVisibility[value].text, buf, bufSize, 7, FPSTR(__func__)); }
    };

    /**
     * Filter that describes what data should be selected or returned from a request.
     */
    class DataFilter : public BaseG1
    {
    public:
        DataFilter() = default;

        // Union field filter.
        // Selects data associated with the developer metadata matching the criteria described by this DeveloperMetadataLookup.
        DataFilter &developerMetadataLookup(const DeveloperMetadataLookup &value) { return wr.add<DataFilter &, DeveloperMetadataLookup>(*this, value, buf, FPSTR(__func__)); }

        // Union field filter.
        // Selects data that matches the specified A1 range.
        DataFilter &a1Range(const String &value) { return wr.add<DataFilter &, String>(*this, value, buf, FPSTR(__func__)); }

        // Union field filter.
        // Selects data that matches the range described by the GridRange.
        DataFilter &gridRange(const GridRange &value) { return wr.add<DataFilter &, GridRange>(*this, value, buf, FPSTR(__func__)); }
    };

}
#endif