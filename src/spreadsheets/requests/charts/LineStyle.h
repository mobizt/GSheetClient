#ifndef LINE_STYLE_H
#define LINE_STYLE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{
    // The dash type of a line.
    enum LineDashType
    {
        LINE_DASH_TYPE_UNSPECIFIED, //	Default value, do not use.
        INVISIBLE,                  //	No dash type, which is equivalent to a non-visible line.
        CUSTOM,                     //	A custom dash for a line. Modifying the exact custom dash style is currently unsupported.
        SOLID,                      //	A solid line.
        DOTTED,                     //	A dotted line.
        MEDIUM_DASHED,              //	A dashed line where the dashes have "medium" length.
        MEDIUM_DASHED_DOTTED,       //	A line that alternates between a "medium" dash and a dot.
        LONG_DASHED,                //	A dashed line where the dashes have "long" length.
        LONG_DASHED_DOTTED          //	A line that alternates between a "long" dash and a dot.

    };
    
    /**
     * Properties that describe the style of a line.
     */
    class LineStyle : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        LineStyle &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        LineStyle() {}
        // The thickness of the line, in px.
        LineStyle &width(int value) { return setObject(buf[1], "width", String(value), false, true); }
        // The dash type of the line.
        LineStyle &type(LineDashType value)
        {
            if (value == LINE_DASH_TYPE_UNSPECIFIED)
                return setObject(buf[2], "type", "LINE_DASH_TYPE_UNSPECIFIED", true, true);
            else if (value == INVISIBLE)
                return setObject(buf[2], "type", "INVISIBLE", true, true);
            else if (value == CUSTOM)
                return setObject(buf[2], "type", "CUSTOM", true, true);
            else if (value == SOLID)
                return setObject(buf[2], "type", "SOLID", true, true);
            else if (value == DOTTED)
                return setObject(buf[2], "type", "DOTTED", true, true);
            else if (value == MEDIUM_DASHED)
                return setObject(buf[2], "type", "MEDIUM_DASHED", true, true);
            else if (value == MEDIUM_DASHED_DOTTED)
                return setObject(buf[2], "type", "MEDIUM_DASHED_DOTTED", true, true);
            else if (value == LONG_DASHED)
                return setObject(buf[2], "type", "LONG_DASHED", true, true);
            else if (value == LONG_DASHED_DOTTED)
                return setObject(buf[2], "type", "LONG_DASHED_DOTTED", true, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}

#endif