#ifndef PIVOT_TABLE_H
#define PIVOT_TABLE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/BooleanCondition.h"
#include "./spreadsheets/requests/SortSpec.h"
#include "./spreadsheets/requests/GridRange.h"
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
 *              +   PivotFilterSpec             +   PivotFilterCriteria                     +   BooleanCondition*
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
 *
 * See BooleanCondition.h
 */

namespace GSHEET
{

    /**
     * The kinds of value that a cell in a spreadsheet can have.
     */
    class ExtendedValue : public O1
    {
    public:
        ExtendedValue() {}
        // Union field value
        // Represents a double value. Note: Dates, Times and DateTimes are represented as doubles in SERIAL_NUMBER format.
        ExtendedValue &numberValue(double value) { return wr.add<ExtendedValue &, double>(*this, value, buf, FPSTR(__func__)); }
        // Union field value
        // Represents a string value. Leading single quotes are not included. For example, if the user typed '123 into the UI, this would be represented as a stringValue of "123".
        ExtendedValue &stringValue(const String &value) { return wr.add<ExtendedValue &, String>(*this, value, buf, FPSTR(__func__)); }
        // Union field value
        // Represents a boolean value.
        ExtendedValue &boolValue(bool value) { return wr.add<ExtendedValue &, bool>(*this, value, buf, FPSTR(__func__)); }
        // Union field value
        // Represents a formula.
        ExtendedValue &formulaValue(const String &value) { return wr.add<ExtendedValue &, String>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Metadata about a value in a pivot grouping.
     */
    class PivotGroupValueMetadata : public O4
    {

    public:
        PivotGroupValueMetadata() {}
        // The calculated value the metadata corresponds to. (Note that formulaValue is not valid, because the values will be calculated.)
        PivotGroupValueMetadata &value(ExtendedValue value) { return wr.set<PivotGroupValueMetadata &, ExtendedValue>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // True if the data corresponding to the value is collapsed.
        PivotGroupValueMetadata &collapsed(bool value) { return wr.set<PivotGroupValueMetadata &, bool>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * Information about which values in a pivot group should be used for sorting.
     */
    class PivotGroupSortValueBucket : public O4
    {
    public:
        PivotGroupSortValueBucket() {}
        // The offset in the PivotTable.values list which the values in this grouping should be sorted by.
        PivotGroupSortValueBucket &valuesIndex(int value) { return wr.set<PivotGroupSortValueBucket &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Add to a list
        // Determines the bucket from which values are chosen to sort.
        // For example, in a pivot table with one row group & two column groups, the row group can list up to two values. The first value corresponds to a value within the first column group, and the second value corresponds to a value in the second column group. If no values are listed, this would indicate that the row should be sorted according to the "Grand Total" over the column groups. If a single value is listed, this would correspond to using the "Total" of that bucket.
        PivotGroupSortValueBucket &buckets(const ExtendedValue &value) { return wr.append<PivotGroupSortValueBucket &, ExtendedValue>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * A group name and a list of items from the source data that should be placed in the group with this name.
     */
    class ManualRuleGroup : public O4
    {
    public:
        ManualRuleGroup() {}
        // The group name, which must be a string. Each group in a given ManualRule must have a unique group name.
        ManualRuleGroup &groupName(const ExtendedValue &value) { return wr.set<ManualRuleGroup &, ExtendedValue>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Add to a list
        //  The items in the source data that should be placed into this group. Each item may be a string, number, or boolean. Items may appear in at most one group within a given ManualRule. Items that do not appear in any group will appear on their own.
        ManualRuleGroup &items(const ExtendedValue &value) { return wr.append<ManualRuleGroup &, ExtendedValue>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
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
    class ManualRule : public O1
    {

    public:
        ManualRule() {}
        // The list of group names and the corresponding items from the source data that map to each group name.
        ManualRule &addGroups(const ManualRuleGroup &value) { return wr.add<ManualRule &, ManualRuleGroup>(*this, value, buf, FPSTR(__func__)); }
    };

    /**
     * Allows you to organize the numeric values in a source data column into buckets of a constant size. All values from HistogramRule.start to HistogramRule.end are placed into groups of size HistogramRule.interval. In addition, all values below HistogramRule.start are placed in one group, and all values above HistogramRule.end are placed in another. Only HistogramRule.interval is required, though if HistogramRule.start and HistogramRule.end are both provided, HistogramRule.start must be less than HistogramRule.end.
     */
    class HistogramRule : public O4
    {
    public:
        HistogramRule() {}
        // The size of the buckets that are created. Must be positive.
        HistogramRule &interval(double value) { return wr.set<HistogramRule &, double>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The minimum value at which items are placed into buckets of constant size. Values below start are lumped into a single bucket. This field is optional.
        HistogramRule &start(double value) { return wr.set<HistogramRule &, double>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The maximum value at which items are placed into buckets of constant size. Values above end are lumped into a single bucket. This field is optional.
        HistogramRule &end(double value) { return wr.set<HistogramRule &, double>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
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
    class DateTimeRule : public O1
    {
    public:
        DateTimeRule() {}
        // The size of the buckets that are created. Must be positive.
        DateTimeRule &type(Group::DateTimeRuleType value) { return wr.add<DateTimeRule &, const char *>(*this, _DateTimeRuleType[value].text, buf, FPSTR(__func__)); }
    };

    /**
     * An optional setting on a PivotGroup that defines buckets for the values in the source data column rather than breaking out each individual value. Only one PivotGroup with a group rule may be added for each column in the source data, though on any given column you may add both a PivotGroup that has a rule and a PivotGroup that does not.
     */
    class PivotGroupRule : public O1
    {
    public:
        PivotGroupRule() {}
        // Union field rule
        // A ManualRule.
        PivotGroupRule &manualRule(const ManualRule &value) { return wr.add<PivotGroupRule &, ManualRule>(*this, value, buf, FPSTR(__func__)); }
        // Union field rule
        // A HistogramRule.
        PivotGroupRule &histogramRule(const HistogramRule &value) { return wr.add<PivotGroupRule &, HistogramRule>(*this, value, buf, FPSTR(__func__)); }
        // Union field rule
        // A DateTimeRule.
        PivotGroupRule &dateTimeRule(const DateTimeRule &value) { return wr.add<PivotGroupRule &, DateTimeRule>(*this, value, buf, FPSTR(__func__)); }
    };
    /**
     * The count limit on rows or columns in the pivot group.
     */
    class PivotGroupLimit : public O4
    {
    public:
        PivotGroupLimit() {}
        // The count limit.
        PivotGroupLimit &countLimit(int value) { return wr.set<PivotGroupLimit &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The order in which the group limit is applied to the pivot table.
        // Pivot group limits are applied from lower to higher order number. Order numbers are normalized to consecutive integers from 0.
        // For write request, to fully customize the applying orders, all pivot group limits should have this field set with an unique number. Otherwise, the order is determined by the index in the PivotTable.rows list and then the PivotTable.columns list.
        PivotGroupLimit &applyOrder(int value) { return wr.set<PivotGroupLimit &, int>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * A single grouping (either row or column) in a pivot table.
     */
    class PivotGroup : public O12
    {
    public:
        PivotGroup() {}
        // True if the pivot table should include the totals for this grouping.
        PivotGroup &showTotals(bool value) { return wr.set<PivotGroup &, bool>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Add to a list
        // Metadata about values in the grouping.
        PivotGroup &valueMetadata(const PivotGroupValueMetadata &value) { return wr.append<PivotGroup &, PivotGroupValueMetadata>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The order the values in this group should be sorted./
        PivotGroup &sortOrder(Sort::SortOrder value) { return wr.set<PivotGroup &, bool>(*this, _SortOrder[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
        // The bucket of the opposite pivot group to sort by. If not specified, sorting is alphabetical by this group's values.
        PivotGroup &valueBucket(const PivotGroupSortValueBucket &value) { return wr.set<PivotGroup &, PivotGroupSortValueBucket>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // True if the headings in this pivot group should be repeated. This is only valid for row groupings and is ignored by columns.
        // By default, we minimize repetition of headings by not showing higher level headings where they are the same. For example, even though the third row below corresponds to "Q1 Mar", "Q1" is not shown because it is redundant with previous rows. Setting repeatHeadings to true would cause "Q1" to be repeated for "Feb" and "Mar".
        PivotGroup &repeatHeadings(bool value) { return wr.set<PivotGroup &, bool>(*this, value, buf, bufSize, buf[5], FPSTR(__func__)); }
        // The labels to use for the row/column groups which can be customized. For example, in the following pivot table, the row label is Region (which could be renamed to State) and the column label is Product (which could be renamed Item). Pivot tables created before December 2017 do not have header labels. If you'd like to add header labels to an existing pivot table, please delete the existing pivot table and then create a new pivot table with same parameters.
        PivotGroup &lable(const String &value) { return wr.set<PivotGroup &, String>(*this, value, buf, bufSize, buf[6], FPSTR(__func__)); }
        // The group rule to apply to this row/column group.
        PivotGroup &groupRule(const PivotGroupRule &value) { return wr.set<PivotGroup &, PivotGroupRule>(*this, value, buf, bufSize, buf[7], FPSTR(__func__)); }
        // The count limit on rows or columns to apply to this pivot group.
        PivotGroup &groupLimit(const PivotGroupLimit &value) { return wr.set<PivotGroup &, PivotGroupLimit>(*this, value, buf, bufSize, buf[8], FPSTR(__func__)); }
        // Union field source
        // The column offset of the source range that this grouping is based on.
        // For example, if the source was C10:E15, a sourceColumnOffset of 0 means this group refers to column C, whereas the offset 1 would refer to column D.
        PivotGroup &sourceColumnOffset(int value) { return wr.set<PivotGroup &, int>(*this, value, buf, bufSize, buf[9], FPSTR(__func__)); }
        // Union field source
        // The reference to the data source column this grouping is based on.
        PivotGroup &dataSourceColumnReference(const DataSourceColumnReference &value) { return wr.set<PivotGroup &, DataSourceColumnReference>(*this, value, buf, bufSize, buf[9], FPSTR(__func__)); }
    };

    /**
     * Criteria for showing/hiding rows in a pivot table.
     */
    class PivotFilterCriteria : public O4
    {

    public:
        PivotFilterCriteria() {}
        // Values that should be included. Values not listed here are excluded.
        PivotFilterCriteria &addVisibleValues(const String &value) { return wr.append<PivotFilterCriteria &, String>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // A condition that must be true for values to be shown. ( visibleValues does not override this -- even if a value is listed there, it is still hidden if it does not meet the condition.)
        PivotFilterCriteria &condition(const BooleanCondition &value) { return wr.set<PivotFilterCriteria &, BooleanCondition>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Whether values are visible by default. If true, the visibleValues are ignored, all values that meet condition (if specified) are shown. If false, values that are both in visibleValues and meet condition are shown.
        PivotFilterCriteria &visibleByDefault(bool value) { return wr.set<PivotFilterCriteria &, bool>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
    };
    /**
     * The pivot table filter criteria associated with a specific source column offset.
     */
    class PivotFilterSpec : public O4
    {
    public:
        PivotFilterSpec() {}
        // The criteria for the column.
        PivotFilterSpec &filterCriteria(const PivotFilterCriteria &value) { return wr.set<PivotFilterSpec &, PivotFilterCriteria>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Union field source
        // The zero-based column offset of the source range.
        PivotFilterSpec &columnOffsetIndex(int value) { return wr.set<PivotFilterSpec &, int>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Union field source
        // The reference to the data source column.
        PivotFilterSpec &dataSourceColumnReference(const DataSourceColumnReference &value) { return wr.set<PivotFilterSpec &, DataSourceColumnReference>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
    };

    /**
     * The definition of how a value in a pivot table should be calculated.
     */
    class PivotValue : public O6
    {
    public:
        PivotValue() {}
        // A function to summarize the value. If formula is set, the only supported values are SUM and CUSTOM. If sourceColumnOffset is set, then CUSTOM is not supported.
        PivotValue &summarizeFunction(Function::PivotValueSummarizeFunction value) { return wr.set<PivotValue &, const char *>(*this, _PivotValueSummarizeFunction[value].text, buf, bufSize, buf[1], FPSTR(__func__)); }
        // A name to use for the value.
        PivotValue &name(const String &value) { return wr.set<PivotValue &, String>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // If specified, indicates that pivot values should be displayed as the result of a calculation with another pivot value. For example, if calculatedDisplayType is specified as PERCENT_OF_GRAND_TOTAL, all the pivot values are displayed as the percentage of the grand total. In the Sheets editor, this is referred to as "Show As" in the value section of a pivot table.
        PivotValue &calculatedDisplayType(Display::PivotValueCalculatedDisplayType value) { return wr.set<PivotValue &, const char *>(*this, _PivotValueCalculatedDisplayType[value].text, buf, bufSize, buf[3], FPSTR(__func__)); }
        // Union field value
        // The column offset of the source range that this value reads from.
        // For example, if the source was C10:E15, a sourceColumnOffset of 0 means this value refers to column C, whereas the offset 1 would refer to column D.
        PivotValue &sourceColumnOffset(int value) { return wr.set<PivotValue &, int>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // Union field value
        // A custom formula to calculate the value. The formula must start with an = character.
        PivotValue &formula(const String &value) { return wr.set<PivotValue &, String>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // Union field value
        // The reference to the data source column that this value reads from.
        PivotValue &dataSourceColumnReference(const DataSourceColumnReference &value) { return wr.set<PivotValue &, DataSourceColumnReference>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
    };

    /**
     * A pivot table.
     */
    class PivotTable : public O8
    {

    public:
        PivotTable() {}
        // Add to a list
        //  Each row grouping in the pivot table.
        PivotTable &rows(const PivotGroup &value) { return wr.append<PivotTable &, PivotGroup>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // Add to a list
        //  Each column grouping in the pivot table.
        PivotTable &columns(const PivotGroup &value) { return wr.append<PivotTable &, PivotGroup>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // Add to a list
        //  The filters applied to the source columns before aggregating data for the pivot table.
        //  Both criteria and filterSpecs are populated in responses. If both fields are specified in an update request, this field takes precedence.
        PivotTable &filterSpecs(const PivotFilterSpec &value) { return wr.append<PivotTable &, PivotFilterSpec>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // Add to a list
        // A list of values to include in the pivot table.
        PivotTable &values(const PivotValue &value) { return wr.append<PivotTable &, PivotValue>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // Whether values should be listed horizontally (as columns) or vertically (as rows).
        PivotTable &valueLayout(Direction::PivotValueLayout value) { return wr.set<PivotTable &, const char *>(*this, _PivotValueLayout[value].text, buf, bufSize, buf[5], FPSTR(__func__)); }
        // Union field source_data
        // The range the pivot table is reading data from.
        PivotTable &source(const GridRange &value) { return wr.set<PivotTable &, GridRange>(*this, value, buf, bufSize, buf[6], FPSTR(__func__)); }
        // Union field source_data
        // The ID of the data source the pivot table is reading data from.
        PivotTable &dataSourceId(const String &value) { return wr.set<PivotTable &, String>(*this, value, buf, bufSize, buf[6], FPSTR(__func__)); }
    };

}
#endif