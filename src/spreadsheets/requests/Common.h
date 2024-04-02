#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace Direction
{
    // The direction of text in a cell.
    enum TextDirection
    {
        TEXT_DIRECTION_UNSPECIFIED, //	The text direction is not specified. Do not use this.
        LEFT_TO_RIGHT,              //	The text direction of left-to-right was set by the user.
        RIGHT_TO_LEFT               //	The text direction of right-to-left was set by the user.
    };

    // The layout of pivot values.
    enum PivotValueLayout
    {
        HORIZONTAL, //	Values are laid out horizontally (as columns).
        VERTICAL    //	Values are laid out vertically (as rows).
    };
}

namespace Format
{
    // The number format of the cell. In this documentation the locale is assumed to be en_US, but the actual format depends on the locale of the spreadsheet.
    enum NumberFormatType
    {
        NUMBER_FORMAT_TYPE_UNSPECIFIED, //	The number format is not specified and is based on the contents of the cell. Do not explicitly use this.
        TEXT,                           //	Text formatting, e.g 1000.12
        NUMBER,                         //	Number formatting, e.g, 1,000.12
        PERCENT,                        //	Percent formatting, e.g 10.12%
        CURRENCY,                       //	Currency formatting, e.g $1,000.12
        DATE,                           //	Date formatting, e.g 9/26/2008
        TIME,                           //	Time formatting, e.g 3:59:00 PM
        DATE_TIME,                      //	Date+Time formatting, e.g 9/26/08 15:59:00
        SCIENTIFIC                      //                     //	Scientific number formatting, e.g 1.01E+03
    };
}

namespace Line
{
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
}

namespace Display
{
    // Whether to explicitly render a hyperlink. If not specified, the hyperlink is linked.
    enum HyperlinkDisplayType
    {
        HYPERLINK_DISPLAY_TYPE_UNSPECIFIED, //	The default value: the hyperlink is rendered. Do not use this.
        LINKED,                             //	A hyperlink should be explicitly rendered.
        PLAIN_TEXT                          //	A hyperlink should not be rendered.
    };

    // The possible ways that pivot values may be calculated for display.
    enum PivotValueCalculatedDisplayType
    {
        PIVOT_VALUE_CALCULATED_DISPLAY_TYPE_UNSPECIFIED, //	Default value, do not use.
        PERCENT_OF_ROW_TOTAL,                            //	Shows the pivot values as percentage of the row total values.
        PERCENT_OF_COLUMN_TOTAL,                         //	Shows the pivot values as percentage of the column total values.
        PERCENT_OF_GRAND_TOTAL                           //	Shows the pivot values as percentage of the grand total values.
    };

}

namespace Date
{
    // Represents a day of the week.
    enum DayOfWeek
    {
        DAY_OF_WEEK_UNSPECIFIED, //	The day of the week is unspecified.
        MONDAY,                  //	Monday
        TUESDAY,                 //	Tuesday
        WEDNESDAY,               //	Wednesday
        THURSDAY,                //	Thursday
        FRIDAY,                  //	Friday
        SATURDAY,                //	Saturday
        SUNDAY                   //	Sunday
    };

}

namespace Cell
{
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
}

namespace Selection
{
    // The data source table column selection types.
    enum DataSourceTableColumnSelectionType
    {
        DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED, //	The default column selection type, do not use.
        SELECTED,                                            //	Select columns specified by columns field.
        SYNC_ALL                                             //	Sync all current and future columns in the data source.
        // If set, the data source table fetches all the columns in the data source at the time of refresh.
    };

}

namespace Metadata
{
    // An enumeration of possible metadata visibilities.
    enum DeveloperMetadataVisibility
    {
        DEVELOPER_METADATA_VISIBILITY_UNSPECIFIED, //	Default value.
        DOCUMENT,                                  //	Document-visible metadata is accessible from any developer project with access to the document.
        PROJECT,                                   //	Project-visible metadata is only visible to and accessible by the developer project that created the metadata.
    };
}

