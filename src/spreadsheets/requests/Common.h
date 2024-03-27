#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

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

    // Controls how a date condition is evaluated.
    enum RelativeDate
    {
        RELATIVE_DATE_UNSPECIFIED, //	Default value, do not use.
        PAST_YEAR,                 //	The value is one year before today.
        PAST_MONTH,                //	The value is one month before today.
        PAST_WEEK,                 //	The value is one week before today.
        YESTERDAY,                 //	The value is yesterday.
        TODAY,                     //	The value is today.
        TOMORROW                   //	The value is tomorrow.
    };

    // The type of condition.
    enum ConditionType
    {
        CONDITION_TYPE_UNSPECIFIED, //	The default value, do not use.
        NUMBER_GREATER,             //	The cell's value must be greater than the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue.
        NUMBER_GREATER_THAN_EQ,     //	The cell's value must be greater than or equal to the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue.
        NUMBER_LESS,                //	The cell's value must be less than the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue.
        NUMBER_LESS_THAN_EQ,        //	The cell's value must be less than or equal to the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue.
        NUMBER_EQ,                  //	The cell's value must be equal to the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue for data validation, conditional formatting, and filters on non-data source objects and at least one ConditionValue for filters on data source objects.
        NUMBER_NOT_EQ,              //	The cell's value must be not equal to the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue for data validation, conditional formatting, and filters on non-data source objects and at least one ConditionValue for filters on data source objects.
        NUMBER_BETWEEN,             //	The cell's value must be between the two condition values. Supported by data validation, conditional formatting and filters. Requires exactly two ConditionValues.
        NUMBER_NOT_BETWEEN,         //	The cell's value must not be between the two condition values. Supported by data validation, conditional formatting and filters. Requires exactly two ConditionValues.
        TEXT_CONTAINS,              //	The cell's value must contain the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue.
        TEXT_NOT_CONTAINS,          //	The cell's value must not contain the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue.
        TEXT_STARTS_WITH,           //	The cell's value must start with the condition's value. Supported by conditional formatting and filters. Requires a single ConditionValue.
        TEXT_ENDS_WITH,             //	The cell's value must end with the condition's value. Supported by conditional formatting and filters. Requires a single ConditionValue.
        TEXT_EQ,                    //	The cell's value must be exactly the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue for data validation, conditional formatting, and filters on non-data source objects and at least one ConditionValue for filters on data source objects.
        TEXT_IS_EMAIL,              //	The cell's value must be a valid email address. Supported by data validation. Requires no ConditionValues.
        TEXT_IS_URL,                //	The cell's value must be a valid URL. Supported by data validation. Requires no ConditionValues.
        DATE_EQ,                    //	The cell's value must be the same date as the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue for data validation, conditional formatting, and filters on non-data source objects and at least one ConditionValue for filters on data source objects.
        DATE_BEFORE,                //	The cell's value must be before the date of the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue that may be a relative date.
        DATE_AFTER,                 //	The cell's value must be after the date of the condition's value. Supported by data validation, conditional formatting and filters. Requires a single ConditionValue that may be a relative date.
        DATE_ON_OR_BEFORE,          //	The cell's value must be on or before the date of the condition's value. Supported by data validation. Requires a single ConditionValue that may be a relative date.
        DATE_ON_OR_AFTER,           //	The cell's value must be on or after the date of the condition's value. Supported by data validation. Requires a single ConditionValue that may be a relative date.
        DATE_BETWEEN,               //	The cell's value must be between the dates of the two condition values. Supported by data validation. Requires exactly two ConditionValues.
        DATE_NOT_BETWEEN,           //	The cell's value must be outside the dates of the two condition values. Supported by data validation. Requires exactly two ConditionValues.
        DATE_IS_VALID,              //	The cell's value must be a date. Supported by data validation. Requires no ConditionValues.
        ONE_OF_RANGE,               //	The cell's value must be listed in the grid in condition value's range. Supported by data validation. Requires a single ConditionValue, and the value must be a valid range in A1 notation.
        ONE_OF_LIST,                //	The cell's value must be in the list of condition values. Supported by data validation. Supports any number of condition values, one per item in the list. Formulas are not supported in the values.
        BLANK,                      //	The cell's value must be empty. Supported by conditional formatting and filters. Requires no ConditionValues.
        NOT_BLANK,                  //	The cell's value must not be empty. Supported by conditional formatting and filters. Requires no ConditionValues.
        CUSTOM_FORMULA,             //	The condition's formula must evaluate to true. Supported by data validation, conditional formatting and filters. Not supported by data source sheet filters. Requires a single ConditionValue.
        BOOLEAN,                    //	The cell's value must be TRUE/FALSE or in the list of condition values. Supported by data validation. Renders as a cell checkbox. Supports zero, one or two ConditionValues. No values indicates the cell must be TRUE or FALSE, where TRUE renders as checked and FALSE renders as unchecked. One value indicates the cell will render as checked when it contains that value and unchecked when it is blank. Two values indicate that the cell will render as checked when it contains the first value and unchecked when it contains the second value. For example, ["Yes","No"] indicates that the cell will render a checked box when it has the value "Yes" and an unchecked box when it has the value "No".
        TEXT_NOT_EQ,                //	The cell's value must be exactly not the condition's value. Supported by filters on data source objects. Requires at least one ConditionValue.
        DATE_NOT_EQ,                //	The cell's value must be exactly not the condition's value. Supported by filters on data source objects. Requires at least one ConditionValue.
        FILTER_EXPRESSION           //	The cell's value must follow the pattern specified. Requires a single ConditionValue.
    };

    // A sort order.
    enum SortOrder
    {
        SORT_ORDER_UNSPECIFIED, //	Default value, do not use this.
        ASCENDING,              //	Sort ascending.
        DESCENDING              //	Sort descending.
    };

    // The data source table column selection types.
    enum DataSourceTableColumnSelectionType
    {
        DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED, //	The default column selection type, do not use.
        SELECTED,                                            //	Select columns specified by columns field.
        SYNC_ALL                                             //	Sync all current and future columns in the data source.
        // If set, the data source table fetches all the columns in the data source at the time of refresh.
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
     * The kinds of value that a cell in a spreadsheet can have.
     */
    class ExtendedValue : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ExtendedValue &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ExtendedValue() {}
        // Represents a double value. Note: Dates, Times and DateTimes are represented as doubles in SERIAL_NUMBER format.
        ExtendedValue &numberValue(double value)
        {
            // Union field value
            if (buf[2].length() == 0 && buf[3].length() == 0 && buf[4].length() == 0 && buf[5].length() == 0)
                return setObject(buf[1], "numberValue", String(value), false, true);
            return *this;
        }
        // Represents a string value. Leading single quotes are not included. For example, if the user typed '123 into the UI, this would be represented as a stringValue of "123".
        ExtendedValue &stringValue(const String &value)
        {
            // Union field value
            if (buf[1].length() == 0 && buf[3].length() == 0 && buf[4].length() == 0 && buf[5].length() == 0)
                return setObject(buf[2], "stringValue", value, true, true);
            return *this;
        }
        // Represents a boolean value.
        ExtendedValue &boolValue(bool value)
        {
            // Union field value
            if (buf[1].length() == 0 && buf[2].length() == 0 && buf[4].length() == 0 && buf[5].length() == 0)
                return setObject(buf[3], "boolValue", owriter.getBoolStr(value), false, true);
            return *this;
        }
        // Represents a formula.
        ExtendedValue &formulaValue(const String &value)
        {
            // Union field value
            if (buf[1].length() == 0 && buf[2].length() == 0 && buf[3].length() == 0 && buf[5].length() == 0)
                return setObject(buf[4], "formulaValue", value, true, true);
            return *this;
        }

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

    /**
     * The value of the condition.
     */
    class ConditionValue : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ConditionValue &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ConditionValue() {}
        // A relative date (based on the current date). Valid only if the type is DATE_BEFORE, DATE_AFTER, DATE_ON_OR_BEFORE or DATE_ON_OR_AFTER.
        // Relative dates are not supported in data validation. They are supported only in conditional formatting and conditional filters.
        ConditionValue &relativeDate(RelativeDate value)
        {
            if (value == RELATIVE_DATE_UNSPECIFIED)
                return setObject(buf[1], "relativeDate", "RELATIVE_DATE_UNSPECIFIED", true, true);
            else if (value == PAST_YEAR)
                return setObject(buf[1], "relativeDate", "PAST_YEAR", true, true);
            else if (value == PAST_MONTH)
                return setObject(buf[1], "relativeDate", "PAST_MONTH", true, true);
            else if (value == PAST_WEEK)
                return setObject(buf[1], "relativeDate", "PAST_WEEK", true, true);
            else if (value == YESTERDAY)
                return setObject(buf[1], "relativeDate", "YESTERDAY", true, true);
            else if (value == TODAY)
                return setObject(buf[1], "relativeDate", "TODAY", true, true);
            else if (value == TOMORROW)
                return setObject(buf[1], "relativeDate", "TOMORROW", true, true);
            return *this;
        }
        // A value the condition is based on. The value is parsed as if the user typed into a cell. Formulas are supported (and must begin with an = or a '+').
        ConditionValue &userEnteredValue(const String &value) { return setObject(buf[2], "userEnteredValue", value, true, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A condition that can evaluate to true or false. BooleanConditions are used by conditional formatting, data validation, and the criteria in filters.
     */
    class BooleanCondition : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        BooleanCondition &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        BooleanCondition() {}
        // The type of condition.
        BooleanCondition &type(ConditionType value)
        {
            if (value == CONDITION_TYPE_UNSPECIFIED)
                return setObject(buf[1], "type", "CONDITION_TYPE_UNSPECIFIED", true, true);
            else if (value == NUMBER_GREATER)
                return setObject(buf[1], "type", "NUMBER_GREATER", true, true);
            else if (value == NUMBER_GREATER_THAN_EQ)
                return setObject(buf[1], "type", "NUMBER_GREATER_THAN_EQ", true, true);
            else if (value == NUMBER_LESS)
                return setObject(buf[1], "type", "NUMBER_LESS", true, true);
            else if (value == NUMBER_LESS_THAN_EQ)
                return setObject(buf[1], "type", "NUMBER_LESS_THAN_EQ", true, true);
            else if (value == NUMBER_EQ)
                return setObject(buf[1], "type", "NUMBER_EQ", true, true);
            else if (value == NUMBER_NOT_EQ)
                return setObject(buf[1], "type", "NUMBER_NOT_EQ", true, true);
            else if (value == NUMBER_BETWEEN)
                return setObject(buf[1], "type", "NUMBER_BETWEEN", true, true);
            else if (value == NUMBER_NOT_BETWEEN)
                return setObject(buf[1], "type", "NUMBER_NOT_BETWEEN", true, true);
            else if (value == TEXT_CONTAINS)
                return setObject(buf[1], "type", "TEXT_CONTAINS", true, true);
            else if (value == TEXT_NOT_CONTAINS)
                return setObject(buf[1], "type", "TEXT_NOT_CONTAINS", true, true);
            else if (value == TEXT_STARTS_WITH)
                return setObject(buf[1], "type", "TEXT_STARTS_WITH", true, true);
            else if (value == TEXT_ENDS_WITH)
                return setObject(buf[1], "type", "TEXT_ENDS_WITH", true, true);
            else if (value == TEXT_EQ)
                return setObject(buf[1], "type", "TEXT_EQ", true, true);
            else if (value == TEXT_IS_EMAIL)
                return setObject(buf[1], "type", "TEXT_IS_EMAIL", true, true);
            else if (value == TEXT_IS_URL)
                return setObject(buf[1], "type", "TEXT_IS_URL", true, true);
            else if (value == DATE_EQ)
                return setObject(buf[1], "type", "DATE_EQ", true, true);
            else if (value == DATE_BEFORE)
                return setObject(buf[1], "type", "DATE_BEFORE", true, true);
            else if (value == DATE_AFTER)
                return setObject(buf[1], "type", "DATE_AFTER", true, true);
            else if (value == DATE_ON_OR_BEFORE)
                return setObject(buf[1], "type", "DATE_ON_OR_BEFORE", true, true);
            else if (value == DATE_ON_OR_AFTER)
                return setObject(buf[1], "type", "DATE_ON_OR_AFTER", true, true);
            else if (value == DATE_BETWEEN)
                return setObject(buf[1], "type", "DATE_BETWEEN", true, true);
            else if (value == DATE_NOT_BETWEEN)
                return setObject(buf[1], "type", "DATE_NOT_BETWEEN", true, true);
            else if (value == DATE_IS_VALID)
                return setObject(buf[1], "type", "DATE_IS_VALID", true, true);
            else if (value == ONE_OF_RANGE)
                return setObject(buf[1], "type", "ONE_OF_RANGE", true, true);
            else if (value == ONE_OF_LIST)
                return setObject(buf[1], "type", "ONE_OF_LIST", true, true);
            else if (value == BLANK)
                return setObject(buf[1], "type", "BLANK", true, true);
            else if (value == NOT_BLANK)
                return setObject(buf[1], "type", "NOT_BLANK", true, true);
            else if (value == CUSTOM_FORMULA)
                return setObject(buf[1], "type", "CUSTOM_FORMULA", true, true);
            else if (value == BOOLEAN)
                return setObject(buf[1], "type", "BOOLEAN", true, true);
            else if (value == TEXT_NOT_EQ)
                return setObject(buf[1], "type", "TEXT_NOT_EQ", true, true);
            else if (value == DATE_NOT_EQ)
                return setObject(buf[1], "type", "DATE_NOT_EQ", true, true);
            else if (value == FILTER_EXPRESSION)
                return setObject(buf[1], "type", "FILTER_EXPRESSION", true, true);
            return *this;
        }
        // The values of the condition. The number of supported values depends on the condition type. Some support zero values, others one or two values, and ConditionType.ONE_OF_LIST supports an arbitrary number of values.
        BooleanCondition &addValues(const ConditionValue &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("values"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * An unique identifier that references a data source column.
     */
    class DataSourceColumnReference : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceColumnReference() {}
        // The display name of the column. It should be unique within a data source.
        DataSourceColumnReference &name(const String &value)
        {
            jut.addObject(buf, "name", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Criteria for showing/hiding rows in a filter or filter view.
     */
    class FilterCriteria : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        FilterCriteria &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        FilterCriteria() {}
        // Values that should be hidden.
        FilterCriteria &hiddenValues(const String &value) { return setObject(buf[1], "hiddenValues", value, true, true); }
        // A condition that must be true for values to be shown. (This does not override hiddenValues -- if a value is listed there, it will still be hidden.)
        FilterCriteria &condition(const BooleanCondition &value) { return setObject(buf[2], "condition", value.c_str(), false, true); }
        // The background fill color to filter by; only cells with this fill color are shown. This field is mutually exclusive with visibleForegroundColor, and must be set to an RGB-type color. If visibleBackgroundColor is also set, this field takes precedence.
        FilterCriteria &visibleBackgroundColorStyle(const ColorStyle &value) { return setObject(buf[3], "visibleBackgroundColorStyle", value.c_str(), false, true); }
        // The foreground color to filter by; only cells with this foreground color are shown. This field is mutually exclusive with visibleBackgroundColor, and must be set to an RGB-type color. If visibleForegroundColor is also set, this field takes precedence.
        FilterCriteria &visibleForegroundColorStyle(const Color &value) { return setObject(buf[4], "visibleForegroundColorStyle", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * The filter criteria associated with a specific column.
     */
    class FilterSpec : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        FilterSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        FilterSpec() {}
        // The criteria for the column.
        FilterSpec &filterCriteria(const FilterCriteria &value) { return setObject(buf[1], "filterCriteria", value.c_str(), false, true); }
        // The zero-based column index.
        // Union field reference
        FilterSpec &columnIndex(int value) { return buf[3].length() == 0 ? setObject(buf[2], "columnIndex", String(value), false, true) : *this; }
        // Reference to a data source column.
        // Union field reference
        FilterSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return buf[2].length() == 0 ? setObject(buf[3], "dataSourceColumnReference", value.c_str(), false, true) : *this; }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A sort order associated with a specific column or row.
     */
    class SortSpec : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        SortSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        SortSpec() {}
        // The order data should be sorted.
        SortSpec &sortOrder(SortOrder value)
        {
            if (value == SORT_ORDER_UNSPECIFIED)
                return setObject(buf[1], "sortOrder", "SORT_ORDER_UNSPECIFIED", true, true);
            else if (value == ASCENDING)
                return setObject(buf[1], "sortOrder", "ASCENDING", true, true);
            else if (value == DESCENDING)
                return setObject(buf[1], "sortOrder", "DESCENDING", true, true);
            return *this;
        }
        // The foreground color to sort by; cells with this foreground color are sorted to the top. Mutually exclusive with backgroundColor, and must be an RGB-type color. If foregroundColor is also set, this field takes precedence.
        SortSpec &foregroundColorStyle(const ColorStyle &value) { return setObject(buf[2], "foregroundColorStyle", value.c_str(), false, true); }
        // The background fill color to sort by; cells with this fill color are sorted to the top. Mutually exclusive with foregroundColor, and must be an RGB-type color. If backgroundColor is also set, this field takes precedence.
        SortSpec &backgroundColorStyle(const ColorStyle &value) { return setObject(buf[3], "backgroundColorStyle", value.c_str(), false, true); }
        // The dimension the sort should be applied to.
        SortSpec &dimensionIndex(int value) { return buf[5].length() == 0 ? setObject(buf[4], "dimensionIndex", String(value), false, true) : *this; }
        // Reference to a data source column.
        SortSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return buf[4].length() == 0 ? setObject(buf[5], "dataSourceColumnReference", value.c_str(), false, true) : *this; }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * A data source table, which allows the user to import a static table of data from the DataSource into Sheets. This is also known as "Extract" in the Sheets editor.
     */
    class DataSourceTable : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataSourceTable &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataSourceTable() {}
        // The ID of the data source the data source table is associated with.
        DataSourceTable &dataSourceId(const String &value) { return setObject(buf[1], "dataSourceId", value, true, true); }
        // The type to select columns for the data source table. Defaults to SELECTED.
        DataSourceTable &columnSelectionType(DataSourceTableColumnSelectionType value)
        {
            if (value == DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED)
                return setObject(buf[2], "columnSelectionType", "DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED", true, true);
            else if (value == SELECTED)
                return setObject(buf[2], "columnSelectionType", "SELECTED", true, true);
            else if (value == SYNC_ALL)
                return setObject(buf[2], "columnSelectionType", "SYNC_ALL", true, true);
            return *this;
        }
        // Columns selected for the data source table. The columnSelectionType must be SELECTED.
        DataSourceTable &addColumns(const DataSourceColumnReference &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("columns"), value.c_str(), false);
            return *this;
        }
        // Filter specifications in the data source table.
        DataSourceTable &addFilterSpecs(const FilterSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("filterSpecs"), value.c_str(), false);
            return *this;
        }
        // Sort specifications in the data source table. The result of the data source table is sorted based on the sort specifications in order.
        DataSourceTable &addSortSpecs(const SortSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("sortSpecs"), value.c_str(), false);
            return *this;
        }
        // The limit of rows to return. If not set, a default limit is applied. Please refer to the Sheets editor for the default and max limit.
        DataSourceTable &rowLimit(int value) { return setObject(buf[6], "rowLimit", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * A data source formula.
     */
    class DataSourceFormula : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceFormula() {}
        // The ID of the data source the formula is associated with.
        DataSourceFormula &dataSourceId(const String &value) { jut.addObject(buf, "dataSourceId", value, true, true); }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

}
#endif