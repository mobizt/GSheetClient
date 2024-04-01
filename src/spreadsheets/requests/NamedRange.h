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

}
#endif