#ifndef TEXT_POSITION_H
#define TEXT_POSITION_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
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

    /**
     * Position settings for text.
     */
    class TextPosition : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        TextPosition() {}
        // Horizontal alignment setting for the piece of text.
        TextPosition &horizontalAlignment(HorizontalAlign value)
        {
            clear();
            if (value == HORIZONTAL_ALIGN_UNSPECIFIED)
                jut.addObject(buf, "horizontalAlignment", "HORIZONTAL_ALIGN_UNSPECIFIED", true, true);
            else if (value == LEFT)
                jut.addObject(buf, "horizontalAlignment", "LEFT", true, true);
            else if (value == CENTER)
                jut.addObject(buf, "horizontalAlignment", "CENTER", true, true);
            else if (value == RIGHT)
                jut.addObject(buf, "horizontalAlignment", "RIGHT", true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

}

#endif