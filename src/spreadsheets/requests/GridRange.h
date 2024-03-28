#ifndef GRID_RANGE_H
#define GRID_RANGE_H
#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{
    
    /**
     * A range on a sheet. All indexes are zero-based. Indexes are half open, i.e. the start index is inclusive and the end index is exclusive -- [startIndex, endIndex). Missing indexes indicate the range is unbounded on that side.
     */
    class GridRange : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GridRange &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GridRange() {}
        // The sheet this range is on.
        GridRange &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        // The start row (inclusive) of the range, or not set if unbounded.
        GridRange &startRowIndex(int value) { return setObject(buf[2], "startRowIndex", String(value), false, true); }
        // The end row (exclusive) of the range, or not set if unbounded.
        GridRange &endRowIndex(int value) { return setObject(buf[3], "endRowIndex", String(value), false, true); }
        // The start column (inclusive) of the range, or not set if unbounded.
        GridRange &startColumnIndex(int value) { return setObject(buf[4], "startColumnIndex", String(value), false, true); }
        // The end column (exclusive) of the range, or not set if unbounded.
        GridRange &endColumnIndex(int value) { return setObject(buf[5], "endColumnIndex", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}

#endif