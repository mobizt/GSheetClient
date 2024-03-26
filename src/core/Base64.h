/**
 * Created March 26, 2024
 * 
 * For MCU build target (CORE_ARDUINO_XXXX), see Options.h.
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
#ifndef GSHEET_CORE_BASE64_H
#define GSHEET_CORE_BASE64_H

#include <Arduino.h>
#include <Client.h>
#include "./GSheetConfig.h"
#include "./core/Memory.h"
#include "./core/FileConfig.h"

#if __has_include(<vector>)
#include <vector>
#endif

static const char gsheet_boundary_table[] PROGMEM = "=_abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const unsigned char gsheet_base64_table[65] PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

template <typename T>
struct gsheet_base64_io_t
{
    // the total bytes of data in output buffer
    int bufWrite = 0;
    // the size of output buffer
    size_t bufLen = 1024;
// for file to write
#if defined(GSHEET_ENABLE_FS)
    GSHEET_FILEOBJ file;
#endif
    // for T array
    T *outT = nullptr;
    // for T vector
    std::vector<T> *outL = nullptr;
    // for client
    Client *outC = nullptr;
    // for blob
    gsheet_blob_writer *outB = nullptr;
    // for ota
    bool ota = false;
};

class GSheetBase64Util
{
public:
    int getBase64Len(int n)
    {
        int len = (4 * ceil(n / 3.0));
        return len;
    }

    int getBase64Padding(int n)
    {
        int pLen = getBase64Len(n);
        int uLen = ceil(4.0 * n / 3.0);
        return pLen - uLen;
    }

    size_t encodedLength(size_t len) const
    {
        return ((len + 2) / 3 * 4) + 1;
    }

    int decodedLen(const char *src)
    {
        int len = strlen(src), i = len - 1, pad = 0;
        if (len < 4)
            return 0;
        while (i > 0 && src[i--] == '=')
        {
            pad++;
        }
        return (3 * (len / 4)) - pad;
    }

    unsigned char *creatBase64EncBuffer(GSheetMemory &mem, bool isURL)
    {

        unsigned char *base64EncBuf = reinterpret_cast<unsigned char *>(mem.alloc(65));
        strcpy_P((char *)base64EncBuf, (char *)gsheet_base64_table);
        if (isURL)
        {
            base64EncBuf[62] = '-';
            base64EncBuf[63] = '_';
        }
        return base64EncBuf;
    }

    bool updateWrite(uint8_t *data, size_t len)
    {
#if defined(GSHEET_OTA_UPDATE_ENABLED)
#if defined(ESP32) || defined(ESP8266) || defined(CORE_ARDUINO_PICO)
        return Update.write(data, len) == len;
#endif
#endif
        return false;
    }

    unsigned char *creatBase64DecBuffer(GSheetMemory &mem)
    {

        unsigned char *base64DecBuf = reinterpret_cast<unsigned char *>(mem.alloc(256, false));
        memset(base64DecBuf, 0x80, 256);
        for (size_t i = 0; i < sizeof(gsheet_base64_table) - 1; i++)
            base64DecBuf[gsheet_base64_table[i]] = (unsigned char)i;
        base64DecBuf['='] = 0;
        return base64DecBuf;
    }

    template <typename T = uint8_t>
    bool writeOutput(gsheet_base64_io_t<T> &out)
    {
        size_t write = out.bufWrite;
        out.bufWrite = 0;

        if (write == 0)
            return true;

        if (out.outC && out.outC->write((uint8_t *)out.outT, write) == write)
            return true;
#if defined(GSHEET_ENABLE_FS)
        else if (out.file && out.file.write((uint8_t *)out.outT, write) == write)
            return true;
#endif
        else if (out.outB && out.outB->write((uint8_t *)out.outT, write) == write)
            return true;

#if defined(GSHEET_OTA_UPDATE_ENABLED)
        else if (out.ota && updateWrite((uint8_t *)out.outT, write))
            return true;
#endif
        return false;
    }

    template <typename T = uint8_t>
    bool setOutput(uint8_t val, gsheet_base64_io_t<T> &out, T **pos)
    {
        if (out.outT)
        {
#if defined(GSHEET_ENABLE_FS)
            if (out.ota || out.outC || out.file || out.outB)
#else
            if (out.ota || out.outC || out.outB)
#endif
            {
                out.outT[out.bufWrite++] = val;
                if (out.bufWrite == (int)out.bufLen && !writeOutput(out))
                    return false;
            }
            else
                *(*pos)++ = (T)(val);
        }
        else if (out.outL)
            out.outL->push_back(val);

        return true;
    }

    template <typename T>
    bool decode(GSheetMemory &mem, unsigned char *base64DecBuf, const char *src, size_t len, gsheet_base64_io_t<T> &out)
    {
        // the maximum chunk size that writes to output is limited by out.bufLen, the minimum is depending on the source length
        bool ret = false;
        unsigned char *block = reinterpret_cast<unsigned char *>(mem.alloc(4, false));
        unsigned char temp;
        size_t i, count;
        int pad = 0;
        size_t extra_pad;
        T *pos = out.outT ? (T *)&out.outT[0] : nullptr;
        if (len == 0)
            len = strlen(src);

        count = 0;

        for (i = 0; i < len; i++)
        {
            if ((uint8_t)base64DecBuf[(uint8_t)src[i]] != 0x80)
                count++;
        }

        if (count == 0)
            goto skip;

        extra_pad = (4 - count % 4) % 4;
        count = 0;
        for (i = 0; i < len + extra_pad; i++)
        {
            unsigned char val;

            if (i >= len)
                val = '=';
            else
                val = src[i];

            temp = base64DecBuf[val];

            if (temp == 0x80)
                continue;

            if (val == '=')
                pad++;

            block[count] = temp;
            count++;
            if (count == 4)
            {

                setOutput((block[0] << 2) | (block[1] >> 4), out, &pos);

                count = 0;
                if (pad)
                {
                    if (pad == 1)
                        setOutput((block[1] << 4) | (block[2] >> 2), out, &pos);
                    else if (pad > 2)
                        goto skip;

                    break;
                }
                else
                {
                    setOutput((block[1] << 4) | (block[2] >> 2), out, &pos);
                    setOutput((block[2] << 6) | block[3], out, &pos);
                }
            }
        }

        // write remaining
        if (out.bufWrite > 0 && !writeOutput(out))
            goto skip;

        ret = true;

    skip:
        mem.release(&block);
        return ret;
    }

    template <typename T>
    bool encodeLast(unsigned char *base64EncBuf, const unsigned char *in, size_t len, gsheet_base64_io_t<T> &out, T **pos)
    {
        if (len > 2)
            return false;

        if (!setOutput(base64EncBuf[in[0] >> 2], out, pos))
            return false;

        if (len == 1)
        {
            if (!setOutput(base64EncBuf[(in[0] & 0x03) << 4], out, pos))
                return false;
            if (!setOutput('=', out, pos))
                return false;
        }
        else
        {
            if (!setOutput(base64EncBuf[((in[0] & 0x03) << 4) | (in[1] >> 4)], out, pos))
                return false;
            if (!setOutput(base64EncBuf[(in[1] & 0x0f) << 2], out, pos))
                return false;
        }

        if (!setOutput('=', out, pos))
            return false;

        return true;
    }

    template <typename T>
    bool encode(unsigned char *base64EncBuf, uint8_t *src, size_t len, gsheet_base64_io_t<T> &out, bool writeAllRemaining = true)
    {
        const unsigned char *end, *in;

        T *pos = out.outT ? (T *)&out.outT[0] : nullptr;
        in = src;
        end = src + len;

        while (end - in >= 3)
        {
            if (!setOutput(base64EncBuf[in[0] >> 2], out, &pos))
                return false;
            if (!setOutput(base64EncBuf[((in[0] & 0x03) << 4) | (in[1] >> 4)], out, &pos))
                return false;
            if (!setOutput(base64EncBuf[((in[1] & 0x0f) << 2) | (in[2] >> 6)], out, &pos))
                return false;
            if (!setOutput(base64EncBuf[in[2] & 0x3f], out, &pos))
                return false;
            in += 3;
        }

        if (end - in && !encodeLast(base64EncBuf, in, end - in, out, &pos))
            return false;

        if (writeAllRemaining && out.bufWrite > 0 && !writeOutput(out))
            return false;

        return true;
    }

#if defined(GSHEET_ENABLE_FS)
    bool decodeToFile(GSheetMemory &mem, GSHEET_FILEOBJ file, const char *src)
    {
        gsheet_base64_io_t<uint8_t> out;
        out.file = file;
        uint8_t *buf = reinterpret_cast<uint8_t *>(mem.alloc(out.bufLen));
        out.outT = buf;
        unsigned char *base64DecBuf = creatBase64DecBuffer(mem);
        bool ret = decode<uint8_t>(mem, base64DecBuf, src, strlen(src), out);
        mem.release(&buf);
        mem.release(&base64DecBuf);
        return ret;
    }
#endif
    bool decodeToBlob(GSheetMemory &mem, gsheet_blob_writer *bWriter, const char *src)
    {
        gsheet_base64_io_t<uint8_t> out;
        out.outB = bWriter;
        uint8_t *buf = reinterpret_cast<uint8_t *>(mem.alloc(out.bufLen));
        out.outT = buf;
        unsigned char *base64DecBuf = creatBase64DecBuffer(mem);
        bool ret = decode<uint8_t>(mem, base64DecBuf, src, strlen(src), out);
        mem.release(&buf);
        mem.release(&base64DecBuf);
        return ret;
    }

    void encodeUrl(GSheetMemory &mem, char *encoded, unsigned char *string, size_t len)
    {
        size_t i;
        char *p = encoded;
        unsigned char *base64EncBuf = creatBase64EncBuffer(mem, true);

        for (i = 0; i < len - 2; i += 3)
        {
            *p++ = base64EncBuf[(string[i] >> 2) & 0x3F];
            *p++ = base64EncBuf[((string[i] & 0x3) << 4) | ((int)(string[i + 1] & 0xF0) >> 4)];
            *p++ = base64EncBuf[((string[i + 1] & 0xF) << 2) | ((int)(string[i + 2] & 0xC0) >> 6)];
            *p++ = base64EncBuf[string[i + 2] & 0x3F];
        }

        if (i < len)
        {
            *p++ = base64EncBuf[(string[i] >> 2) & 0x3F];
            if (i == (len - 1))
                *p++ = base64EncBuf[((string[i] & 0x3) << 4)];
            else
            {
                *p++ = base64EncBuf[((string[i] & 0x3) << 4) | ((int)(string[i + 1] & 0xF0) >> 4)];
                *p++ = base64EncBuf[((string[i + 1] & 0xF) << 2)];
            }
        }

        *p++ = '\0';
        mem.release(&base64EncBuf);
    }

    char *encodeToChars(GSheetMemory &mem, uint8_t *src, size_t len)
    {
        char *encoded = reinterpret_cast<char *>(mem.alloc(encodedLength(len) + 1));
        gsheet_base64_io_t<char> out;
        out.outT = encoded;
        unsigned char *base64EncBuf = creatBase64EncBuffer(mem, false);
        encode<char>(base64EncBuf, (uint8_t *)src, len, out);
        mem.release(&base64EncBuf);
        return encoded;
    }
};

#endif