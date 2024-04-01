#ifndef CELL_H
#define CELL_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/PivotTable.h"
#include "./spreadsheets/requests/BooleanCondition.h"
#include "./spreadsheets/requests/DataSourceTable.h"
#include "./spreadsheets/requests/CellFormat.h"
#include "./spreadsheets/requests/GridRange.h"

/**
 * CELL DATA CLASS DEPENDENCIES
 *
 * CellData     +                           ExtendedValue
 *              |
 *              +                           CellFormat*
 *              |
 *              +                           TextFormatRun           +       TextFormat              +           ColorStyle*
 *              +                           DataValidationRule      +       BooleanCondition*
 *              +                           PivotTable*
 *              |
 *              +                           DataSourceTable         +       (enum) DataSourceTableColumnSelectionType
 *              |                                                   |
 *              |                                                   +       DataSourceColumnReference
 *              |                                                   |
 *              |                                                   +       FilterSpec*
 *              |                                                   |
 *              |                                                   +       SortSpec*
 *              |
 *              +                           DataSourceFormula
 *
 * See PivotTable.h, BooleanCondition.h, Theme.h, FilterSpec.h, SortSpec.h, CellFormat.h
 */

namespace GSHEET
{

    /**
     * A run of a text format. The format of this run continues until the start index of the next run. When updating, all fields must be set.
     */
    class TextFormatRun : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        TextFormatRun &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        TextFormatRun() {}
        // The zero-based character index where this run starts, in UTF-16 code units.
        TextFormatRun &startIndex(int value) { return setObject(buf[1], "startIndex", String(value), false, true); }
        // The format of this run. Absent values inherit the cell's format.
        TextFormatRun &format(const TextFormat &value) { return setObject(buf[2], "format", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A data validation rule.
     */
    class DataValidationRule : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        DataValidationRule &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        DataValidationRule() {}
        // The condition that data in the cell must match.
        DataValidationRule &condition(const BooleanCondition value) { return setObject(buf[1], "condition", value.c_str(), false, true); }
        // A message to show the user when adding data to the cell.
        DataValidationRule &inputMessage(const String &value) { return setObject(buf[2], "inputMessage", value, true, true); }
        // True if invalid data should be rejected.
        DataValidationRule &strict(bool value) { return setObject(buf[3], "strict", owriter.getBoolStr(value), false, true); }
        // True if the UI should be customized based on the kind of condition. If true, "List" conditions will show a dropdown.
        DataValidationRule &showCustomUi(bool value) { return setObject(buf[4], "showCustomUi", owriter.getBoolStr(value), false, true); }
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
        String buf;
        GSheetJSONUtil jut;
        GSheetObjectWriter owriter;

    public:
        ExtendedValue() {}
        // Union field value
        // Represents a double value. Note: Dates, Times and DateTimes are represented as doubles in SERIAL_NUMBER format.
        ExtendedValue &numberValue(double value)
        {
            clear();
            jut.addObject(buf, "numberValue", String(value), false, true);
            return *this;
        }
        // Union field value
        // Represents a string value. Leading single quotes are not included. For example, if the user typed '123 into the UI, this would be represented as a stringValue of "123".
        ExtendedValue &stringValue(const String &value)
        {
            clear();
            jut.addObject(buf, "stringValue", value, true, true);
            return *this;
        }
        // Union field value
        // Represents a boolean value.
        ExtendedValue &boolValue(bool value)
        {
            clear();
            jut.addObject(buf, "boolValue", owriter.getBoolStr(value), false, true);
            return *this;
        }
        // Union field value
        // Represents a formula.
        ExtendedValue &formulaValue(const String &value)
        {
            clear();
            jut.addObject(buf, "formulaValue", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * Data about a specific cell.
     */
    class CellData : public Printable
    {
    private:
        size_t bufSize = 10;
        String buf[10];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        CellData &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        CellData() {}
        // The value the user entered in the cell. e.g., 1234, 'Hello', or =NOW() Note: Dates, Times and DateTimes are represented as doubles in serial number format.
        CellData &userEnteredValue(const ExtendedValue &value) { return setObject(buf[1], "userEnteredValue", value.c_str(), false, true); }
        // The formatted value of the cell. This is the value as it's shown to the user. This field is read-only.
        CellData &formattedValue(const String &value) { return setObject(buf[2], "formattedValue", value, true, true); }
        // The format the user entered for the cell.
        // When writing, the new format will be merged with the existing format.
        CellData &userEnteredFormat(const CellFormat &value) { return setObject(buf[3], "userEnteredFormat", value.c_str(), false, true); }
        // A hyperlink this cell points to, if any. If the cell contains multiple hyperlinks, this field will be empty. This field is read-only. To set it, use a =HYPERLINK formula in the userEnteredValue.formulaValue field. A cell-level link can also be set from the userEnteredFormat.textFormat field. Alternatively, set a hyperlink in the textFormatRun.format.link field that spans the entire cell.
        CellData &hyperlink(const String &value) { return setObject(buf[4], "hyperlink", value, true, true); }
        // Any note on the cell.
        CellData &note(const String &value) { return setObject(buf[5], "note", value, true, true); }
        // Runs of rich text applied to subsections of the cell. Runs are only valid on user entered strings, not formulas, bools, or numbers. Properties of a run start at a specific index in the text and continue until the next run. Runs will inherit the properties of the cell unless explicitly changed.
        // When writing, the new runs will overwrite any prior runs. When writing a new userEnteredValue, previous runs are erased.
        CellData &addTextFormatRuns(const TextFormatRun &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[6], FPSTR("textFormatRuns"), value.c_str(), false);
            return *this;
        }
        // A data validation rule on the cell, if any.
        // When writing, the new data validation rule will overwrite any prior rule.
        CellData &dataValidation(const DataValidationRule &value) { return setObject(buf[7], "dataValidation", value.c_str(), false, true); }
        // A pivot table anchored at this cell. The size of pivot table itself is computed dynamically based on its data, grouping, filters, values, etc. Only the top-left cell of the pivot table contains the pivot table definition. The other cells will contain the calculated values of the results of the pivot in their effectiveValue fields.
        CellData &pivotTable(const PivotTable &value) { return setObject(buf[8], "pivotTable", value.c_str(), false, true); }
        // A data source table anchored at this cell. The size of data source table itself is computed dynamically based on its configuration. Only the first cell of the data source table contains the data source table definition. The other cells will contain the display values of the data source table result in their effectiveValue fields.
        CellData &dataSourceTable(const DataSourceTable &value) { return setObject(buf[9], "dataSourceTable", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };
}
#endif