namespace Dimensions
{
    // Indicates which dimension an operation should apply to.
    enum Dimension
    {
        DIMENSION_UNSPECIFIED, //	The default value, do not use.
        ROWS,                  //	Operates on the rows of a sheet.
        COLUMNS                //	Operates on the columns of a sheet.
    };
}

namespace Group
{
    // The available types of date-time grouping rules. This documentation assumes the spreadsheet locale is "en-US", though the actual rendering of the dates and times uses the locale of the spreadsheet for some rule types.
    enum DateTimeRuleType
    {
        DATE_TIME_RULE_TYPE_UNSPECIFIED, //	The default type, do not use.
        SECOND,                          //	Group dates by second, from 0 to 59.
        MINUTE,                          //	Group dates by minute, from 0 to 59.
        HOUR,                            //	Group dates by hour using a 24-hour system, from 0 to 23.
        HOUR_MINUTE,                     //	Group dates by hour and minute using a 24-hour system, for example 19:45.
        HOUR_MINUTE_AMPM,                //	Group dates by hour and minute using a 12-hour system, for example 7:45 PM. The AM/PM designation is translated based on the spreadsheet locale.
        DAY_OF_WEEK,                     //	Group dates by day of week, for example Sunday. The days of the week will be translated based on the spreadsheet locale.
        DAY_OF_YEAR,                     //	Group dates by day of year, from 1 to 366. Note that dates after Feb. 29 fall in different buckets in leap years than in non-leap years.
        DAY_OF_MONTH,                    //	Group dates by day of month, from 1 to 31.
        DAY_MONTH,                       //	Group dates by day and month, for example 22-Nov. The month is translated based on the spreadsheet locale.
        MONTH,                           //	Group dates by month, for example Nov. The month is translated based on the spreadsheet locale.
        QUARTER,                         //	Group dates by quarter, for example Q1 (which represents Jan-Mar).
        YEAR,                            //	Group dates by year, for example 2008.
        YEAR_MONTH,                      //	Group dates by year and month, for example 2008-Nov. The month is translated based on the spreadsheet locale.
        YEAR_QUARTER,                    //	Group dates by year and quarter, for example 2008 Q4.
        YEAR_MONTH_DAY                   //	Group dates by year, month, and day, for example 2008-11-22.
    };
}

namespace Function
{
    // A function to summarize a pivot value.
    enum PivotValueSummarizeFunction
    {
        PIVOT_STANDARD_VALUE_FUNCTION_UNSPECIFIED, //	The default, do not use.
        SUM_FUNC,                                  //	Corresponds to the SUM function.
        COUNTA_FUNC,                               //	Corresponds to the COUNTA function.
        COUNT_FUNC,                                //	Corresponds to the COUNT function.
        COUNTUNIQUE_FUNC,                          //	Corresponds to the COUNTUNIQUE function.
        AVERAGE_FUNC,                              //	Corresponds to the AVERAGE function.
        MAX_FUNC,                                  //	Corresponds to the MAX function.
        MIN_FUNC,                                  //	Corresponds to the MIN function.
        MEDIAN_FUNC,                               //	Corresponds to the MEDIAN function.
        PRODUCT_FUNC,                              //	Corresponds to the PRODUCT function.
        STDEV_FUNC,                                //	Corresponds to the STDEV function.
        STDEVP_FUNC,                               //	Corresponds to the STDEVP function.
        VAR_FUNC,                                  //	Corresponds to the VAR function.
        VARP_FUNC,                                 //	Corresponds to the VARP function.
        CUSTOM_FUNC                                //	Indicates the formula should be used as-is. Only valid if PivotValue.formula was set.
    };
}

namespace Sort
{
    // A sort order.
    enum SortOrder
    {
        SORT_ORDER_UNSPECIFIED, //	Default value, do not use this.
        ASCENDING,              //	Sort ascending.
        DESCENDING              //	Sort descending.
    };
}

