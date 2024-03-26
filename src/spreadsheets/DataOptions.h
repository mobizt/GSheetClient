/**
 * Created March 24, 2024
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GSHEET_DATAOPTIONS_H
#define GSHEET_DATAOPTIONS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

#define GSHEET_RESOURCE_PATH_BASE FPSTR("<resource_path>")

enum gsheet_request_type
{
    gsheet_request_type_undefined,
    gsheet_request_type_rollback = 200,
    gsheet_request_type_begin_transaction,
    gsheet_request_type_commit_document,
    gsheet_request_type_batch_write_doc,
    gsheet_request_type_run_query,
    gsheet_request_type_list_collection,
    gsheet_request_type_export_docs,
    gsheet_request_type_import_docs,
    gsheet_request_type_create_doc,
    gsheet_request_type_batch_get_doc,
    gsheet_request_type_create_composite_index,
    gsheet_request_type_create_field_index,
    gsheet_request_type_manage_database,

    gsheet_request_type_get_doc = 300,
    gsheet_request_type_list_doc,
    gsheet_request_type_list_index,
    gsheet_request_type_get_index,

    gsheet_request_type_patch_doc = 400,

    gsheet_request_type_delete_doc = 500,
    gsheet_request_type_delete_index
};

namespace GSHEET
{
    // An enumeration of the possible recalculation interval options.
    enum RecalculationInterval
    {
        RECALCULATION_INTERVAL_UNSPECIFIED, //	Default value. This value must not be used.
        ON_CHANGE,                          //	Volatile functions are updated on every change.
        MINUTE,                             //	Volatile functions are updated on every change and every minute.
        HOUR                                //	Volatile functions are updated on every change and hourly.
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

    // The direction of text in a cell.
    enum TextDirection
    {
        TEXT_DIRECTION_UNSPECIFIED, //	The text direction is not specified. Do not use this.
        LEFT_TO_RIGHT,              //	The text direction of left-to-right was set by the user.
        RIGHT_TO_LEFT               //	The text direction of right-to-left was set by the user.
    };

    // Theme color types.
    // SpreadsheetProperties contain a SpreadsheetTheme that defines a mapping of these theme color types to concrete colors.
    enum ThemeColorType
    {
        THEME_COLOR_TYPE_UNSPECIFIED, //	Unspecified theme color
        TEXT,                         //	Represents the primary text color
        BACKGROUND,                   //	Represents the primary background color
        ACCENT1,                      //	Represents the first accent color
        ACCENT2,                      //	Represents the second accent color
        ACCENT3,                      //	Represents the third accent color
        ACCENT4,                      //	Represents the fourth accent color
        ACCENT5,                      //	Represents the fifth accent color
        ACCENT6,                      //	Represents the sixth accent color
        LINK                          //	Represents the color to use for hyperlinks
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

    // Whether to explicitly render a hyperlink. If not specified, the hyperlink is linked.
    enum HyperlinkDisplayType
    {
        HYPERLINK_DISPLAY_TYPE_UNSPECIFIED, //	The default value: the hyperlink is rendered. Do not use this.
        LINKED,                             //	A hyperlink should be explicitly rendered.
        PLAIN_TEXT                          //	A hyperlink should not be rendered.
    };
    // The kind of sheet
    enum SheetType
    {
        SHEET_TYPE_UNSPECIFIED, //	Default value, do not use.
        GRID,                   //	The sheet is a grid.
        OBJECT,                 //	The sheet has no grid and instead has an object like a chart or image.
        DATA_SOURCE             //	The sheet connects with an external DataSource and shows the preview of data.
    };

    /**
     * Represents a color in the RGBA color space. This representation is designed for simplicity of conversion to/from color representations in various languages over compactness.
     */
    struct Color : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        Color &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        // The amount of red in the color as a value in the interval [0, 1].
        Color &red(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[1], "red", String(value), false, true);
        }
        // The amount of green in the color as a value in the interval [0, 1].
        Color &green(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[2], "green", String(value), false, true);
        }
        // The amount of blue in the color as a value in the interval [0, 1].
        Color &blue(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[3], "blue", String(value), false, true);
        }
        // The fraction of this color that should be applied to the pixel. That is, the final pixel color is defined by the equation:
        // pixel color = alpha * (this color) + (1.0 - alpha) * (background color)
        // This means that a value of 1.0 corresponds to a solid color, whereas a value of 0.0 corresponds to a completely transparent color.
        Color &alpha(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[4], "alpha", String(value), false, true);
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A color value.
     */
    class ColorStyle : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ColorStyle &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ColorStyle() {}
        // RGB color. The alpha value in the Color object isn't generally supported.
        ColorStyle &rgbColor(Color value)
        {
            // / Union field kind
            if (buf[2].length() == 0)
                return setObject(buf[1], "rgbColor", value.c_str(), false, true);
            return *this;
        }
        // When iterative calculation is enabled and successive results differ by less than this threshold value, the calculation rounds stop.
        ColorStyle &themeColor(ThemeColorType value)
        {
            // Union field kind
            if (buf[1].length() == 0)
            {
                if (value == THEME_COLOR_TYPE_UNSPECIFIED)
                    return setObject(buf[2], "themeColor", "THEME_COLOR_TYPE_UNSPECIFIED", true, true);
                else if (value == TEXT)
                    return setObject(buf[2], "themeColor", "TEXT", true, true);
                else if (value == BACKGROUND)
                    return setObject(buf[2], "themeColor", "BACKGROUND", true, true);
                else if (value == ACCENT1)
                    return setObject(buf[2], "themeColor", "ACCENT1", true, true);
                else if (value == ACCENT2)
                    return setObject(buf[2], "themeColor", "ACCENT2", true, true);
                else if (value == ACCENT3)
                    return setObject(buf[2], "themeColor", "ACCENT3", true, true);
                else if (value == ACCENT4)
                    return setObject(buf[2], "themeColor", "ACCENT4", true, true);
                else if (value == ACCENT5)
                    return setObject(buf[2], "themeColor", "ACCENT5", true, true);
                else if (value == ACCENT6)
                    return setObject(buf[2], "themeColor", "ACCENT6", true, true);
                else if (value == LINK)
                    return setObject(buf[2], "themeColor", "LINK", true, true);
            }
            return *this;
        }
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
    /**
     * A pair mapping a spreadsheet theme color type to the concrete color it represents.
     */
    class ThemeColorPair : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ThemeColorPair &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ThemeColorPair() {}
        // The type of the spreadsheet theme color.
        ThemeColorPair &colorType(ThemeColorType value)
        {
            if (value == THEME_COLOR_TYPE_UNSPECIFIED)
                return setObject(buf[1], "colorType", "THEME_COLOR_TYPE_UNSPECIFIED", true, true);
            else if (value == TEXT)
                return setObject(buf[1], "colorType", "TEXT", true, true);
            else if (value == BACKGROUND)
                return setObject(buf[1], "colorType", "BACKGROUND", true, true);
            else if (value == ACCENT1)
                return setObject(buf[1], "colorType", "ACCENT1", true, true);
            else if (value == ACCENT2)
                return setObject(buf[1], "colorType", "ACCENT2", true, true);
            else if (value == ACCENT3)
                return setObject(buf[1], "colorType", "ACCENT3", true, true);
            else if (value == ACCENT4)
                return setObject(buf[1], "colorType", "ACCENT4", true, true);
            else if (value == ACCENT5)
                return setObject(buf[1], "colorType", "ACCENT5", true, true);
            else if (value == ACCENT6)
                return setObject(buf[1], "colorType", "ACCENT6", true, true);
            else if (value == LINK)
                return setObject(buf[1], "colorType", "LINK", true, true);
            return *this;
        }
        // The concrete color corresponding to the theme color type.
        ThemeColorPair &color(const ColorStyle value) { return setObject(buf[2], "color", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Settings to control how circular dependencies are resolved with iterative calculation.
     */
    class IterativeCalculationSettings : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        IterativeCalculationSettings &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        IterativeCalculationSettings() {}
        // When iterative calculation is enabled, the maximum number of calculation rounds to perform.
        IterativeCalculationSettings &maxIterations(int value) { return setObject(buf[1], "maxIterations", String(value), false, true); }
        // When iterative calculation is enabled and successive results differ by less than this threshold value, the calculation rounds stop.
        IterativeCalculationSettings &convergenceThreshold(double value) { return setObject(buf[2], "convergenceThreshold", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Represents spreadsheet theme
     */
    class SpreadsheetTheme : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SpreadsheetTheme &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SpreadsheetTheme() {}
        // Name of the primary font family.
        SpreadsheetTheme &primaryFontFamily(const String &value) { return setObject(buf[1], "primaryFontFamily", value, true, true); }
        /** The spreadsheet theme color pairs. To update you must provide all theme color pairs.
         * The item or value will be added to the array or list.
         * To add more items, call this function again.
         */
        SpreadsheetTheme &themeColors(ThemeColorPair value)
        {

            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("themeColors"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Properties of a spreadsheet.
     */
    class SpreadsheetProperties : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SpreadsheetProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SpreadsheetProperties() {}
        // The title of the spreadsheet.
        SpreadsheetProperties &title(const String &value) { return setObject(buf[1], "title", value, true, true); }
        // The locale of the spreadsheet in one of the following formats:
        // an ISO 639-1 language code such as en
        // an ISO 639-2 language code such as fil, if no 639-1 code exists
        // a combination of the ISO language code and country code, such as en_US
        // Note: when updating this field, not all locales/languages are supported.
        SpreadsheetProperties &local(const String &value) { return setObject(buf[2], "local", value, true, true); }
        // The amount of time to wait before volatile functions are recalculated.
        SpreadsheetProperties &autoRecalc(RecalculationInterval value)
        {
            if (value == RECALCULATION_INTERVAL_UNSPECIFIED)
                return setObject(buf[3], "autoRecalc", "RECALCULATION_INTERVAL_UNSPECIFIED", true, true);
            else if (value == ON_CHANGE)
                return setObject(buf[3], "autoRecalc", "ON_CHANGE", true, true);
            else if (value == MINUTE)
                return setObject(buf[3], "autoRecalc", "MINUTE", true, true);
            else if (value == HOUR)
                return setObject(buf[3], "autoRecalc", "HOUR", true, true);
            return *this;
        }
        // The time zone of the spreadsheet, in CLDR format such as America/New_York. If the time zone isn't recognized, this may be a custom time zone such as GMT-07:00.
        SpreadsheetProperties &timeZone(const String &value) { return setObject(buf[4], "timeZone", value, true, true); }
        // The default format of all cells in the spreadsheet. CellData.effectiveFormat will not be set if the cell's format is equal to this default format. This field is read-only.
        SpreadsheetProperties &defaultFormat(const CellFormat &value) { return setObject(buf[5], "defaultFormat", value.c_str(), false, true); }
        // Determines whether and how circular references are resolved with iterative calculation. Absence of this field means that circular references result in calculation errors.
        SpreadsheetProperties &iterativeCalculationSettings(const IterativeCalculationSettings &value) { return setObject(buf[6], "iterativeCalculationSettings", value.c_str(), false, true); }
        // Theme applied to the spreadsheet.
        SpreadsheetProperties &spreadsheetTheme(const SpreadsheetTheme &value) { return setObject(buf[7], "spreadsheetTheme", value.c_str(), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates properties of a spreadsheet.
     */
    class UpdateSpreadsheetPropertiesRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateSpreadsheetPropertiesRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateSpreadsheetPropertiesRequest() {}
        // The properties to update.
        UpdateSpreadsheetPropertiesRequest &properties(const SpreadsheetProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root 'properties' is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSpreadsheetPropertiesRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Properties of a grid.
     */
    class GridProperties : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GridProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GridProperties() {}
        // The number of rows in the grid.
        GridProperties &rowCount(int value) { return setObject(buf[1], "rowCount", String(value), false, true); }
        // The number of columns in the grid.
        GridProperties &columnCount(int value) { return setObject(buf[2], "columnCount", String(value), false, true); }
        // The number of rows that are frozen in the grid.
        GridProperties &frozenRowCount(int value) { return setObject(buf[3], "frozenRowCount", String(value), false, true); }
        // The number of columns that are frozen in the grid.
        GridProperties &frozenColumnCount(int value) { return setObject(buf[4], "frozenColumnCount", String(value), false, true); }
        // True if the grid isn't showing gridlines in the UI.
        GridProperties &hideGridlines(bool value) { return setObject(buf[5], "hideGridlines", owriter.getBoolStr(value), false, true); }
        // True if the row grouping control toggle is shown after the group.
        GridProperties &rowGroupControlAfter(bool value) { return setObject(buf[6], "rowGroupControlAfter", owriter.getBoolStr(value), false, true); }
        // True if the column grouping control toggle is shown after the group.
        GridProperties &columnGroupControlAfter(bool value) { return setObject(buf[7], "columnGroupControlAfter", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Properties of a spreadsheet.
     */
    class SheetProperties : public Printable
    {
    private:
        size_t bufSize = 9;
        String buf[9];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SheetProperties &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SheetProperties() {}
        // The ID of the sheet. Must be non-negative. This field cannot be changed once set.
        SheetProperties &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        // The name of the sheet.
        SheetProperties &title(const String &value) { return setObject(buf[2], "title", value, true, true); }
        // The index of the sheet within the spreadsheet. When adding or updating sheet properties, if this field is excluded then the sheet is added or moved to the end of the sheet list. When updating sheet indices or inserting sheets, movement is considered in "before the move" indexes. For example, if there were three sheets (S1, S2, S3) in order to move S1 ahead of S2 the index would have to be set to 2. A sheet index update request is ignored if the requested index is identical to the sheets current index or if the requested new index is equal to the current sheet index + 1.
        SheetProperties &index(int value) { return setObject(buf[3], "index", String(value), false, true); }
        SheetProperties &sheetType(SheetType value)
        {

            if (value == SHEET_TYPE_UNSPECIFIED)
                return setObject(buf[4], "sheetType", "SHEET_TYPE_UNSPECIFIED", true, true);
            else if (value == GRID)
                return setObject(buf[4], "sheetType", "GRID", true, true);
            else if (value == OBJECT)
                return setObject(buf[4], "sheetType", "OBJECT", true, true);
            else if (value == DATA_SOURCE)
                return setObject(buf[4], "sheetType", "DATA_SOURCE", true, true);
            return *this;
        }
        // Additional properties of the sheet if this sheet is a grid. (If the sheet is an object sheet, containing a chart or image, then this field will be absent.) When writing it is an error to set any grid properties on non-grid sheets.
        // If this sheet is a DATA_SOURCE sheet, this field is output only but contains the properties that reflect how a data source sheet is rendered in the UI, e.g. rowCount.
        SheetProperties &gridProperties(const GridProperties &value) { return setObject(buf[5], "gridProperties", value.c_str(), true, true); }
        // True if the sheet is hidden in the UI, false if it's visible.
        SheetProperties &hidden(bool value) { return setObject(buf[6], "hidden", owriter.getBoolStr(value), false, true); }
        // The color of the tab in the UI.
        SheetProperties &tabColorStyle(const ColorStyle &value) { return setObject(buf[7], "tabColorStyle", value.c_str(), true, true); }
        // True if the sheet is an RTL sheet instead of an LTR sheet.
        SheetProperties &rightToLeft(bool value) { return setObject(buf[8], "rightToLeft", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Updates properties of the sheet with the specified sheetId.
     */
    class UpdateSheetPropertiesRequest : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        UpdateSheetPropertiesRequest &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        UpdateSheetPropertiesRequest() {}
        // The properties to update.
        UpdateSheetPropertiesRequest &properties(const SheetProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The fields that should be updated. At least one field must be specified. The root properties is implied and should not be specified. A single "*" can be used as short-hand for listing every field.
        UpdateSheetPropertiesRequest &fields(const String &value) { return setObject(buf[2], "fields", value, true, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    template <typename T>
    class Request : public Printable
    {
    private:
        size_t bufSize = 2;
        String buf[2];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Request &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Request(const T &value)
        {
            String name;
            if (std::is_same<T, SpreadsheetProperties>::value)
                name = FPSTR("updateSpreadsheetProperties");
            else if (std::is_same<T, UpdateSheetPropertiesRequest>::value)
                name = FPSTR("updateSheetProperties");
            // TODO
            //  ADD MORE REQUESTS

            if (name.length())
                setObject(buf[1], name, value.c_str(), false, true);
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Batch update options
     */
    class BatchUpdateOptions : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BatchUpdateOptions &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BatchUpdateOptions() {}
        // A list of updates to apply to the spreadsheet. Requests will be applied in the order they are specified. If any request is not valid, no requests will be applied.
        template <typename T>
        BatchUpdateOptions &addRequest(const Request<T> &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("values"), value.c_str(), false);
            return *this;
        }
        // Determines if the update response should include the spreadsheet resource.
        BatchUpdateOptions &includeSpreadsheetInResponse(bool value) { return setObject(buf[2], "includeSpreadsheetInResponse", owriter.getBoolStr(value), false, true); }
        // Limits the ranges included in the response spreadsheet. Meaningful only if includeSpreadsheetInResponse is 'true'.
        BatchUpdateOptions &addResponseRanges(const String &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("responseRanges"), value, true);
            return *this;
        }
        // True if grid data should be returned. Meaningful only if includeSpreadsheetInResponse is 'true'. This parameter is ignored if a field mask was set in the request.
        BatchUpdateOptions &responseIncludeGridData(bool value) { return setObject(buf[4], "responseIncludeGridData", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    class Parent
    {
        friend class GSheetBase;

    private:
        String spreadsheetId;
        String sheetId;

    public:
        Parent() {}
        Parent(const String &spreadsheetId, const String &sheetId = "")
        {
            this->spreadsheetId = spreadsheetId;
            this->sheetId = sheetId;
        }
        void setSpreadsheetId(const String &spreadsheetId) { this->spreadsheetId = spreadsheetId; }
        void setSheetId(const String &) { this->sheetId = sheetId; }
        String getSpreadsheetId() const { return spreadsheetId.c_str(); }
        String getSheetId() const { return sheetId.c_str(); }
    };

    class DataOptions
    {
        friend class GSheetBase;

    public:
        Parent parent;
        String spreadsheetId;
        String sheetId;
        String extras;
        String payload;
        gsheet_request_type requestType = gsheet_request_type_undefined;
        unsigned long requestTime = 0;

        void copy(DataOptions &rhs)
        {
            this->parent = rhs.parent;
            this->spreadsheetId = rhs.spreadsheetId;
            this->sheetId = rhs.sheetId;
            this->extras = rhs.extras;
            this->payload = rhs.payload;
        }

    private:
    };
}

#endif