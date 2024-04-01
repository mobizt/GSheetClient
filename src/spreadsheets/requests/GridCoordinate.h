#ifndef GRID_COORDINATE_H
#define GRID_COORDINATE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{

    /**
     * A coordinate in a sheet. All indexes are zero-based.
     */
    class GridCoordinate : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GridCoordinate &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GridCoordinate() {}
        // The sheet this coordinate is on.
        GridCoordinate &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        // The row index of the coordinate.
        GridCoordinate &rowIndex(int value) { return setObject(buf[2], "rowIndex", String(value), false, true); }
        // The column index of the coordinate.
        GridCoordinate &columnIndex(int value) { return setObject(buf[3], "columnIndex", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}
#endif