#ifndef TEXT_POSITION_H
#define TEXT_POSITION_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Common.h"

namespace TextAlignment
{
    // The horizontal alignment of text in a cell.
    enum HorizontalAlign
    {
        HORIZONTAL_ALIGN_UNSPECIFIED, //	The horizontal alignment is not specified. Do not use this.
        LEFT,                         //	The text is explicitly aligned to the left of the cell.
        CENTER,                       //	The text is explicitly aligned to the center of the cell.
        RIGHT                         //	The text is explicitly aligned to the right of the cell.
    };

    // The vertical alignment of text in a cell.
    enum VerticalAlign
    {
        VERTICAL_ALIGN_UNSPECIFIED, //	The vertical alignment is not specified. Do not use this.
        TOP,                        //	The text is explicitly aligned to the top of the cell.
        MIDDLE,                     //	The text is explicitly aligned to the middle of the cell.
        BOTTOM                      //	The text is explicitly aligned to the bottom of the cell.
    };

    const struct GSHEET::key_str_30 _HorizontalAlign[HorizontalAlign::RIGHT + 1] PROGMEM = {
        "HORIZONTAL_ALIGN_UNSPECIFIED",
        "LEFT",
        "CENTER",
        "RIGHT"};

    const struct GSHEET::key_str_30 _VerticalAlign[VerticalAlign::BOTTOM + 1] PROGMEM = {
        "VERTICAL_ALIGN_UNSPECIFIED",
        "TOP",
        "MIDDLE",
        "BOTTOM"};

}

namespace GSHEET
{

    /**
     * Position settings for text.
     */
    class TextPosition : public O1
    {
    public:
        TextPosition() {}
        // Horizontal alignment setting for the piece of text.
        TextPosition &horizontalAlignment(TextAlignment::HorizontalAlign value) { return wr.add<TextPosition &, const char *>(*this, TextAlignment::_HorizontalAlign[value].text, buf, FPSTR(__func__)); }
    };

}

#endif