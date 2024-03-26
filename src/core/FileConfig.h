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
#ifndef GSHEET_CORE_FILE_CONFIG_H
#define GSHEET_CORE_FILE_CONFIG_H

#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/Memory.h"

#if defined(GSHEET_ENABLE_FS)
#include <FS.h>
#define FILEOBJ File
#else

#endif

#define GSHEET_CHUNK_SIZE 2048

enum gsheet_file_operating_mode
{
    gsheet_file_mode_open_read,
    gsheet_file_mode_open_write,
    gsheet_file_mode_open_append,
    gsheet_file_mode_remove
};

struct gsheet_blob_writer
{
public:
    gsheet_blob_writer() {}
    void init(uint8_t *data, size_t size)
    {
        this->data = data;
        this->size = size;
        index = 0;
    }

    size_t curIndex() const { return index; }

    size_t write(const uint8_t *buf, size_t size)
    {
        uint32_t towrite = size;
        if (index + towrite > this->size)
            towrite = this->size - index;
        if (towrite > 0)
        {
            memcpy(data + index, buf, towrite);
            index += towrite;
        }
        return towrite;
    }

private:
    size_t size = 0;
    uint8_t *data = nullptr;
    uint32_t index = 0;
};

#if defined(GSHEET_ENABLE_FS)
typedef void (*GSheetFileConfigCallback)(FILEOBJ &file, const char *filename, gsheet_file_operating_mode mode);
#endif

struct gsheet_file_config_data
{
    enum gsheet_file_operating_status
    {
        gsheet_file_status_closed,
        gsheet_file_status_opened
    };
#if defined(GSHEET_ENABLE_FS)
    FILEOBJ file;
#endif
    String filename;
    size_t file_size = 0;

#if defined(GSHEET_ENABLE_FS)
    GSheetFileConfigCallback cb = NULL;
#endif
    gsheet_file_operating_status file_status = gsheet_file_status_closed;
    uint8_t *data = nullptr;
    size_t data_pos = 0;
    size_t data_size = 0;
    bool internal_data = false;
    gsheet_blob_writer outB;
    bool initialized = false;

    void clearInternalData()
    {
        if (internal_data && data && data_size > 0)
        {
            delete data;
            data = nullptr;
        }
        data_size = 0;
        internal_data = false;
    }

    void initBlobWriter(size_t size)
    {
        clearInternalData();
        data = new uint8_t[size];
        if (data)
        {
            data_size = size;
            internal_data = true;
            outB.init(data, size);
        }
    }

public:
    gsheet_file_config_data &operator=(gsheet_file_config_data rhs)
    {
        copy(rhs);
        return *this;
    }

    void copy(gsheet_file_config_data rhs)
    {
#if defined(GSHEET_ENABLE_FS)
        this->file = rhs.file;
#endif
        this->filename = rhs.filename;
#if defined(GSHEET_ENABLE_FS)
        this->cb = rhs.cb;
#endif
        if (!rhs.internal_data && rhs.data && rhs.data_size > 0)
        {
            clearInternalData();
            this->data = rhs.data;
            this->data_size = rhs.data_size;
            this->internal_data = false;
        }

        this->initialized = rhs.initialized;
    }

    void clear()
    {
        clearInternalData();
        filename.remove(0, filename.length());
#if defined(ENABLE_FS)
        cb = NULL;
#endif
        data = nullptr;
        data_size = 0;
        internal_data = false;
        initialized = false;
    }
};

#if defined(GSHEET_ENABLE_FS)

class GSheetFileConfig
{

public:
    GSheetFileConfig(const String &filename, GSheetFileConfigCallback cb)
    {
        clear();
        setFilename(filename);
        setCallback(cb);
        data.initialized = true;
    }
    ~GSheetFileConfig() {}
    void clear() { data.clear(); }

    GSheetFileConfig &setFilename(const String &filename)
    {
        if (filename.length() > 0 && filename[0] != '/')
            data.filename += '/';
        data.filename += filename;
        return *this;
    }

    GSheetFileConfig &setCallback(GSheetFileConfigCallback cb)
    {
        data.cb = cb;
        return *this;
    }

    gsheet_file_config_data &get() { return data; }

private:
    gsheet_file_config_data data;
};

#endif

class GSheetDefaultFileConfig
{

public:
    template <typename T = const char *>
    GSheetDefaultFileConfig() {}
    ~GSheetDefaultFileConfig() {}
    gsheet_file_config_data &get() { return data; }

private:
    gsheet_file_config_data data;
};

class GSheetBlobConfig
{

public:
    GSheetBlobConfig(uint8_t *data = nullptr, size_t size = 0)
    {
        clear();
        if (data && size > 0)
        {
            this->data.data = data;
            this->data.data_size = size;
            this->data.internal_data = false;
        }

        this->data.initialized = true;
    }
    ~GSheetBlobConfig() {}
    void clear() { data.clear(); }

    uint8_t *blob() const { return data.data; }
    size_t size() const { return data.data_size; }

    gsheet_file_config_data &getData() { return data; }

private:
    gsheet_file_config_data data;
};

namespace GSHEET
{

    template <typename T>
    static gsheet_file_config_data &getFile(T &file) { return file.get(); }

    template <typename T>
    static gsheet_file_config_data &getBlob(T &blob) { return blob.getData(); }

}

#endif