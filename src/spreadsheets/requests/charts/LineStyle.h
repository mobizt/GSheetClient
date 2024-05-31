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
        CUSTOM_DASH,                //	A custom dash for a line. Modifying the exact custom dash style is currently unsupported.
        SOLID_LINE,                 //	A solid line.
        DOTTED_LINE,                //	A dotted line.
        MEDIUM_DASHED,              //	A dashed line where the dashes have "medium" length.
        MEDIUM_DASHED_DOTTED,       //	A line that alternates between a "medium" dash and a dot.
        LONG_DASHED,                //	A dashed line where the dashes have "long" length.
        LONG_DASHED_DOTTED          //	A line that alternates between a "long" dash and a dot.

    };

    const struct key_str_40 _LineDashType[LineDashType::LONG_DASHED_DOTTED + 1] PROGMEM = {"LINE_DASH_TYPE_UNSPECIFIED", "INVISIBLE", "CUSTOM_DASH", "SOLID_LINE", "DOTTED_LINE", "MEDIUM_DASHED", "MEDIUM_DASHED_DOTTED", "LONG_DASHED", "LONG_DASHED_DOTTED"};

    /**
     * Properties that describe the style of a line.
     */
    class LineStyle : public BaseG4
    {

    public:
        LineStyle() {}

        // The thickness of the line, in px.
        LineStyle &width(int value) { return wr.set<LineStyle &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The dash type of the line.
        LineStyle &type(LineDashType value) { return wr.set<LineStyle &, const char *>(*this, _LineDashType[value].text, buf, bufSize, 2, FPSTR(__func__)); }
    };

}

#endif