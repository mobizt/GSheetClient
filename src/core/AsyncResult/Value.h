/**
 * Created March 26, 2024
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GSHEET_VALUE_H
#define GSHEET_VALUE_H
#include <Arduino.h>
#include <string>

struct gsheet_boolean_t : public Printable
{
private:
    String buf;
    gsheet_boolean_t &copy(bool rhs)
    {
        buf = rhs ? FPSTR("true") : FPSTR("false");
        return *this;
    }

public:
    gsheet_boolean_t() {}
    gsheet_boolean_t(bool v) { buf = v ? FPSTR("true") : FPSTR("false"); }
    const char *c_str() const { return buf.c_str(); }
    size_t printTo(Print &p) const { return p.print(buf.c_str()); }
};

struct gsheet_number_t : public Printable
{
private:
    String buf;

public:
    gsheet_number_t() {}
    template <typename T1 = int, typename T = int>
    gsheet_number_t(T1 v, T d) { buf = String(v, d); }
    template <typename T = int>
    gsheet_number_t(T o) { buf = String(o); }
    const char *c_str() const { return buf.c_str(); }
    size_t printTo(Print &p) const { return p.print(buf.c_str()); }
};

struct gsheet_string_t : public Printable
{
private:
    String buf;

public:
    gsheet_string_t() {}
    template <typename T = const char *>
    gsheet_string_t(T v)
    {
        aq(true);
        if (std::is_same<T, bool>::value)
            buf += v ? FPSTR("true") : FPSTR("false");
        else
            buf += v;
        aq();
    }
    gsheet_string_t(gsheet_number_t v)
    {
        aq(true);
        buf += v.c_str();
        aq();
    }
    gsheet_string_t(gsheet_boolean_t v)
    {
        aq(true);
        buf += v.c_str();
        aq();
    }
    template <typename T>
    auto operator+=(const T &rval) -> typename std::enable_if<std::is_same<T, gsheet_number_t>::value || std::is_same<T, gsheet_boolean_t>::value, gsheet_string_t &>::type
    {
        sap();
        buf += rval.c_str();
        aq();
        return *this;
    }

    template <typename T>
    auto operator+=(const T &rval) -> typename std::enable_if<!std::is_same<T, gsheet_number_t>::value && !std::is_same<T, gsheet_boolean_t>::value, gsheet_string_t &>::type
    {
        sap();
        buf += rval;
        aq();
        return *this;
    }

    const char *c_str() const { return buf.c_str(); }
    size_t printTo(Print &p) const { return p.print(buf.c_str()); }
    void clear() { buf.remove(0, buf.length()); }

private:
    void sap()
    {
        String temp;
        if (buf.length())
            temp = buf.substring(1, buf.length() - 1);
        aq(true);
        buf += temp;
    }
    void aq(bool clear = false)
    {
        if (clear)
            buf.remove(0, buf.length());
        buf += '"';
    }
};

struct gsheet_object_t : public Printable
{
    friend class GSheetJsonWriter;

private:
    String buf;

public:
    gsheet_object_t() {}
    gsheet_object_t(const String &o) { buf = o; }
    const char *c_str() const { return buf.c_str(); }
    template <typename T = const char *>
    gsheet_object_t(T o) { buf = String(o); }
    gsheet_object_t(gsheet_boolean_t o) { buf = o.c_str(); }
    gsheet_object_t(gsheet_number_t o) { buf = o.c_str(); }
    gsheet_object_t(gsheet_string_t o) { buf = o.c_str(); }
    gsheet_object_t(bool o) { buf = o ? FPSTR("true") : FPSTR("false"); }
    size_t printTo(Print &p) const { return p.print(buf.c_str()); }
    void clear() { buf.remove(0, buf.length()); }
    void initObject() { buf = FPSTR("{}"); };
    void initArray() { buf = FPSTR("[]"); };

private:
    explicit operator bool() const { return buf.length() > 0; }

    template <typename T = String>
    auto operator+=(const T &rval) -> typename std::enable_if<!std::is_same<T, gsheet_object_t>::value && !std::is_same<T, gsheet_string_t>::value && !std::is_same<T, gsheet_number_t>::value && !std::is_same<T, gsheet_boolean_t>::value, gsheet_object_t &>::type
    {
        buf += rval;
        return *this;
    }

    template <typename T>
    auto operator+=(const T &rval) -> typename std::enable_if<std::is_same<T, gsheet_object_t>::value || std::is_same<T, gsheet_string_t>::value || std::is_same<T, gsheet_number_t>::value || std::is_same<T, gsheet_boolean_t>::value, gsheet_object_t &>::type
    {
        buf += rval.c_str();
        return *this;
    }

    size_t length() const { return buf.length(); }
    gsheet_object_t substring(unsigned int beginIndex, unsigned int endIndex) const { return buf.substring(beginIndex, endIndex); }
};

class GSheetValueConverter
{
public:
    GSheetValueConverter() {}
    ~GSheetValueConverter() {}

    template <typename T>
    struct v_sring
    {
        static bool const value = std::is_same<T, const char *>::value || std::is_same<T, std::string>::value || std::is_same<T, String>::value;
    };

    template <typename T>
    struct v_number
    {
        static bool const value = std::is_same<T, uint64_t>::value || std::is_same<T, int64_t>::value || std::is_same<T, uint32_t>::value || std::is_same<T, int32_t>::value ||
                                  std::is_same<T, uint16_t>::value || std::is_same<T, int16_t>::value || std::is_same<T, uint8_t>::value || std::is_same<T, int8_t>::value ||
                                  std::is_same<T, double>::value || std::is_same<T, float>::value || std::is_same<T, int>::value;
    };

    template <typename T = gsheet_object_t>
    auto getVal(String &buf, T value) -> typename std::enable_if<std::is_same<T, gsheet_object_t>::value || std::is_same<T, gsheet_string_t>::value || std::is_same<T, gsheet_boolean_t>::value || std::is_same<T, gsheet_number_t>::value, void>::type
    {
        buf = value.c_str();
    }

    template <typename T = const char *>
    auto getVal(String &buf, T value) -> typename std::enable_if<(v_number<T>::value || v_sring<T>::value || std::is_same<T, bool>::value) && !std::is_same<T, gsheet_object_t>::value && !std::is_same<T, gsheet_string_t>::value && !std::is_same<T, gsheet_boolean_t>::value && !std::is_same<T, gsheet_number_t>::value, void>::type
    {
        buf.remove(0, buf.length());
        if (std::is_same<T, bool>::value)
        {
            buf = value ? "true" : "false";
        }
        else
        {
            if (v_sring<T>::value)
                buf += '\"';
            buf += value;
            if (v_sring<T>::value)
                buf += '\"';
        }
    }

    template <typename T>
    auto to(const char *payload) -> typename std::enable_if<v_number<T>::value || std::is_same<T, bool>::value, T>::type
    {
        if (!useLength && strlen(payload) > 0)
        {
            setInt(payload);
            setFloat(payload);
        }
        else
            setBool(strlen(payload));

        if (std::is_same<T, int>::value)
            return iVal.int32;
        else if (std::is_same<T, bool>::value)
            return iVal.int32 > 0;
        else if (std::is_same<T, int8_t>::value)
            return iVal.int8;
        else if (std::is_same<T, uint8_t>::value)
            return iVal.uint8;
        else if (std::is_same<T, int16_t>::value)
            return iVal.int16;
        else if (std::is_same<T, uint16_t>::value)
            return iVal.uint16;
        else if (std::is_same<T, int32_t>::value)
            return iVal.int32;
        else if (std::is_same<T, uint32_t>::value)
            return iVal.uint32;
        else if (std::is_same<T, int64_t>::value)
            return iVal.int64;
        else if (std::is_same<T, uint64_t>::value)
            return iVal.uint64;
        else if (std::is_same<T, float>::value)
            return fVal.f;
        else if (std::is_same<T, double>::value)
            return fVal.d;
        else
            return 0;
    }

    template <typename T>
    auto to(const char *payload) -> typename std::enable_if<v_sring<T>::value, T>::type
    {
        if (payload && payload[0] == '"' && payload[strlen(payload) - 1] == '"')
        {
            buf = payload + 1;
            buf[buf.length() - 1] = 0;
        }
        else
            buf = payload;

        return buf.c_str();
    }

private:
    union IVal
    {
        uint64_t uint64;
        int64_t int64;
        uint32_t uint32;
        int32_t int32;
        int16_t int16;
        uint16_t uint16;
        int8_t int8;
        uint8_t uint8;
    };

    struct FVal
    {
        double d = 0;
        float f = 0;
        void setd(double v)
        {
            d = v;
            f = static_cast<float>(v);
        }

        void setf(float v)
        {
            f = v;
            d = static_cast<double>(v);
        }
    };

    String buf;
    bool trim = false;
    bool useLength = false;

    IVal iVal = {0};
    FVal fVal;

    void setBool(bool value)
    {
        if (value)
        {
            iVal = {1};
            fVal.setd(1);
        }
        else
        {
            iVal = {0};
            fVal.setd(0);
        }
    }

    void setInt(const char *value)
    {
        if (strlen(value) > 0)
        {
            char *pEnd;
            value[0] == '-' ? iVal.int64 = strtoll(value, &pEnd, 10) : iVal.uint64 = strtoull(value, &pEnd, 10);
        }
        else
            iVal = {0};
    }

    void setFloat(const char *value)
    {
        if (strlen(value) > 0)
        {
            char *pEnd;
            fVal.setd(strtod(value, &pEnd));
        }
        else
            fVal.setd(0);
    }
};

#endif