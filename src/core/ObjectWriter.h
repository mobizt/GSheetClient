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
#ifndef GSHEET_OBJECT_WRITER_H
#define GSHEET_OBJECT_WRITER_H

#include <Arduino.h>
#include "./GSheetConfig.h"

#include "./core/JSON.h"

#define GSHEET_RESOURCE_PATH_BASE FPSTR("<resource_path>")

class GSheetObjectWriter
{
private:
    GSheetJSONUtil jut;

public:
    void addMember(String &buf, const String &v, bool isString, const String &token = "}}")
    {
        int p = buf.lastIndexOf(token);
        String str = buf.substring(0, p);
        str += ',';
        // Add to object
        if (token[0] == '}')
        {
            String tmp;
            if (isString)
                tmp += '"';
            tmp += v;
            if (isString)
                tmp += '"';
            str += tmp.substring(1, tmp.length() - 1);
        }
        // Add to array
        else
        {
            if (isString)
                str += '"';
            str += v;
            if (isString)
                str += '"';
        }
        str += token;
        buf = str;
    }

    void addObject(String &buf, const String &object, const String &token, bool clear = false)
    {
        if (clear)
            buf.remove(0, buf.length());
        if (object.length() > 0)
        {
            if (buf.length() == 0)
                buf = object;
            else
                addMember(buf, object, false, token);
        }
    }

    void addMapArrayMember(String *buf, size_t size, uint8_t index, const String &key, const String &memberValue, bool isString)
    {
        if (index < size)
        {
            if (buf[index].length() == 0)
            {
                String temp;
                jut.addArray(temp, memberValue, isString, true);
                jut.addObject(buf[index], key, temp, false, true);
            }
            else
                addMember(buf[index], memberValue, isString, "]}");

            getBuf(buf, size);
        }
    }

    void getBuf(String *buf, size_t size)
    {
        clear(buf[0]);
        for (size_t i = 1; i < size; i++)
            addObject(buf[0], buf[i], "}", i == 0);
    }

    void setObject(String *buf, size_t size, uint8_t index, const String &key, const String &value, bool isString, bool last)
    {
        if (index < size)
        {
            if (key.length())
            {
                clear(buf[index]);
                jut.addObject(buf[index], key, value, isString, last);
            }
            getBuf(buf, size);
        }
    }

    void clearBuf(String *buf, size_t size)
    {
        for (size_t i = 0; i < size; i++)
            clear(buf[i]);
    }

    void clear(String &buf) { buf.remove(0, buf.length()); }

    const char *setPair(String &buf, const String &key, const String &value, bool isArrayValue = false)
    {
        buf.remove(0, buf.length());
        jut.addObject(buf, key, isArrayValue ? getArrayStr(value) : value, false, true);
        return buf.c_str();
    }
    void setBool(String &buf, bool value) { buf = getBoolStr(value); }

    String getBoolStr(bool value) { return value ? FPSTR("true") : FPSTR("false"); }

    String getArrayStr(const String &value)
    {
        String str = FPSTR("[");
        str += value;
        str += ']';
        return str;
    }

    void setString(String &buf, const String &value)
    {
        buf = FPSTR("\"");
        buf += value;
        buf += '"';
    }

    String makeResourcePath(const String &path, bool toString = false)
    {
        String full_path;
        if (toString)
            full_path += '"';
        full_path += GSHEET_RESOURCE_PATH_BASE;
        if (path.length())
        {
            if (path.length() && path[0] != '/')
                full_path += '/';
            full_path += path;
        }
        if (toString)
            full_path += '"';
        return full_path;
    }
};

class GSheetBufWriter
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

    void setObject(String *buf, size_t bufSize, uint8_t index, const String &key, const String &value, bool isString, bool last)
    {
        owriter.setObject(buf, bufSize, index, key, value, isString, last);
    }

