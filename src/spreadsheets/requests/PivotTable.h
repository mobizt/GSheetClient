#ifndef PIVOT_TABLE_H
#define PIVOT_TABLE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Common.h"

/**
 * PIVOT TABLE CLASS DEPENDENCIES
 * 
 * PivotTable   +   PivotGroup                  +    PivotGroupValueMetadata                +   ExtendedValue
 *              |                               |
 *              |                               +   (enum) SortOrder
 *              |                               |
 *              |                               +   PivotGroupSortValueBucket               +   ExtendedValue
 *              |                               |
 *              |                               |
 *              |                               +   PivotGroupRule                          +   ManualRule          +   ManualRuleGroup         +   ExtendedValue
 *              |                               |                                           |
 *              |                               |                                           +   HistogramRule
 *              |                               |                                           |
 *              |                               |                                           +   DateTimeRule        +   (enum) DateTimeRuleType
 *              |                               |
 *              |                               +    PivotGroupLimit
 *              |                               |
 *              |                               +    DataSourceColumnReference
 *              |
 *              |
 *              +   PivotFilterSpec             +   PivotFilterCriteria                     +   BooleanCondition    +   (enum) ConditionType
 *              |                               |                                                                   |
 *              |                               |                                                                   +   ConditionValue          +   (enum) RelativeDate
 *              |                               |
 *              |                               +   DataSourceColumnReference
 *              |
 *              |
 *              +   PivotValue                  +   (enum) PivotValueSummarizeFunction
 *              |                               |
 *              |                               +   (enum) PivotValueCalculatedDisplayType
 *              |                               |
 *              |                               +   DataSourceColumnReference
 *              |
 *              +   (enum) PivotValueLayout
 *              |
 *              +   GridRange
 */

namespace GSHEET
{

    // The available types of date-time grouping rules. This documentation assumes the spreadsheet locale is "en-US", though the actual rendering of the dates and times uses the locale of the spreadsheet for some rule types.
    enum DateTimeRuleType
    {
        DATE_TIME_RULE_TYPE_UNSPECIFIED, //	The default type, do not use.
        SECOND,                          //	Group dates by second, from 0 to 59.
        _MINUTE,                         //	Group dates by minute, from 0 to 59.
        _HOUR,                           //	Group dates by hour using a 24-hour system, from 0 to 23.
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

    // A function to summarize a pivot value.
    enum PivotValueSummarizeFunction
    {
        PIVOT_STANDARD_VALUE_FUNCTION_UNSPECIFIED, //	The default, do not use.
        SUM,                                       //	Corresponds to the SUM function.
        COUNTA,                                    //	Corresponds to the COUNTA function.
        COUNT,                                     //	Corresponds to the COUNT function.
        COUNTUNIQUE,                               //	Corresponds to the COUNTUNIQUE function.
        AVERAGE,                                   //	Corresponds to the AVERAGE function.
        MAX,                                       //	Corresponds to the MAX function.
        MIN,                                       //	Corresponds to the MIN function.
        MEDIAN,                                    //	Corresponds to the MEDIAN function.
        PRODUCT,                                   //	Corresponds to the PRODUCT function.
        STDEV,                                     //	Corresponds to the STDEV function.
        STDEVP,                                    //	Corresponds to the STDEVP function.
        VAR,                                       //	Corresponds to the VAR function.
        VARP,                                      //	Corresponds to the VARP function.
        CUSTOM                                     //	Indicates the formula should be used as-is. Only valid if PivotValue.formula was set.
    };

    // The possible ways that pivot values may be calculated for display.
    enum PivotValueCalculatedDisplayType
    {
        PIVOT_VALUE_CALCULATED_DISPLAY_TYPE_UNSPECIFIED, //	Default value, do not use.
        PERCENT_OF_ROW_TOTAL,                            //	Shows the pivot values as percentage of the row total values.
        PERCENT_OF_COLUMN_TOTAL,                         //	Shows the pivot values as percentage of the column total values.
        PERCENT_OF_GRAND_TOTAL                           //	Shows the pivot values as percentage of the grand total values.
    };

