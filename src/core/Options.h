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
#ifndef GSHEET_BUILD_OPTIONS_H
#define GSHEET_BUILD_OPTIONS_H

#define GSHEET_DEFAULT_TS 1618971013

#if defined(ARDUINO_ARCH_RP2040)

#if defined(ARDUINO_NANO_RP2040_CONNECT)
#ifndef ARDUINO_NANO_RP2040_CONNECT_MODULE
#define ARDUINO_NANO_RP2040_CONNECT_MODULE
#endif
#else
#ifndef ARDUINO_PICO_MODULE
#define ARDUINO_PICO_MODULE
#endif
#endif

#endif

#if defined(GSHEET_DISABLE_ALL_OPTIONS)

#undef GSHEET_ENABLE_PSRAM
#undef GSHEET_ENABLE_FS
#undef GSHEET_ENABLE_SERVICE_AUTH
#undef GSHEET_ENABLE_ACCESS_TOKEN
#undef GSHEET_ENABLE_JWT

#endif


#if defined(GSHEET_DISABLE_PSRAM)
#undef GSHEET_ENABLE_PSRAM
#endif


#if defined(GSHEET_DISABLE_FS)
#undef GSHEET_ENABLE_FS
#endif

#if defined(GSHEET_DISABLE_SERVICE_AUTH)
#undef GSHEET_ENABLE_SERVICE_AUTH
#endif


#if defined(GSHEET_DISABLE_ACCESS_TOKEN)
#undef GSHEET_ENABLE_ACCESS_TOKEN
#endif


#if defined(GSHEET_ENABLE_PSRAM)
#define GSHEET_USE_PSRAM
#endif

#if !defined(FPSTR)
#define FPSTR
#endif


/// CORE_ARDUINO_XXXX macro for MCU build target.

#if defined(ESP8266) || defined(ESP32)
#ifndef CORE_ARDUINO_ESP
#define CORE_ARDUINO_ESP
#endif
#endif

#if defined(__arm__)
#ifndef CORE_ARDUINO_ARM
#define CORE_ARDUINO_ARM
#endif
#endif

#if defined(ARDUINO_ARCH_SAMD)
#ifndef CORE_ARDUINO_ARCH_SAMD
#define CORE_ARDUINO_ARCH_SAMD
#endif
#endif

#if defined(ARDUINO_ARCH_RP2040)

#if defined(ARDUINO_NANO_RP2040_CONNECT)
#ifndef CORE_ARDUINO_NANO_RP2040_CONNECT
#define CORE_ARDUINO_NANO_RP2040_CONNECT
#endif
#else
#ifndef CORE_ARDUINO_PICO
#define CORE_ARDUINO_PICO
#endif
#endif

#endif

#if defined(TEENSYDUINO)
#ifndef CORE_ARDUINO_TEENSY
#define CORE_ARDUINO_TEENSY
#endif
#endif

#if defined(GSHEET_ENABLE_FS)
#if __has_include(<FS.h>)
#include <FS.h>
#define GSHEET_FILEOBJ File
#elif __has_include(<SD.h>) && __has_include(<SPI.h>)
#include <SPI.h>
#include <SD.h>
#define GSHEET_FILEOBJ File
#else
#undef GSHEET_ENABLE_FS
#endif
#endif

#if defined(GSHEET_ENABLE_FS)

#if (defined(ESP8266) || defined(CORE_ARDUINO_PICO)) || (defined(ESP32) && __has_include(<SPIFFS.h>))
#ifndef GSHEET_FLASH_SUPPORTS
#define GSHEET_FLASH_SUPPORTS
#endif

#if !defined(FILE_OPEN_MODE_READ)
#define FILE_OPEN_MODE_READ "r"
#endif

#if !defined(FILE_OPEN_MODE_WRITE)
#define FILE_OPEN_MODE_WRITE "w"
#endif

#if !defined(FILE_OPEN_MODE_APPEND)
#define FILE_OPEN_MODE_APPEND "a"
#endif

#elif __has_include(<SD.h>) && __has_include(<SPI.h>)

#if !defined(FILE_OPEN_MODE_READ)
#define FILE_OPEN_MODE_READ FILE_READ
#endif

#if !defined(FILE_OPEN_MODE_WRITE)
#define FILE_OPEN_MODE_WRITE FILE_WRITE
#endif

#if !defined(FILE_OPEN_MODE_APPEND)
#define FILE_OPEN_MODE_APPEND FILE_WRITE
#endif

#endif

#endif

#if __has_include(<time.h>)
#include <time.h>
#endif


#if defined(ESP32)
#if defined(ESP_ARDUINO_VERSION)
#if ESP_ARDUINO_VERSION > ESP_ARDUINO_VERSION_VAL(2, 0, 1)
#define ESP32_GT_2_0_1_FS_MEMORY_FIX
#endif
#endif
#endif

#if defined(GSHEET_ENABLE_SERVICE_AUTH) 
#define GSHEET_ENABLE_JWT
#endif

#if defined(GSHEET_ENABLE_JWT)

#if !defined(GSHEET_ENABLE_SERVICE_AUTH)
#define GSHEET_ENABLE_SERVICE_AUTH
#endif

#endif

#if !defined(GSHEET_ASYNC_CLIENT)
#define GSHEET_ASYNC_CLIENT AsyncClient
#endif

#endif