namespace Interpolation
{
    // The kind of interpolation point.
    enum InterpolationPointType
    {
        INTERPOLATION_POINT_TYPE_UNSPECIFIED, //	The default value, do not use.
        MIN,                                  //	The interpolation point uses the minimum value in the cells over the range of the conditional format.
        MAX,                                  //	The interpolation point uses the maximum value in the cells over the range of the conditional format.
        NUMBER,                               //	The interpolation point uses exactly the value in InterpolationPoint.value.
        PERCENT,                              //	The interpolation point is the given percentage over all the cells in the range of the conditional format. This is equivalent to NUMBER if the value was: =(MAX(FLATTEN(range)) * (value / 100)) + (MIN(FLATTEN(range)) * (1 - (value / 100))) (where errors in the range are ignored when flattening).
        PERCENTILE                            //	The interpolation point is the given percentile over all the cells in the range of the conditional format. This is equivalent to NUMBER if the value was: =PERCENTILE(FLATTEN(range), value / 100) (where errors in the range are ignored when flattening).
    };
}

namespace GSHEET
{
    // The kind of sheet.
    enum SheetType
    {
        SHEET_TYPE_UNSPECIFIED, //	Default value, do not use.
        GRID,                   //	The sheet is a grid.
        OBJECT,                 //	The sheet has no grid and instead has an object like a chart or image.
        DATA_SOURCE             //	The sheet connects with an external DataSource and shows the preview of data.
    };
}

namespace UpdateInterval
{
    // An enumeration of the possible recalculation interval options.
    enum RecalculationInterval
    {
        RECALCULATION_INTERVAL_UNSPECIFIED, //	Default value. This value must not be used.
        ON_CHANGE,                          //	Volatile functions are updated on every change.
        MINUTE,                             //	Volatile functions are updated on every change and every minute.
        HOUR                                //	Volatile functions are updated on every change and hourly.
    };
}

namespace Paste
{
    // What kind of data should be pasted.
    enum PasteType
    {
        PASTE_NORMAL,                //	Paste values, formulas, formats, and merges.
        PASTE_VALUES,                //	Paste the values ONLY without formats, formulas, or merges.
        PASTE_FORMAT,                //	Paste the format and data validation only.
        PASTE_NO_BORDERS,            //	Like PASTE_NORMAL but without borders.
        PASTE_FORMULA,               //	Paste the formulas only.
        PASTE_DATA_VALIDATION,       //	Paste the data validation only.
        PASTE_CONDITIONAL_FORMATTING //	Paste the conditional formatting rules only.
    };

    // How a paste operation should be performed.
    enum PasteOrientation
    {
        NORMAL,   //	Paste normally.
        TRANSPOSE //	Paste transposed, where all rows become columns and vice versa.
    };

}

namespace Merge
{
    // The type of merge to create.
    enum MergeType
    {
        MERGE_ALL,     //	Create a single merge from the range
        MERGE_COLUMNS, //	Create a merge for each column in the range
        MERGE_ROWS     //	Create a merge for each row in the range
    };
}

namespace Delim
{
    // The delimiter to split on.
    enum DelimiterType
    {
        DELIMITER_TYPE_UNSPECIFIED, //	Default value. This value must not be used.
        COMMA,                //	","
        SEMICOLON,            //	";"
        PERIOD,               //	"."
        SPACE,                //	" "
        CUSTOM,               //	A custom value as defined in delimiter.
        AUTODETECT            //	Automatically detect columns.
    };
}

namespace GSHEET
{
    struct key_str_10
    {
        char text[10];
    };

    struct key_str_20
    {
        char text[20];
    };

    struct key_str_30
    {
        char text[30];
    };

    struct key_str_40
    {
        char text[40];
    };
    struct key_str_60
    {
        char text[60];
    };

