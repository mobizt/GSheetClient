#ifndef CELL_FORMAT_H
#define CELL_FORMAT_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/TextPosition.h"

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

    /**
     * The number format of a cell.
     */
    class NumberFormat : public BaseG4
    {

    public:
        NumberFormat() {}

        // The type of the number format. When writing, this field must be set.
        NumberFormat &type(Format::NumberFormatType value) { return wr.set<NumberFormat &, const char *>(*this, _NumberFormatType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // Pattern string used for formatting. If not set, a default pattern based on the user's locale will be used if necessary for the given type. See the Date and Number Formats guide for more information about the supported patterns.
        NumberFormat &pattern(const String &value) { return wr.set<NumberFormat &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A border along a cell.
     */
    class Border : public BaseG4
    {
    public:
        Border() {}

        // The style of the border.
        Border &style(Line::Style value) { return wr.set<Border &, const char *>(*this, _Style[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The color of the border. If color is also set, this field takes precedence.
        Border &colorStyle(const ColorStyle &value) { return wr.set<Border &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    class Borders : public BaseG6
    {

    public:
        Borders() {}

        // The top border of the cell.
        Borders &top(const Border &value) { return wr.set<Borders &, Border>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The bottom border of the cell.
        Borders &bottom(const Border &value) { return wr.set<Borders &, Border>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The left border of the cell.
        Borders &left(const Border &value) { return wr.set<Borders &, Border>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The right border of the cell.
        Borders &right(const Border &value) { return wr.set<Borders &, Border>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * The amount of padding around the cell, in pixels. When updating padding, every field must be specified.
     */
    class Padding : public BaseG6
    {

    public:
        Padding() {}

        // The top padding of the cell.
        Padding &top(int value) { return wr.set<Padding &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The right padding of the cell.
        Padding &right(int value) { return wr.set<Padding &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The bottom padding of the cell.
        Padding &bottom(int value) { return wr.set<Padding &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The left padding of the cell.
        Padding &left(int value) { return wr.set<Padding &, int>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * An external or local reference.
     */
    class Link : public BaseG1
    {
    public:
        Link() {}

        // The link identifier.
        Link &uri(const String &value) { return wr.add<Link &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * The format of a run of text in a cell. Absent values indicate that the field isn't specified.
     */
    class TextFormat : public BaseG12
    {

    public:
        TextFormat() {}

        // The foreground color of the text.
        TextFormat &foregroundColorStyle(const ColorStyle &value) { return wr.set<TextFormat &, ColorStyle>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The font family.
        TextFormat &fontFamily(const String &value) { return wr.set<TextFormat &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The size of the font.
        TextFormat &fontSize(int value) { return wr.set<TextFormat &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if the text is bold.
        TextFormat &bold(bool value) { return wr.set<TextFormat &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // True if the text is italicized.
        TextFormat &italic(bool value) { return wr.set<TextFormat &, bool>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // True if the text has a strikethrough.
        TextFormat &strikethrough(bool value) { return wr.set<TextFormat &, bool>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // True if the text is underlined.
        TextFormat &underline(bool value) { return wr.set<TextFormat &, bool>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // The link destination of the text, if any. Setting the link field in a TextFormatRun will clear the cell's existing links or a cell-level link set in the same request. When a link is set, the text foreground color will be set to the default link color and the text will be underlined. If these fields are modified in the same request, those values will be used instead of the link defaults.
        TextFormat &link(const Link &value) { return wr.set<TextFormat &, Link>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }
    };

    class TextRotation : public BaseG1
    {
    public:
        TextRotation() {}

        // Union field type
        // The angle between the standard orientation and the desired orientation. Measured in degrees. Valid values are between -90 and 90. Positive angles are angled upwards, negative are angled downwards.
        // Note: For LTR text direction positive angles are in the counterclockwise direction, whereas for RTL they are in the clockwise direction
        TextRotation &angle(int value) { return wr.add<TextRotation &, int>(*this, value, buf, FPSTR(__func__)); }

        // Union field type
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
        TextRotation &vertical(bool value) { return wr.add<TextRotation &, bool>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * The format of a cell.
     */
    class CellFormat : public BaseG12
    {
    public:
        CellFormat() {}

        // A format describing how number values should be represented to the user.
        CellFormat &numberFormat(const NumberFormat &value) { return wr.set<CellFormat &, NumberFormat>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The background color of the cell. If backgroundColor is also set, this field takes precedence.
        CellFormat &backgroundColorStyle(const ColorStyle &value) { return wr.set<CellFormat &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The borders of the cell.
        CellFormat &borders(const Borders &value) { return wr.set<CellFormat &, Borders>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // The padding of the cell.
        CellFormat &padding(const Padding &value) { return wr.set<CellFormat &, Padding>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // The horizontal alignment of the value in the cell.
        CellFormat &horizontalAlignment(TextAlignment::HorizontalAlign value) { return wr.set<CellFormat &, const char *>(*this, TextAlignment::_HorizontalAlign[value].text, buf, bufSize, 5, FPSTR(__func__)); }

        // The vertical alignment of the value in the cell.
        CellFormat &verticalAlignment(TextAlignment::VerticalAlign value) { return wr.set<CellFormat &, const char *>(*this, TextAlignment::_VerticalAlign[value].text, buf, bufSize, 6, FPSTR(__func__)); }

        // The wrap strategy for the value in the cell.
        CellFormat &wrapStrategy(Cell::WrapStrategy value) { return wr.set<CellFormat &, const char *>(*this, _WrapStrategy[value].text, buf, bufSize, 7, FPSTR(__func__)); }

        // The direction of the text in the cell.
        CellFormat &textDirection(Direction::TextDirection value) { return wr.set<CellFormat &, const char *>(*this, _TextDirection[value].text, buf, bufSize, 8, FPSTR(__func__)); }

        // The format of the text in the cell (unless overridden by a format run). Setting a cell-level link here clears the cell's existing links. Setting the link field in a TextFormatRun takes precedence over the cell-level link.
        CellFormat &textFormat(const TextFormat &value) { return wr.set<CellFormat &, TextFormat>(*this, value, buf, bufSize, 9, FPSTR(__func__)); }

        // If one exists, how a hyperlink should be displayed in the cell.
        CellFormat &hyperlinkDisplayType(Display::HyperlinkDisplayType value) { return wr.set<CellFormat &, TextFormat>(*this, _HyperlinkDisplayType[value].text, buf, bufSize, 10, FPSTR(__func__)); }

        // The rotation applied to text in the cell.
        CellFormat &textRotation(const TextRotation &value) { return wr.set<CellFormat &, TextRotation>(*this, value, buf, bufSize, 11, FPSTR(__func__)); }
    };

}
#endif
