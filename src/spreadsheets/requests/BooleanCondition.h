#ifndef BOOLEAN_CONDITION_H
#define BOOLEAN_CONDITION_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

/**
 * BOOLEAN CONDITION CLASS DEPENDENCIES
 *
 * BooleanCondition     +   (enum) ConditionType
 *                      |
 *                      +   ConditionValue          +   (enum) RelativeDate
 */

namespace GSHEET
{
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

    /**
     * The value of the condition.
     */
    class ConditionValue : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        ConditionValue() {}
        // Union field value
        //  A relative date (based on the current date). Valid only if the type is DATE_BEFORE, DATE_AFTER, DATE_ON_OR_BEFORE or DATE_ON_OR_AFTER.
        //  Relative dates are not supported in data validation. They are supported only in conditional formatting and conditional filters.
        ConditionValue &relativeDate(RelativeDate value)
        {
            clear();
            if (value == RELATIVE_DATE_UNSPECIFIED)
                jut.addObject(buf, "relativeDate", "RELATIVE_DATE_UNSPECIFIED", true, true);
            else if (value == PAST_YEAR)
                jut.addObject(buf, "relativeDate", "PAST_YEAR", true, true);
            else if (value == PAST_MONTH)
                jut.addObject(buf, "relativeDate", "PAST_MONTH", true, true);
            else if (value == PAST_WEEK)
                jut.addObject(buf, "relativeDate", "PAST_WEEK", true, true);
            else if (value == YESTERDAY)
                jut.addObject(buf, "relativeDate", "YESTERDAY", true, true);
            else if (value == TODAY)
                jut.addObject(buf, "relativeDate", "TODAY", true, true);
            else if (value == TOMORROW)
                jut.addObject(buf, "relativeDate", "TOMORROW", true, true);
            return *this;
        }
        // Union field value
        //  A value the condition is based on. The value is parsed as if the user typed into a cell. Formulas are supported (and must begin with an = or a '+').
        ConditionValue &userEnteredValue(const String &value)
        {
            clear();
            jut.addObject(buf, "userEnteredValue", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
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

}

#endif