    const struct key_str_30 _DayOfWeek[Date::DayOfWeek::SUNDAY + 1] PROGMEM = {"DAY_OF_WEEK_UNSPECIFIED", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"};

    const struct key_str_30 _TextDirection[Direction::TextDirection::RIGHT_TO_LEFT + 1] PROGMEM = {"TEXT_DIRECTION_UNSPECIFIED", "LEFT_TO_RIGHT", "RIGHT_TO_LEFT"};

    const struct key_str_40 _NumberFormatType[Format::NumberFormatType::SCIENTIFIC + 1] PROGMEM = {"NUMBER_FORMAT_TYPE_UNSPECIFIED", "TEXT", "NUMBER", "PERCENT", "CURRENCY", "DATE", "TIME", "DATE_TIME", "SCIENTIFIC"};

    const struct key_str_20 _Style[Line::Style::DOUBLE + 1] PROGMEM = {"STYLE_UNSPECIFIED", "DOTTED", "DASHED", "SOLID", "SOLID_MEDIUM", "SOLID_THICK", "NONE", "DOUBLE"};

    const struct key_str_30 _WrapStrategy[Cell::WrapStrategy::WRAP + 1] PROGMEM = {"WRAP_STRATEGY_UNSPECIFIED", "OVERFLOW_CELL", "LEGACY_WRAP", "CLIP", "WRAP"};

    const struct key_str_40 _HyperlinkDisplayType[Display::HyperlinkDisplayType::PLAIN_TEXT + 1] PROGMEM = {"HYPERLINK_DISPLAY_TYPE_UNSPECIFIED", "LINKED", "PLAIN_TEXT"};

    const struct key_str_60 _DataSourceTableColumnSelectionType[Selection::DataSourceTableColumnSelectionType::SYNC_ALL + 1] PROGMEM = {"DATA_SOURCE_TABLE_COLUMN_SELECTION_TYPE_UNSPECIFIED", "SELECTED", "SYNC_ALL"};

    const struct key_str_60 _DeveloperMetadataVisibility[Metadata::DeveloperMetadataVisibility::PROJECT + 1] PROGMEM = {"DEVELOPER_METADATA_VISIBILITY_UNSPECIFIED", "DOCUMENT", "PROJECT"};

    const struct key_str_30 _Dimension[Dimensions::Dimension::COLUMNS + 1] PROGMEM = {"DIMENSION_UNSPECIFIED", "ROWS", "COLUMNS"};

    const struct key_str_40 _DateTimeRuleType[Group::DateTimeRuleType::YEAR_MONTH_DAY + 1] PROGMEM = {"DATE_TIME_RULE_TYPE_UNSPECIFIED", "SECOND", "MINUTE", "HOUR", "HOUR_MINUTE", "HOUR_MINUTE_AMPM", "DAY_OF_WEEK", "DAY_OF_YEAR", "DAY_OF_MONTH", "DAY_MONTH", "MONTH", "QUARTER", "YEAR", "YEAR_MONTH", "YEAR_QUARTER", "YEAR_MONTH_DAY"};

    const struct key_str_60 _PivotValueSummarizeFunction[Function::PivotValueSummarizeFunction::CUSTOM_FUNC + 1] PROGMEM = {"PIVOT_STANDARD_VALUE_FUNCTION_UNSPECIFIED", "SUM_FUNC",
                                                                                                                            "COUNTA_FUNC", "COUNT_FUNC", "COUNTUNIQUE_FUNC", "AVERAGE_FUNC", "MAX_FUNC", "MIN_FUNC", "MEDIAN_FUNC", "PRODUCT_FUNC", "STDEV_FUNC", "STDEVP_FUNC", "VAR_FUNC", "VARP_FUNC", "CUSTOM_FUNC"};
    const struct key_str_60 _PivotValueCalculatedDisplayType[Display::PivotValueCalculatedDisplayType::PERCENT_OF_GRAND_TOTAL + 1] PROGMEM = {"PIVOT_VALUE_CALCULATED_DISPLAY_TYPE_UNSPECIFIED", "PERCENT_OF_ROW_TOTAL", "PERCENT_OF_COLUMN_TOTAL", "PERCENT_OF_GRAND_TOTAL"};

    const struct key_str_30 _SortOrder[Sort::SortOrder::DESCENDING + 1] PROGMEM = {"SORT_ORDER_UNSPECIFIED", "ASCENDING", "DESCENDING"};

    const struct key_str_20 _PivotValueLayout[Direction::PivotValueLayout::VERTICAL + 1] PROGMEM = {"HORIZONTAL", "VERTICAL"};

    const struct key_str_30 _SheetType[SheetType::DATA_SOURCE + 1] PROGMEM = {"SHEET_TYPE_UNSPECIFIED", "GRID", "OBJECT", "DATA_SOURCE"};

    const struct key_str_60 _InterpolationPointType[Interpolation::InterpolationPointType::PERCENTILE + 1] PROGMEM = {"INTERPOLATION_POINT_TYPE_UNSPECIFIED", "MIN", "MAX", "NUMBER", "PERCENT", "PERCENTILE"};

    const struct key_str_40 _RecalculationInterval[UpdateInterval::RecalculationInterval::HOUR + 1] PROGMEM = {"RECALCULATION_INTERVAL_UNSPECIFIED", "MINUTE", "HOUR"};

    const struct key_str_30 _PasteType[Paste::PasteType::PASTE_CONDITIONAL_FORMATTING + 1] PROGMEM = {"PASTE_NORMAL", "PASTE_VALUES", "PASTE_FORMAT", "PASTE_NO_BORDERS", "PASTE_FORMULA", "PASTE_DATA_VALIDATION", "PASTE_CONDITIONAL_FORMATTING"};

    const struct key_str_20 _PasteOrientation[Paste::PasteOrientation::TRANSPOSE + 1] PROGMEM = {"NORMAL", "TRANSPOSE"};

    const struct key_str_30 _MergeType[Merge::MergeType::MERGE_ROWS + 1] PROGMEM = {"MERGE_ALL", "MERGE_COLUMNS", "MERGE_ROWS"};

    const struct key_str_30 _DelimiterType[Delim::DelimiterType::AUTODETECT + 1] PROGMEM = {"DELIMITER_TYPE_UNSPECIFIED", "COMMA", "SEMICOLON", "PERIOD", "SPACE", "CUSTOM", "AUTODETECT"};

    class BufWriter
    {
    private:
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        template <typename T>
        struct v_number
        {
            static bool const value = std::is_same<T, uint64_t>::value || std::is_same<T, int64_t>::value || std::is_same<T, uint32_t>::value || std::is_same<T, int32_t>::value ||
                                      std::is_same<T, uint16_t>::value || std::is_same<T, int16_t>::value || std::is_same<T, uint8_t>::value || std::is_same<T, int8_t>::value ||
                                      std::is_same<T, double>::value || std::is_same<T, float>::value || std::is_same<T, int>::value;
        };

        template <typename T>
        struct v_sring
        {
            static bool const value = std::is_same<T, const char *>::value || std::is_same<T, std::string>::value || std::is_same<T, String>::value;
        };

        void setObject(String *buf, size_t bufSize, String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
        }

    public:
        BufWriter() {}
        template <typename T1, typename T2>
        T1 add(T1 ret, bool value, String &buf, const String &name)
        {
            clear(buf);
            jut.addObject(buf, name, owriter.getBoolStr(value), false, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto add(T1 ret, const T2 &value, String &buf, const String &name) -> typename std::enable_if<v_number<T2>::value, T1>::type
        {
            clear(buf);
            jut.addObject(buf, name, String(value), false, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto add(T1 ret, const T2 &value, String &buf, const String &name) -> typename std::enable_if<v_sring<T2>::value, T1>::type
        {
            clear(buf);
            jut.addObject(buf, name, value, true, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto add(T1 ret, const T2 &value, String &buf, const String &name) -> typename std::enable_if<(!v_sring<T2>::value && !v_number<T2>::value && !std::is_same<T2, bool>::value), T1>::type
        {
            clear(buf);
            jut.addObject(buf, name, value.c_str(), false, true);
            return ret;
        }

        template <typename T1, typename T2>
        T1 set(T1 ret, bool value, String *buf, size_t bufSize, String &buf_n, const String &name)
        {
            setObject(buf, bufSize, buf_n, name, owriter.getBoolStr(value), false, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto set(T1 ret, const T2 &value, String *buf, size_t bufSize, String &buf_n, const String &name) -> typename std::enable_if<v_number<T2>::value, T1>::type
        {
            setObject(buf, bufSize, buf_n, name, String(value), false, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto set(T1 ret, const T2 &value, String *buf, size_t bufSize, String &buf_n, const String &name) -> typename std::enable_if<v_sring<T2>::value, T1>::type
        {
            setObject(buf, bufSize, buf_n, name, value, true, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto set(T1 ret, const T2 &value, String *buf, size_t bufSize, String &buf_n, const String &name) -> typename std::enable_if<(!v_sring<T2>::value && !v_number<T2>::value && !std::is_same<T2, bool>::value), T1>::type
        {
            setObject(buf, bufSize, buf_n, name, value.c_str(), false, true);
            return ret;
        }

        template <typename T1, typename T2>
        T1 append(T1 ret, bool value, String *buf, size_t bufSize, String &buf_n, const String &name)
        {
            owriter.addMapArrayMember(buf, bufSize, buf_n, name, owriter.getBoolStr(value), false);
            return ret;
        }

        template <typename T1, typename T2>
        auto append(T1 ret, const T2 &value, String *buf, size_t bufSize, String &buf_n, const String &name) -> typename std::enable_if<v_number<T2>::value, T1>::type
        {
            owriter.addMapArrayMember(buf, bufSize, buf_n, name, String(value), false);
            return ret;
        }

        template <typename T1, typename T2>
        auto append(T1 ret, const T2 &value, String *buf, size_t bufSize, String &buf_n, const String &name) -> typename std::enable_if<v_sring<T2>::value, T1>::type
        {
            owriter.addMapArrayMember(buf, bufSize, buf_n, name, value, true);
            return ret;
        }

        template <typename T1, typename T2>
        auto append(T1 ret, const T2 &value, String *buf, size_t bufSize, String &buf_n, const String &name) -> typename std::enable_if<(!v_sring<T2>::value && !v_number<T2>::value && !std::is_same<T2, bool>::value), T1>::type
        {
            owriter.addMapArrayMember(buf, bufSize, buf_n, name, value.c_str(), false);
            return ret;
        }
        void clear(String &buf) { buf.remove(0, buf.length()); }
        void clear(String *buf, size_t bufSize) { owriter.clearBuf(buf, bufSize); }
    };

    class O1 : public Printable
    {

    protected:
        String buf;
        BufWriter wr;

    public:
        O1() {}
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    class O2 : public Printable
    {

    protected:
        static const size_t bufSize = 2;
        String buf[bufSize];
        BufWriter wr;

    public:
        O2() {}
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { wr.clear(buf, bufSize); }
    };

    class O4 : public Printable
    {

    protected:
        static const size_t bufSize = 4;
        String buf[bufSize];
        BufWriter wr;

    public:
        O4() {}
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { wr.clear(buf, bufSize); }
    };

    class O6 : public Printable
    {

    protected:
        static const size_t bufSize = 6;
        String buf[bufSize];
        BufWriter wr;

    public:
        O6() {}
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { wr.clear(buf, bufSize); }
    };

    class O8 : public Printable
    {
    protected:
        static const size_t bufSize = 8;
        String buf[bufSize];
        BufWriter wr;

    public:
        O8() {}
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { wr.clear(buf, bufSize); }
    };

    class O12 : public Printable
    {

    protected:
        static const size_t bufSize = 12;
        String buf[bufSize];
        BufWriter wr;

    public:
        O12() {}
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { wr.clear(buf, bufSize); }
    };

    class O16 : public Printable
    {
    protected:
        static const size_t bufSize = 16;
        String buf[bufSize];
        BufWriter wr;

    public:
        O16() {}
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { wr.clear(buf, bufSize); }
    };

}

#endif