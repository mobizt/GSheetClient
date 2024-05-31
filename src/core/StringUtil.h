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
#ifndef GSHEET_CORE_STRING_H
#define GSHEET_CORE_STRING_H

#include <Arduino.h>
#include <Client.h>
#include "./GSheetConfig.h"

#if defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_MINIMA) || defined(ARDUINO_PORTENTA_C33)
#define GSHEET_STRSEP strsepImpl
#define GSHEET_USE_STRSEP_IMPL
#else
#define GSHEET_STRSEP strsep
#endif

class GSheetStringUtil
{

public:
    GSheetStringUtil() {}
    ~GSheetStringUtil() {}

#if defined(GSHEET_USE_STRSEP_IMPL)
    char *strsepImpl(char **stringp, const char *delim)
    {
        char *rv = *stringp;
        if (rv)
        {
            *stringp += strcspn(*stringp, delim);
            if (**stringp)
                *(*stringp)++ = '\0';
            else
                *stringp = 0;
        }
        return rv;
    }
#else
    char *strsepImpl(char **stringp, const char *delim) { return strsep(stringp, delim); }
#endif

    void parse(const String &str, const String &name, const String &delim, int &p1, int &p2)
    {
        p1 = str.indexOf(name, p1);
        if (p1 > -1)
        {
            p2 = str.indexOf(':', p1);
            if (p1 > -1 && str.substring(p1, p2) == name)
            {
                p1 = p2 + 1;
                while (str[p1] == ' ')
                {
                    p1++;
                }

                p2 = str.indexOf(delim, p2);
                if (p2 == -1)
                    p2 = str.length() - 1;

                return;
            }
        }
        p1 = -1;
        p2 = -1;
    }

    void trim(String &str)
    {
        int i = str.length() - 1;
        while (i >= 0 && (str[i] == ' ' || str[i] == '}' || str[i] == '\r' || str[i] == '\n'))
        {
            i--;
        }
        str.remove(i);
    }

    void addSp(String &buf) { buf += ' '; }

    String u64Str(uint64_t val)
    {
        // Some cores do not provide 64-bit integer to string conversion.
        String v;
        char buffer[21];
        char *ndx = &buffer[sizeof(buffer) - 1];
        *ndx = '\0';
        do
        {
            *--ndx = val % 10 + '0';
            val = val / 10;
        } while (val != 0);
        v = ndx;
        return v;
    }
};

#endif