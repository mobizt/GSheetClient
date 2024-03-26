#ifndef CELL_DATA_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{

    class CellData : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CellData &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CellData() {}
        CellData &userEnteredValue(const ExtendedValue &value) { return setObject(buf[1], "userEnteredValue", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root namedRange is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        CellData &effectiveValue(const String &value) { return setObject(buf[2], "effectiveValue", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}
#endif