    // The layout of pivot values.
    enum PivotValueLayout
    {
        HORIZONTAL, //	Values are laid out horizontally (as columns).
        VERTICAL    //	Values are laid out vertically (as rows).
    };

    /**
     * Metadata about a value in a pivot grouping.
     */
    class PivotGroupValueMetadata : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotGroupValueMetadata &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotGroupValueMetadata() {}
        // The calculated value the metadata corresponds to. (Note that formulaValue is not valid, because the values will be calculated.)
        PivotGroupValueMetadata &value(ExtendedValue value) { return setObject(buf[1], "value", value.c_str(), false, true); }
        // True if the data corresponding to the value is collapsed.
        PivotGroupValueMetadata &collapsed(bool value) { return setObject(buf[2], "collapsed", owriter.getBoolStr(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Information about which values in a pivot group should be used for sorting.
     */
    class PivotGroupSortValueBucket : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotGroupSortValueBucket &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotGroupSortValueBucket() {}
        // The offset in the PivotTable.values list which the values in this grouping should be sorted by.
        PivotGroupSortValueBucket &valuesIndex(int value) { return setObject(buf[1], "valuesIndex", String(value), false, true); }
        // Determines the bucket from which values are chosen to sort.
        // For example, in a pivot table with one row group & two column groups, the row group can list up to two values. The first value corresponds to a value within the first column group, and the second value corresponds to a value in the second column group. If no values are listed, this would indicate that the row should be sorted according to the "Grand Total" over the column groups. If a single value is listed, this would correspond to using the "Total" of that bucket.
        PivotGroupSortValueBucket &addBuckets(const ExtendedValue &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("buckets"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * A group name and a list of items from the source data that should be placed in the group with this name.
     */
    class ManualRuleGroup : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ManualRuleGroup &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ManualRuleGroup() {}
        // The group name, which must be a string. Each group in a given ManualRule must have a unique group name.
        ManualRuleGroup &groupName(const ExtendedValue &value) { return setObject(buf[1], "groupName", value.c_str(), false, true); }
        // The items in the source data that should be placed into this group. Each item may be a string, number, or boolean. Items may appear in at most one group within a given ManualRule. Items that do not appear in any group will appear on their own.
        ManualRuleGroup &addItems(const ExtendedValue &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("items"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Allows you to manually organize the values in a source data column into buckets with names of your choosing. For example, a pivot table that aggregates population by state:
     * +-------+-------------------+
     * tate | SUM of Population |
     * +-------+-------------------+
     * | AK    |               0.7 |
     * | AL    |               4.8 |
     * | AR    |               2.9 |
     * ...
     * +-------+-------------------+
     */
    class ManualRule : public Printable
    {
    private:
        size_t bufSize = 2;
        String buf[2];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ManualRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ManualRule() {}
        // The list of group names and the corresponding items from the source data that map to each group name.
        ManualRule &addGroups(const ManualRuleGroup &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("groups"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Allows you to organize the numeric values in a source data column into buckets of a constant size. All values from HistogramRule.start to HistogramRule.end are placed into groups of size HistogramRule.interval. In addition, all values below HistogramRule.start are placed in one group, and all values above HistogramRule.end are placed in another. Only HistogramRule.interval is required, though if HistogramRule.start and HistogramRule.end are both provided, HistogramRule.start must be less than HistogramRule.end.
     */
    class HistogramRule : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        HistogramRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        HistogramRule() {}
        // The size of the buckets that are created. Must be positive.
        HistogramRule &interval(double value) { return setObject(buf[1], "interval", String(value), false, true); }
        // The minimum value at which items are placed into buckets of constant size. Values below start are lumped into a single bucket. This field is optional.
        HistogramRule &start(double value) { return setObject(buf[2], "start", String(value), false, true); }
        // The maximum value at which items are placed into buckets of constant size. Values above end are lumped into a single bucket. This field is optional.
        HistogramRule &end(double value) { return setObject(buf[3], "end", String(value), false, true); }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * Allows you to organize the date-time values in a source data column into buckets based on selected parts of their date or time values. For example, consider a pivot table showing sales transactions by date:
     * +----------+--------------+
     * | Date     | SUM of Sales |
     * +----------+--------------+
     * | 1/1/2017 |      $621.14 |
     * | 2/3/2017 |      $708.84 |
     * | 5/8/2017 |      $326.84 |
     * ...
     * +----------+--------------+
     */
    class DateTimeRule : public Printable
    {
    private:
        String buf;
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

    public:
        DateTimeRule() {}
        // The size of the buckets that are created. Must be positive.
        DateTimeRule &type(DateTimeRuleType value)
        {
            clear();

            if (value == DATE_TIME_RULE_TYPE_UNSPECIFIED)
                jut.addObject(buf, "type", "DATE_TIME_RULE_TYPE_UNSPECIFIED", true, true);
            else if (value == SECOND)
                jut.addObject(buf, "type", "SECOND", true, true);
            else if (value == _MINUTE)
                jut.addObject(buf, "type", "MINUTE", true, true);
            else if (value == _HOUR)
                jut.addObject(buf, "type", "HOUR", true, true);
            else if (value == HOUR_MINUTE)
                jut.addObject(buf, "type", "HOUR_MINUTE", true, true);
            else if (value == HOUR_MINUTE_AMPM)
                jut.addObject(buf, "type", "HOUR_MINUTE_AMPM", true, true);
            else if (value == DAY_OF_WEEK)
                jut.addObject(buf, "type", "DAY_OF_WEEK", true, true);
            else if (value == DAY_OF_YEAR)
                jut.addObject(buf, "type", "DAY_OF_YEAR", true, true);
            else if (value == DAY_OF_MONTH)
                jut.addObject(buf, "type", "DAY_OF_MONTH", true, true);
            else if (value == DAY_MONTH)
                jut.addObject(buf, "type", "DAY_MONTH", true, true);
            else if (value == MONTH)
                jut.addObject(buf, "type", "MONTH", true, true);
            else if (value == QUARTER)
                jut.addObject(buf, "type", "QUARTER", true, true);
            else if (value == YEAR)
                jut.addObject(buf, "type", "YEAR", true, true);
            else if (value == YEAR_MONTH)
                jut.addObject(buf, "type", "YEAR_MONTH", true, true);
            else if (value == YEAR_QUARTER)
                jut.addObject(buf, "type", "YEAR_QUARTER", true, true);
            else if (value == YEAR_MONTH_DAY)
                jut.addObject(buf, "type", "YEAR_MONTH_DAY", true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * An optional setting on a PivotGroup that defines buckets for the values in the source data column rather than breaking out each individual value. Only one PivotGroup with a group rule may be added for each column in the source data, though on any given column you may add both a PivotGroup that has a rule and a PivotGroup that does not.
     */
    class PivotGroupRule : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotGroupRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotGroupRule() {}
        // A ManualRule.
        PivotGroupRule &manualRule(const ManualRule &value)
        {
            // Union field rule
            if (buf[2].length() == 0 && buf[3].length() == 0)
                return setObject(buf[1], "manualRule", value.c_str(), false, true);
            return *this;
        }
        // A HistogramRule.
        PivotGroupRule &histogramRule(const HistogramRule &value)
        {
            // Union field rule
            if (buf[1].length() == 0 && buf[3].length() == 0)
                return setObject(buf[2], "histogramRule", value.c_str(), false, true);
            return *this;
        }
        // A DateTimeRule.
        PivotGroupRule &dateTimeRule(const DateTimeRule &value)
        {
            // Union field rule
            if (buf[1].length() == 0 && buf[2].length() == 0)
                return setObject(buf[3], "dateTimeRule", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * The count limit on rows or columns in the pivot group.
     */
    class PivotGroupLimit : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotGroupLimit &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotGroupLimit() {}
        // The count limit.
        PivotGroupLimit &countLimit(int value) { return setObject(buf[1], "countLimit", String(value), false, true); }
        // The order in which the group limit is applied to the pivot table.
        // Pivot group limits are applied from lower to higher order number. Order numbers are normalized to consecutive integers from 0.
        // For write request, to fully customize the applying orders, all pivot group limits should have this field set with an unique number. Otherwise, the order is determined by the index in the PivotTable.rows list and then the PivotTable.columns list.
        PivotGroupLimit &applyOrder(int value) { return setObject(buf[2], "applyOrder", String(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A single grouping (either row or column) in a pivot table.
     */
    class PivotGroup : public Printable
    {
    private:
        size_t bufSize = 11;
        String buf[11];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotGroup &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotGroup() {}
        // True if the pivot table should include the totals for this grouping.
        PivotGroup &showTotals(bool value) { return setObject(buf[1], "showTotals", owriter.getBoolStr(value), false, true); }
        // Metadata about values in the grouping.
        PivotGroup &addValueMetadata(const PivotGroupValueMetadata &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("valueMetadata"), value.c_str(), false);
            return *this;
        }
        // The order the values in this group should be sorted./
        PivotGroup &sortOrder(SortOrder value)
        {
            if (value == SORT_ORDER_UNSPECIFIED)
                return setObject(buf[3], "sortOrder", "SORT_ORDER_UNSPECIFIED", true, true);
            else if (value == ASCENDING)
                return setObject(buf[3], "sortOrder", "ASCENDING", true, true);
            else if (value == DESCENDING)
                return setObject(buf[3], "sortOrder", "DESCENDING", true, true);
            return *this;
        }
        // The bucket of the opposite pivot group to sort by. If not specified, sorting is alphabetical by this group's values.
        PivotGroup &valueBucket(const PivotGroupSortValueBucket &value) { return setObject(buf[4], "valueBucket", value.c_str(), false, true); }
        // True if the headings in this pivot group should be repeated. This is only valid for row groupings and is ignored by columns.
        // By default, we minimize repetition of headings by not showing higher level headings where they are the same. For example, even though the third row below corresponds to "Q1 Mar", "Q1" is not shown because it is redundant with previous rows. Setting repeatHeadings to true would cause "Q1" to be repeated for "Feb" and "Mar".
        PivotGroup &repeatHeadings(bool value) { return setObject(buf[5], "repeatHeadings", String(value), false, true); }
        // The labels to use for the row/column groups which can be customized. For example, in the following pivot table, the row label is Region (which could be renamed to State) and the column label is Product (which could be renamed Item). Pivot tables created before December 2017 do not have header labels. If you'd like to add header labels to an existing pivot table, please delete the existing pivot table and then create a new pivot table with same parameters.
        PivotGroup &lable(const String &value) { return setObject(buf[6], "label", value, true, true); }
        // The group rule to apply to this row/column group.
        PivotGroup &groupRule(const PivotGroupRule &value) { return setObject(buf[7], "groupRule", value.c_str(), false, true); }
        // The count limit on rows or columns to apply to this pivot group.
        PivotGroup &groupLimit(const PivotGroupLimit &value) { return setObject(buf[8], "groupLimit", value.c_str(), false, true); }
        // The column offset of the source range that this grouping is based on.
        // For example, if the source was C10:E15, a sourceColumnOffset of 0 means this group refers to column C, whereas the offset 1 would refer to column D.
        PivotGroup &sourceColumnOffset(int value)
        {
            // Union field source
            if (buf[10].length() == 0)
                setObject(buf[9], "sourceColumnOffset", String(value), false, true);
            return *this;
        }
        // The reference to the data source column this grouping is based on.
        PivotGroup &dataSourceColumnReference(const DataSourceColumnReference &value)
        {
            // Union field source
            if (buf[9].length() == 0)
                return setObject(buf[10], "dataSourceColumnReference", value.c_str(), false, true);
            return *this;
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Criteria for showing/hiding rows in a pivot table.
     */
    class PivotFilterCriteria : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotFilterCriteria &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotFilterCriteria() {}
        // Values that should be included. Values not listed here are excluded.
        PivotFilterCriteria &addVisibleValues(const String &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("visibleValues"), value.c_str(), false);
            return *this;
        }
        // A condition that must be true for values to be shown. ( visibleValues does not override this -- even if a value is listed there, it is still hidden if it does not meet the condition.)
        PivotFilterCriteria &condition(const BooleanCondition &value) { return setObject(buf[2], "condition", value.c_str(), false, true); }
        // Whether values are visible by default. If true, the visibleValues are ignored, all values that meet condition (if specified) are shown. If false, values that are both in visibleValues and meet condition are shown.
        PivotFilterCriteria &visibleByDefault(bool value) { return setObject(buf[3], "visibleByDefault", owriter.getBoolStr(value), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
    /**
     * The pivot table filter criteria associated with a specific source column offset.
     */
    class PivotFilterSpec : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotFilterSpec &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotFilterSpec() {}
        // The criteria for the column.
        PivotFilterSpec &filterCriteria(const PivotFilterCriteria &value) { return setObject(buf[1], "filterCriteria", value.c_str(), false, true); }
        // The zero-based column offset of the source range.
        PivotFilterSpec &columnOffsetIndex(int value)
        {
            // Union field source
            if (buf[3].length() == 0)
                return setObject(buf[2], "columnOffsetIndex", String(value), false, true);
            return *this;
        }
        // The reference to the data source column.
        PivotFilterSpec &dataSourceColumnReference(const DataSourceColumnReference &value)
        {
            // Union field source
            if (buf[2].length() == 0)
                return setObject(buf[3], "dataSourceColumnReference", value.c_str(), false, true);
            return *this;
        }

        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * The definition of how a value in a pivot table should be calculated.
     */
    class PivotValue : public Printable
    {
    private:
        size_t bufSize = 7;
        String buf[7];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotValue &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotValue() {}
        // A function to summarize the value. If formula is set, the only supported values are SUM and CUSTOM. If sourceColumnOffset is set, then CUSTOM is not supported.
        PivotValue &summarizeFunction(PivotValueSummarizeFunction value)
        {

            if (value == PIVOT_STANDARD_VALUE_FUNCTION_UNSPECIFIED)
                return setObject(buf[1], "summarizeFunction", "PIVOT_STANDARD_VALUE_FUNCTION_UNSPECIFIED", true, true);
            else if (value == SUM)
                return setObject(buf[1], "summarizeFunction", "SUM", true, true);
            else if (value == COUNTA)
                return setObject(buf[1], "summarizeFunction", "COUNTA", true, true);
            else if (value == COUNT)
                return setObject(buf[1], "summarizeFunction", "COUNT", true, true);
            else if (value == COUNTUNIQUE)
                return setObject(buf[1], "summarizeFunction", "COUNTUNIQUE", true, true);
            else if (value == AVERAGE)
                return setObject(buf[1], "summarizeFunction", "AVERAGE", true, true);
            else if (value == MAX)
                return setObject(buf[1], "summarizeFunction", "MAX", true, true);
            else if (value == MIN)
                return setObject(buf[1], "summarizeFunction", "MIN", true, true);
            else if (value == MEDIAN)
                return setObject(buf[1], "summarizeFunction", "MEDIAN", true, true);
            else if (value == PRODUCT)
                return setObject(buf[1], "summarizeFunction", "PRODUCT", true, true);
            else if (value == STDEV)
                return setObject(buf[1], "summarizeFunction", "STDEV", true, true);
            else if (value == STDEVP)
                return setObject(buf[1], "summarizeFunction", "STDEVP", true, true);
            else if (value == VAR)
                return setObject(buf[1], "summarizeFunction", "VAR", true, true);
            else if (value == VARP)
                return setObject(buf[1], "summarizeFunction", "VARP", true, true);
            else if (value == CUSTOM)
                return setObject(buf[1], "summarizeFunction", "CUSTOM", true, true);
            return *this;
        }
        // A name to use for the value.
        PivotValue &name(const String &value) { return setObject(buf[2], "name", value, true, true); }
        // If specified, indicates that pivot values should be displayed as the result of a calculation with another pivot value. For example, if calculatedDisplayType is specified as PERCENT_OF_GRAND_TOTAL, all the pivot values are displayed as the percentage of the grand total. In the Sheets editor, this is referred to as "Show As" in the value section of a pivot table.
        PivotValue &calculatedDisplayType(PivotValueCalculatedDisplayType value)
        {
            if (value == PIVOT_VALUE_CALCULATED_DISPLAY_TYPE_UNSPECIFIED)
                return setObject(buf[3], "calculatedDisplayType", "PIVOT_VALUE_CALCULATED_DISPLAY_TYPE_UNSPECIFIED", true, true);
            else if (value == PERCENT_OF_ROW_TOTAL)
                return setObject(buf[3], "calculatedDisplayType", "PERCENT_OF_ROW_TOTAL", true, true);
            else if (value == PERCENT_OF_COLUMN_TOTAL)
                return setObject(buf[3], "calculatedDisplayType", "PERCENT_OF_COLUMN_TOTAL", true, true);
            else if (value == PERCENT_OF_GRAND_TOTAL)
                return setObject(buf[3], "calculatedDisplayType", "PERCENT_OF_GRAND_TOTAL", true, true);
            return *this;
        }
        // The column offset of the source range that this value reads from.
        // For example, if the source was C10:E15, a sourceColumnOffset of 0 means this value refers to column C, whereas the offset 1 would refer to column D.
        PivotValue &sourceColumnOffset(int value)
        {
            // Union field value
            if (buf[5].length() == 0 && buf[6].length() == 0)
                return setObject(buf[4], "sourceColumnOffset", String(value), false, true);
            return *this;
        }
        // A custom formula to calculate the value. The formula must start with an = character.
        PivotValue &formula(const String &value)
        {
            // Union field value
            if (buf[4].length() == 0 && buf[6].length() == 0)
                return setObject(buf[5], "formula", value, true, true);
            return *this;
        }
        // The reference to the data source column that this value reads from.
        PivotValue &dataSourceColumnReference(const DataSourceColumnReference &value)
        {
            // Union field value
            if (buf[4].length() == 0 && buf[5].length() == 0)
                return setObject(buf[6], "dataSourceColumnReference", value.c_str(), false, true);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A pivot table.
     */
    class PivotTable : public Printable
    {
    private:
        size_t bufSize = 8;
        String buf[8];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        PivotTable &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        PivotTable() {}
        // Each row grouping in the pivot table.
        PivotTable &addRows(const PivotGroup &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[1], FPSTR("rows"), value.c_str(), false);
            return *this;
        }
        // Each column grouping in the pivot table.
        PivotTable &addColumns(const PivotGroup &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("columns"), value.c_str(), false);
            return *this;
        }
        // The filters applied to the source columns before aggregating data for the pivot table.
        // Both criteria and filterSpecs are populated in responses. If both fields are specified in an update request, this field takes precedence.
        PivotTable &addFilterSpecs(const PivotFilterSpec &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("filterSpecs"), value.c_str(), false);
            return *this;
        }
        // A list of values to include in the pivot table.
        PivotTable &addValues(const PivotValue &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("values"), value.c_str(), false);
            return *this;
        }
        // Whether values should be listed horizontally (as columns) or vertically (as rows).
        PivotTable &valueLayout(PivotValueLayout value)
        {
            if (value == HORIZONTAL)
                return setObject(buf[5], "valueLayout", "HORIZONTAL", true, true);
            else if (value == VERTICAL)
                return setObject(buf[5], "valueLayout", "VERTICAL", true, true);
            return *this;
        }
        // The range the pivot table is reading data from.
        PivotTable &source(const GridRange &value) { return buf[7].length() == 0 ? setObject(buf[6], "source", value.c_str(), false, true) : *this; }
        // The ID of the data source the pivot table is reading data from.
        PivotTable &dataSourceId(const String &value) { return buf[7].length() == 0 ? setObject(buf[7], "dataSourceId", value, true, true) : *this; }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}
#endif