#ifndef CELL_FORMAT_H
#define CELL_FORMAT_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Theme.h"

/**
 * CELL FORMAT CLASS DEPENDENCIES
 * 
 * CellFormat   +   NumberFormat            +   (enum) NumberFormatType
 *              |
 *              +   ColorStyle*
 *              |
 *              +   Borders                 +   Border                      +       (enum) Style
 *              |                                                           |
 *              |                                                           +       ColorStyle*
 *              +   Padding
 *              |
 *              +   (enum) HorizontalAlign
 *              |
 *              +   (enum) VerticalAlign
 *              |
 *              +   (enum) WrapStrategy
 *              |                                                   
 *              +   (enum) TextDirection
 *              |
 *              +   TextFormat              +   ColorStyle*
 *              |
 *              +   (enum) HyperlinkDisplayType
 *              |                                                   
 *              +   TextRotation
 * 
 * See Theme.h
*/

namespace GSHEET
{
    // The direction of text in a cell.
    enum TextDirection
    {
        TEXT_DIRECTION_UNSPECIFIED, //	The text direction is not specified. Do not use this.
        LEFT_TO_RIGHT,              //	The text direction of left-to-right was set by the user.
        RIGHT_TO_LEFT               //	The text direction of right-to-left was set by the user.
    };

    // The number format of the cell. In this documentation the locale is assumed to be en_US, but the actual format depends on the locale of the spreadsheet.
    enum NumberFormatType
    {
        NUMBER_FORMAT_TYPE_UNSPECIFIED, //	The number format is not specified and is based on the contents of the cell. Do not explicitly use this.
        _TEXT,                          //	Text formatting, e.g 1000.12
        NUMBER,                         //	Number formatting, e.g, 1,000.12
        PERCENT,                        //	Percent formatting, e.g 10.12%
        CURRENCY,                       //	Currency formatting, e.g $1,000.12
        DATE,                           //	Date formatting, e.g 9/26/2008
        TIME,                           //	Time formatting, e.g 3:59:00 PM
        DATE_TIME,                      //	Date+Time formatting, e.g 9/26/08 15:59:00
        SCIENTIFIC                      //                     //	Scientific number formatting, e.g 1.01E+03
    };

    // The style of a border.
    enum Style
    {
        STYLE_UNSPECIFIED, //	The style is not specified. Do not use this.
        DOTTED,            //	The border is dotted.
        DASHED,            //	The border is dashed.
        SOLID,             //	The border is a thin solid line.
        SOLID_MEDIUM,      //	The border is a medium solid line.
        SOLID_THICK,       //	The border is a thick solid line.
        NONE,              //	No border. Used only when updating a border in order to erase it.
        DOUBLE             //	The border is two solid lines.
    };

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

    // How to wrap text in a cell.
    enum WrapStrategy
    {
        WRAP_STRATEGY_UNSPECIFIED, //	The default value, do not use.
        OVERFLOW_CELL,             // Lines that are longer than the cell width will be written in the next cell over, so long as that cell is empty. If the next cell over is non-empty, this behaves the same as CLIP. The text will never wrap to the next line unless the user manually inserts a new line. Example:
        LEGACY_WRAP,               // This wrap strategy represents the old Google Sheets wrap strategy where words that are longer than a line are clipped rather than broken. This strategy is not supported on all platforms and is being phased out. Example:
                                   //
                                   //  | Cell has a |
                                   //  | loooooooooo| <- Word is clipped.
                                   //  | word.      |
        CLIP,                      // Lines that are longer than the cell width will be clipped. The text will never wrap to the next line unless the user manually inserts a new line. Example:
                                   //
                                   //  | First sentence. |
                                   //  | Manual newline t| <- Text is clipped
                                   //  | Next newline.   |
        WRAP                       // Words that are longer than a line are wrapped at the character level rather than clipped. Example:
                                   //
                                   //  | Cell has a |
                                   //  | loooooooooo| <- Word is broken.
                                   //  | ong word.  |
    };

    // Whether to explicitly render a hyperlink. If not specified, the hyperlink is linked.
    enum HyperlinkDisplayType
    {
        HYPERLINK_DISPLAY_TYPE_UNSPECIFIED, //	The default value: the hyperlink is rendered. Do not use this.
        LINKED,                             //	A hyperlink should be explicitly rendered.
        PLAIN_TEXT                          //	A hyperlink should not be rendered.
    };

