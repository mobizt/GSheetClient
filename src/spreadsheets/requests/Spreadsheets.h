#ifndef SPREADSHEETS_H
#define SPREADSHEETS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Sheets.h"
#include "./spreadsheets/requests/NamedRange.h"

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
        // The spreadsheet theme color pairs. To update you must provide all theme color pairs.
        SpreadsheetTheme &addThemeColors(const ThemeColorPair &value)
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
        size_t bufSize = 7;
        String buf[7];
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
        //  an ISO 639-1 language code such as en
        //  an ISO 639-2 language code such as fil, if no 639-1 code exists
        // a combination of the ISO language code and country code, such as en_US
        // Note: when updating this field, not all locales/languages are supported.
        SpreadsheetProperties &locale(const String &value) { return setObject(buf[2], "locale", value, true, true); }
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
        // Determines whether and how circular references are resolved with iterative calculation. Absence of this field means that circular references result in calculation errors.
        SpreadsheetProperties &iterativeCalculationSettings(const IterativeCalculationSettings &value) { return setObject(buf[5], "iterativeCalculationSettings", value.c_str(), false, true); }
        // Theme applied to the spreadsheet.
        SpreadsheetProperties &spreadsheetTheme(const SpreadsheetTheme &value) { return setObject(buf[6], "spreadsheetTheme", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * Resource that represents a spreadsheet.
     */
    class Spreadsheet : public Printable
    {
    private:
        size_t bufSize = 6;
        String buf[6];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        Spreadsheet &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        Spreadsheet() {}
        // Overall properties of a spreadsheet.
        Spreadsheet &properties(const SpreadsheetProperties &value) { return setObject(buf[1], "properties", value.c_str(), false, true); }
        // The sheets that are part of a spreadsheet.
        Spreadsheet &addSheets(const Sheet &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[2], FPSTR("sheets"), value.c_str(), false);
            return *this;
        }
        // The named ranges defined in a spreadsheet.
        Spreadsheet &addNamedRanges(const NamedRange &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[3], FPSTR("namedRanges"), value.c_str(), false);
            return *this;
        }
        // The developer metadata associated with a spreadsheet.
        Spreadsheet &addDeveloperMetadata(const DeveloperMetadata &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[4], FPSTR("developerMetadata"), value.c_str(), false);
            return *this;
        }
        // A list of external data sources connected with the spreadsheet.
        Spreadsheet &addDataSources(const DataSource &value)
        {
            owriter.addMapArrayMember(buf, bufSize, buf[5], FPSTR("dataSources"), value.c_str(), false);
            return *this;
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}

#endif