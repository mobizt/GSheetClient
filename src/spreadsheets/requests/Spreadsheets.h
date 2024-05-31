#ifndef SPREADSHEETS_H
#define SPREADSHEETS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Sheets.h"
#include "./spreadsheets/requests/NamedRange.h"
#include "./spreadsheets/requests/Common.h"

namespace GSHEET
{

    /**
     * Settings to control how circular dependencies are resolved with iterative calculation.
     */
    class IterativeCalculationSettings : public BaseG4
    {

    public:
        IterativeCalculationSettings() {}

        // When iterative calculation is enabled, the maximum number of calculation rounds to perform.
        IterativeCalculationSettings &maxIterations(int value) { return wr.set<IterativeCalculationSettings &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // When iterative calculation is enabled and successive results differ by less than this threshold value, the calculation rounds stop.
        IterativeCalculationSettings &convergenceThreshold(double value) { return wr.set<IterativeCalculationSettings &, double>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Represents spreadsheet theme
     */
    class SpreadsheetTheme : public BaseG4
    {

    public:
        SpreadsheetTheme() {}

        // Name of the primary font family.
        SpreadsheetTheme &primaryFontFamily(const String &value) { return wr.set<SpreadsheetTheme &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // This value represents the item to add to an array.
        // The spreadsheet theme color pairs. To update you must provide all theme color pairs.
        SpreadsheetTheme &themeColors(const ThemeColorPair &value) { return wr.append<SpreadsheetTheme &, ThemeColorPair>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

    /**
     * Properties of a spreadsheet.
     */
    class SpreadsheetProperties : public BaseG8
    {

    public:
        SpreadsheetProperties() {}

        // The title of the spreadsheet.
        SpreadsheetProperties &title(const String &value) { return wr.set<SpreadsheetProperties &, String>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The locale of the spreadsheet in one of the following formats:
        //  an ISO 639-1 language code such as en
        //  an ISO 639-2 language code such as fil, if no 639-1 code exists
        // a combination of the ISO language code and country code, such as en_US
        // Note: when updating this field, not all locales/languages are supported.
        SpreadsheetProperties &locale(const String &value) { return wr.set<SpreadsheetProperties &, String>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The amount of time to wait before volatile functions are recalculated.
        SpreadsheetProperties &autoRecalc(UpdateInterval::RecalculationInterval value) { return wr.set<SpreadsheetProperties &, const char *>(*this, _RecalculationInterval[value].text, buf, bufSize, 3, FPSTR(__func__)); }

        // The time zone of the spreadsheet, in CLDR format such as America/New_York. If the time zone isn't recognized, this may be a custom time zone such as GMT-07:00.
        SpreadsheetProperties &timeZone(const String &value) { return wr.set<SpreadsheetProperties &, String>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }

        // Determines whether and how circular references are resolved with iterative calculation. Absence of this field means that circular references result in calculation errors.
        SpreadsheetProperties &iterativeCalculationSettings(const IterativeCalculationSettings &value) { return wr.set<SpreadsheetProperties &, IterativeCalculationSettings>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }

        // Theme applied to the spreadsheet.
        SpreadsheetProperties &spreadsheetTheme(const SpreadsheetTheme &value) { return wr.set<SpreadsheetProperties &, SpreadsheetTheme>(*this, value, buf, bufSize, 6, FPSTR(__func__)); }
    };

    /**
     * Resource that represents a spreadsheet.
     */
    class Spreadsheet : public BaseG6
    {

    public:
        Spreadsheet() {}

        // Overall properties of a spreadsheet.
        Spreadsheet &properties(const SpreadsheetProperties &value) { return wr.set<Spreadsheet &, SpreadsheetProperties>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // Add to list
        // The sheets that are part of a spreadsheet.
        Spreadsheet &sheets(const Sheet &value) { return wr.append<Spreadsheet &, Sheet>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // Add to list
        // The named ranges defined in a spreadsheet.
        Spreadsheet &namedRanges(const NamedRange &value) { return wr.append<Spreadsheet &, NamedRange>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
        // Add to list
        // The developer metadata associated with a spreadsheet.
        Spreadsheet &developerMetadata(const DeveloperMetadata &value) { return wr.append<Spreadsheet &, DeveloperMetadata>(*this, value, buf, bufSize, 4, FPSTR(__func__)); }
        // Add to list
        // A list of external data sources connected with the spreadsheet.
        Spreadsheet &dataSources(const DataSource &value) { return wr.append<Spreadsheet &, DataSource>(*this, value, buf, bufSize, 5, FPSTR(__func__)); }
    };
}

#endif