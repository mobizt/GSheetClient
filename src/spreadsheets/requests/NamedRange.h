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
    class NamedRange : public O4
    {

    public:
        NamedRange() {}
        // The ID of the named range.
        NamedRange &namedRangeId(const String &value) { return wr.set<NamedRange &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The name of the named range.
        NamedRange &name(const String &value) { return wr.set<NamedRange &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The range this represents.
        NamedRange &range(const GridRange &value) { return wr.set<NamedRange &, GridRange>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };

}
#endif