    /**
     * The number format of a cell.
     */
    class NumberFormat : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        NumberFormat &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        NumberFormat() {}
        // The type of the number format. When writing, this field must be set.
        NumberFormat &type(NumberFormatType value)
        {
            if (value == NUMBER_FORMAT_TYPE_UNSPECIFIED)
                return setObject(buf[1], "type", "NUMBER_FORMAT_TYPE_UNSPECIFIED", true, true);
            else if (value == _TEXT)
                return setObject(buf[1], "type", "TEXT", true, true);
            else if (value == NUMBER)
                return setObject(buf[1], "type", "NUMBER", true, true);
            else if (value == PERCENT)
                return setObject(buf[1], "type", "PERCENT", true, true);
            else if (value == CURRENCY)
                return setObject(buf[1], "type", "CURRENCY", true, true);
            else if (value == DATE)
                return setObject(buf[1], "type", "DATE", true, true);
            else if (value == TIME)
                return setObject(buf[1], "type", "TIME", true, true);
            else if (value == DATE_TIME)
                return setObject(buf[1], "type", "DATE_TIME", true, true);
            else if (value == SCIENTIFIC)
                return setObject(buf[1], "type", "SCIENTIFIC", true, true);
            return *this;
        }
        // Pattern string used for formatting. If not set, a default pattern based on the user's locale will be used if necessary for the given type. See the Date and Number Formats guide for more information about the supported patterns.
        NumberFormat &pattern(const String &value) { return setObject(buf[2], "pattern", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A border along a cell.
     */
    class Border : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Border &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Border() {}
        // The style of the border.
        Border &style(Style value)
        {
            if (value == STYLE_UNSPECIFIED)
                return setObject(buf[1], "style", "STYLE_UNSPECIFIED", true, true);
            else if (value == DOTTED)
                return setObject(buf[1], "style", "DOTTED", true, true);
            else if (value == DASHED)
                return setObject(buf[1], "style", "DASHED", true, true);
            else if (value == SOLID)
                return setObject(buf[1], "style", "SOLID", true, true);
            else if (value == SOLID_MEDIUM)
                return setObject(buf[1], "style", "SOLID_MEDIUM", true, true);
            else if (value == SOLID_THICK)
                return setObject(buf[1], "style", "SOLID_THICK", true, true);
            else if (value == NONE)
                return setObject(buf[1], "style", "NONE", true, true);
            else if (value == DOUBLE)
                return setObject(buf[1], "style", "DOUBLE", true, true);
            return *this;
        }
        // The color of the border. If color is also set, this field takes precedence.
        Border &colorStyle(const ColorStyle &value) { return setObject(buf[2], "colorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    class Borders : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Borders &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Borders() {}
        // The top border of the cell.
        Borders &top(const Border &value) { return setObject(buf[1], "top", value.c_str(), false, true); }
        // The bottom border of the cell.
        Borders &bottom(const Border &value) { return setObject(buf[2], "bottom", value.c_str(), false, true); }
        // The left border of the cell.
        Borders &left(const Border &value) { return setObject(buf[3], "left", value.c_str(), false, true); }
        // The right border of the cell.
        Borders &right(const Border &value) { return setObject(buf[4], "right", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The amount of padding around the cell, in pixels. When updating padding, every field must be specified.
     */
    class Padding : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Padding &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Padding() {}
        // The top padding of the cell.
        Padding &top(int value) { return setObject(buf[1], "top", String(value), false, true); }
        // The right padding of the cell.
        Padding &right(int value) { return setObject(buf[2], "right", String(value), false, true); }
        // The bottom padding of the cell.
        Padding &bottom(int value) { return setObject(buf[3], "bottom", String(value), false, true); }
        // The left padding of the cell.
        Padding &left(int value) { return setObject(buf[4], "left", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * An external or local reference.
     */
    class Link : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        /**
         * @param uri The link identifier.
         * */
        Link(const String &uri) { jut.addObject(buf, "uri", uri, true, true); }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * The format of a run of text in a cell. Absent values indicate that the field isn't specified.
     */
    class TextFormat : public Printable
    {
    private:
        size_t bufSize = 9;
        String buf[9];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        TextFormat &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        TextFormat() {}
        // The foreground color of the text.
        TextFormat &foregroundColorStyle(const ColorStyle &value) { return setObject(buf[1], "foregroundColorStyle", value.c_str(), false, true); }
        // The font family.
        TextFormat &fontFamily(const String &value) { return setObject(buf[2], "fontFamily", value, true, true); }
        // The size of the font.
        TextFormat &fontSize(int value) { return setObject(buf[3], "fontSize", String(value), false, true); }
        // True if the text is bold.
        TextFormat &bold(bool value) { return setObject(buf[4], "bold", owriter.getBoolStr(value), false, true); }
        // True if the text is italicized.
        TextFormat &italic(bool value) { return setObject(buf[5], "italic", owriter.getBoolStr(value), false, true); }
        // True if the text has a strikethrough.
        TextFormat &strikethrough(bool value) { return setObject(buf[6], "strikethrough", owriter.getBoolStr(value), false, true); }
        // True if the text is underlined.
        TextFormat &underline(bool value) { return setObject(buf[7], "underline", owriter.getBoolStr(value), false, true); }
        // The link destination of the text, if any. Setting the link field in a TextFormatRun will clear the cell's existing links or a cell-level link set in the same request. When a link is set, the text foreground color will be set to the default link color and the text will be underlined. If these fields are modified in the same request, those values will be used instead of the link defaults.
        TextFormat &link(const Link &value) { return setObject(buf[8], "link", value.c_str(), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    class TextRotation : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        TextRotation &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        TextRotation() {}
        // The angle between the standard orientation and the desired orientation. Measured in degrees. Valid values are between -90 and 90. Positive angles are angled upwards, negative are angled downwards.
        // Note: For LTR text direction positive angles are in the counterclockwise direction, whereas for RTL they are in the clockwise direction
        TextRotation &angle(int value)
        {
            // Union field type
            if (buf[2].length() == 0)
                return setObject(buf[1], "angle", String(value), false, true);
            return *this;
        }

        // If true, text reads top to bottom, but the orientation of individual characters is unchanged. For example:
        //
        //  | V |
        //  | e |
        //  | r |
        //  | t |
        //  | i |
        //  | c |
        //  | a |
        //  | l |
        TextRotation &vertical(bool value)
        {
            // Union field type
            if (buf[1].length() == 0)
                return setObject(buf[1], "vertical", owriter.getBoolStr(value), false, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The format of a cell.
     */
    class CellFormat : public Printable
    {
    private:
        size_t bufSize = 13;
        String buf[13];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CellFormat &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CellFormat() {}
        CellFormat &numberFormat(const NumberFormat &value) { return setObject(buf[1], "numberFormat", value.c_str(), false, true); }
        CellFormat &backgroundColor(const Color &value) { return setObject(buf[2], "backgroundColor", value.c_str(), false, true); }
        CellFormat &backgroundColorStyle(const ColorStyle &value) { return setObject(buf[3], "backgroundColorStyle", value.c_str(), false, true); }
        CellFormat &borders(const Borders &value) { return setObject(buf[4], "borders", value.c_str(), false, true); }
        CellFormat &padding(const Padding &value) { return setObject(buf[5], "padding", value.c_str(), false, true); }
        CellFormat &horizontalAlignment(HorizontalAlign value)
        {
            if (value == HORIZONTAL_ALIGN_UNSPECIFIED)
                return setObject(buf[6], "horizontalAlignment", "HORIZONTAL_ALIGN_UNSPECIFIED", true, true);
            else if (value == LEFT)
                return setObject(buf[6], "horizontalAlignment", "LEFT", true, true);
            else if (value == CENTER)
                return setObject(buf[6], "horizontalAlignment", "CENTER", true, true);
            else if (value == RIGHT)
                return setObject(buf[6], "horizontalAlignment", "RIGHT", true, true);
            return *this;
        }
        CellFormat &verticalAlignment(VerticalAlign value)
        {
            if (value == VERTICAL_ALIGN_UNSPECIFIED)
                return setObject(buf[7], "verticalAlignment", "VERTICAL_ALIGN_UNSPECIFIED", true, true);
            else if (value == TOP)
                return setObject(buf[7], "verticalAlignment", "TOP", true, true);
            else if (value == MIDDLE)
                return setObject(buf[7], "verticalAlignment", "MIDDLE", true, true);
            else if (value == BOTTOM)
                return setObject(buf[7], "verticalAlignment", "BOTTOM", true, true);
            return *this;
        }
        CellFormat &wrapStrategy(WrapStrategy value)
        {
            if (value == WRAP_STRATEGY_UNSPECIFIED)
                return setObject(buf[8], "wrapStrategy", "WRAP_STRATEGY_UNSPECIFIED", true, true);
            else if (value == OVERFLOW_CELL)
                return setObject(buf[8], "wrapStrategy", "OVERFLOW_CELL", true, true);
            else if (value == LEGACY_WRAP)
                return setObject(buf[8], "wrapStrategy", "LEGACY_WRAP", true, true);
            else if (value == CLIP)
                return setObject(buf[8], "wrapStrategy", "CLIP", true, true);
            else if (value == WRAP)
                return setObject(buf[8], "wrapStrategy", "WRAP", true, true);
            return *this;
        }
        CellFormat &textDirection(TextDirection value)
        {
            if (value == TEXT_DIRECTION_UNSPECIFIED)
                return setObject(buf[9], "textDirection", "TEXT_DIRECTION_UNSPECIFIED", true, true);
            else if (value == LEFT_TO_RIGHT)
                return setObject(buf[9], "textDirection", "LEFT_TO_RIGHT", true, true);
            else if (value == RIGHT_TO_LEFT)
                return setObject(buf[9], "textDirection", "RIGHT_TO_LEFT", true, true);
            return *this;
        }
        CellFormat &textFormat(const TextFormat &value) { return setObject(buf[10], "textFormat", value.c_str(), false, true); }
        CellFormat &hyperlinkDisplayType(HyperlinkDisplayType value)
        {
            if (value == HYPERLINK_DISPLAY_TYPE_UNSPECIFIED)
                return setObject(buf[11], "hyperlinkDisplayType", "HYPERLINK_DISPLAY_TYPE_UNSPECIFIED", true, true);
            else if (value == LINKED)
                return setObject(buf[11], "hyperlinkDisplayType", "LINKED", true, true);
            else if (value == PLAIN_TEXT)
                return setObject(buf[11], "hyperlinkDisplayType", "PLAIN_TEXT", true, true);
            return *this;
        }
        CellFormat &textRotation(const TextRotation &value) { return setObject(buf[12], "textRotation", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}
#endif
