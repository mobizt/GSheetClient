#ifndef NAMED_RANGE_H
#define NAMED_RANGE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/GridRange.h"

namespace GSHEET
{
    /**
     * A named range.
     */
    class NamedRange : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        NamedRange &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        NamedRange() {}
        // The ID of the named range.
        NamedRange &namedRangeId(const String &value) { return setObject(buf[1], "namedRangeId", value, true, true); }
        // The name of the named range.
        NamedRange &name(const String &value) { return setObject(buf[2], "name", value, true, true); }
        // The range this represents.
        NamedRange &range(const GridRange &value) { return setObject(buf[3], "range", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates properties of the named range with the specified namedRangeId.
     */
    class UpdateNamedRangeRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateNamedRangeRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateNamedRangeRequest() {}
        // The named range to update with the new properties.
        UpdateNamedRangeRequest &namedRange(const String &value) { return setObject(buf[1], "namedRange", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root namedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateNamedRangeRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Adds a named range to the spreadsheet.
     */
    class AddNamedRangeRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        AddNamedRangeRequest() {}
        // The named range to add. The namedRangeId field is optional; if one is not set, an id will be randomly generated. (It is an error to specify the ID of a range that already exists.)
        AddNamedRangeRequest &name(const NamedRange &value)
        {
            clear();
            jut.addObject(buf, "namedRange", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };
    /**
     * Removes the named range with the given ID from the spreadsheet.
     */
    class DeleteNamedRangeRequest : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DeleteNamedRangeRequest() {}
        // The ID of the named range to delete.
        DeleteNamedRangeRequest &namedRangeId(const String &value)
        {
            clear();
            jut.addObject(buf, "namedRangeId", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

}
#endif