#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Theme.h"

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

    // The kind of sheet
    enum SheetType
    {
        SHEET_TYPE_UNSPECIFIED, //	Default value, do not use.
        GRID,                   //	The sheet is a grid.
        OBJECT,                 //	The sheet has no grid and instead has an object like a chart or image.
        DATA_SOURCE             //	The sheet connects with an external DataSource and shows the preview of data.
    };

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
     * A coordinate in a sheet. All indexes are zero-based.
     */
    class GridCoordinate : public Printable
    {
    private:
        size_t bufSize = 4;
        String buf[4];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        GridCoordinate &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        GridCoordinate() {}
        // The sheet this coordinate is on.
        GridCoordinate &sheetId(int value) { return setObject(buf[1], "sheetId", String(value), false, true); }
        // The row index of the coordinate.
        GridCoordinate &rowIndex(int value) { return setObject(buf[2], "rowIndex", String(value), false, true); }
        // The column index of the coordinate.
        GridCoordinate &columnIndex(int value) { return setObject(buf[3], "columnIndex", String(value), false, true); }
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
        DataSourceFormula &dataSourceId(const String &value)
        {
            clear();
            jut.addObject(buf, "dataSourceId", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };
    

}
#endif