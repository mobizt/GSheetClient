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
    class TextFormatRun : public BaseG4
    {

    public:
        TextFormatRun() = default;

        // The zero-based character index where this run starts, in UTF-16 code units.
        TextFormatRun &startIndex(int value) { return wr.set<TextFormatRun &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The format of this run. Absent values inherit the cell's format.
        TextFormatRun &format(const TextFormat &value) { return wr.set<TextFormatRun &, TextFormat>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * A data validation rule.
     */
    class DataValidationRule : public BaseG6
    {

    public:
        DataValidationRule() = default;

        // The condition that data in the cell must match.
        DataValidationRule &condition(const BooleanCondition value) { return wr.set<DataValidationRule &, BooleanCondition>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // A message to show the user when adding data to the cell.
        DataValidationRule &inputMessage(const String &value) { return wr.set<DataValidationRule &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // True if invalid data should be rejected.
        DataValidationRule &strict(bool value) { return wr.set<DataValidationRule &, bool>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // True if the UI should be customized based on the kind of condition. If true, "List" conditions will show a dropdown.
        DataValidationRule &showCustomUi(bool value) { return wr.set<DataValidationRule &, bool>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * Data about a specific cell.
     */
    class CellData : public BaseG12
    {

    public:
        CellData() = default;

        // The value the user entered in the cell. e.g., 1234, 'Hello', or =NOW() Note: Dates, Times and DateTimes are represented as doubles in serial number format.
        CellData &userEnteredValue(const ExtendedValue &value) { return wr.set<CellData &, ExtendedValue>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The formatted value of the cell. This is the value as it's shown to the user. This field is read-only.
        CellData &formattedValue(const String &value) { return wr.set<CellData &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The format the user entered for the cell.
        // When writing, the new format will be merged with the existing format.
        CellData &userEnteredFormat(const CellFormat &value) { return wr.set<CellData &, CellFormat>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }

        // A hyperlink this cell points to, if any. If the cell contains multiple hyperlinks, this field will be empty. This field is read-only. To set it, use a =HYPERLINK formula in the userEnteredValue.formulaValue field. A cell-level link can also be set from the userEnteredFormat.textFormat field. Alternatively, set a hyperlink in the textFormatRun.format.link field that spans the entire cell.
        CellData &hyperlink(const String &value) { return wr.set<CellData &, String>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Any note on the cell.
        CellData &note(const String &value) { return wr.set<CellData &, String>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        //  Runs of rich text applied to subsections of the cell. Runs are only valid on user entered strings, not formulas, bools, or numbers. Properties of a run start at a specific index in the text and continue until the next run. Runs will inherit the properties of the cell unless explicitly changed.
        //  When writing, the new runs will overwrite any prior runs. When writing a new userEnteredValue, previous runs are erased.
        CellData &textFormatRuns(const TextFormatRun &value) { return wr.append<CellData &, TextFormatRun>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }

        // A data validation rule on the cell, if any.
        // When writing, the new data validation rule will overwrite any prior rule.
        CellData &dataValidation(const DataValidationRule &value) { return wr.set<CellData &, DataValidationRule>(*this, value, buf, bufSize, 7, FPSTR(__func__)); }

        // A pivot table anchored at this cell. The size of pivot table itself is computed dynamically based on its data, grouping, filters, values, etc. Only the top-left cell of the pivot table contains the pivot table definition. The other cells will contain the calculated values of the results of the pivot in their effectiveValue fields.
        CellData &pivotTable(const PivotTable &value) { return wr.set<CellData &, PivotTable>(*this, value, buf, bufSize, 8, FPSTR(__func__)); }

        // A data source table anchored at this cell. The size of data source table itself is computed dynamically based on its configuration. Only the first cell of the data source table contains the data source table definition. The other cells will contain the display values of the data source table result in their effectiveValue fields.
        CellData &dataSourceTable(const DataSourceTable &value) { return wr.set<CellData &, DataSourceTable>(*this, value, buf, bufSize, 9, FPSTR(__func__)); }
    };
}
#endif