public:
    GSheetBufWriter() = default;
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
    T1 set(T1 ret, bool value, String *buf, size_t bufSize, uint8_t index, const String &name)
    {
        setObject(buf, bufSize, index, name, owriter.getBoolStr(value), false, true);
        return ret;
    }

    template <typename T1, typename T2>
    auto set(T1 ret, const T2 &value, String *buf, size_t bufSize, uint8_t index, const String &name) -> typename std::enable_if<v_number<T2>::value, T1>::type
    {
        setObject(buf, bufSize, index, name, String(value), false, true);
        return ret;
    }

    template <typename T1, typename T2>
    auto set(T1 ret, const T2 &value, String *buf, size_t bufSize, uint8_t index, const String &name) -> typename std::enable_if<v_sring<T2>::value, T1>::type
    {
        setObject(buf, bufSize, index, name, value, true, true);
        return ret;
    }

    template <typename T1, typename T2>
    auto set(T1 ret, const T2 &value, String *buf, size_t bufSize, uint8_t index, const String &name) -> typename std::enable_if<(!v_sring<T2>::value && !v_number<T2>::value && !std::is_same<T2, bool>::value), T1>::type
    {
        setObject(buf, bufSize, index, name, value.c_str(), false, true);
        return ret;
    }

    template <typename T1, typename T2>
    T1 append(T1 ret, bool value, String *buf, size_t bufSize, uint8_t index, const String &name)
    {
        owriter.addMapArrayMember(buf, bufSize, index, name, owriter.getBoolStr(value), false);
        return ret;
    }

    template <typename T1, typename T2>
    auto append(T1 ret, const T2 &value, String *buf, size_t bufSize, uint8_t index, const String &name) -> typename std::enable_if<v_number<T2>::value, T1>::type
    {
        owriter.addMapArrayMember(buf, bufSize, index, name, String(value), false);
        return ret;
    }

    template <typename T1, typename T2>
    auto append(T1 ret, const T2 &value, String *buf, size_t bufSize, uint8_t index, const String &name) -> typename std::enable_if<v_sring<T2>::value, T1>::type
    {
        owriter.addMapArrayMember(buf, bufSize, index, name, value, true);
        return ret;
    }

    template <typename T1, typename T2>
    auto append(T1 ret, const T2 &value, String *buf, size_t bufSize, uint8_t index, const String &name) -> typename std::enable_if<(!v_sring<T2>::value && !v_number<T2>::value && !std::is_same<T2, bool>::value), T1>::type
    {
        owriter.addMapArrayMember(buf, bufSize, index, name, value.c_str(), false);
        return ret;
    }
    void clear(String &buf) { buf.remove(0, buf.length()); }
    void clear(String *buf, size_t bufSize) { owriter.clearBuf(buf, bufSize); }
};

class BaseG1 : public Printable
{

protected:
    String buf;
    GSheetBufWriter wr;

public:
    BaseG1() = default;
    const char *c_str() const { return buf.c_str(); }
    size_t printTo(Print &p) const { return p.print(buf.c_str()); }
    void clear() { buf.remove(0, buf.length()); }
};

class BaseG2 : public Printable
{

protected:
    static const size_t bufSize = 2;
    String buf[bufSize];
    GSheetBufWriter wr;

public:
    BaseG2() = default;
    const char *c_str() const { return buf[0].c_str(); }
    size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
    void clear() { wr.clear(buf, bufSize); }
};

class BaseG4 : public Printable
{

protected:
    static const size_t bufSize = 4;
    String buf[bufSize];
    GSheetBufWriter wr;

public:
    BaseG4() = default;
    const char *c_str() const { return buf[0].c_str(); }
    size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
    void clear() { wr.clear(buf, bufSize); }
};

class BaseG6 : public Printable
{

protected:
    static const size_t bufSize = 6;
    String buf[bufSize];
    GSheetBufWriter wr;

public:
    BaseG6() = default;
    const char *c_str() const { return buf[0].c_str(); }
    size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
    void clear() { wr.clear(buf, bufSize); }
};

class BaseG8 : public Printable
{
protected:
    static const size_t bufSize = 8;
    String buf[bufSize];
    GSheetBufWriter wr;

public:
    BaseG8() = default;
    const char *c_str() const { return buf[0].c_str(); }
    size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
    void clear() { wr.clear(buf, bufSize); }
};

class BaseG12 : public Printable
{

protected:
    static const size_t bufSize = 12;
    String buf[bufSize];
    GSheetBufWriter wr;

public:
    BaseG12() = default;
    const char *c_str() const { return buf[0].c_str(); }
    size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
    void clear() { wr.clear(buf, bufSize); }
};

class BaseG16 : public Printable
{
protected:
    static const size_t bufSize = 16;
    String buf[bufSize];
    GSheetBufWriter wr;

public:
    BaseG16() = default;
    const char *c_str() const { return buf[0].c_str(); }
    size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
    void clear() { wr.clear(buf, bufSize); }
};

 class GSheetUnityRange
    {
    public:
        GSheetUnityRange() = default;

        float val(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return value;
        }
    };

#endif