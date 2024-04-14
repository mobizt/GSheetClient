#ifndef GSHEET_CONFIG_H
#define GSHEET_CONFIG_H

/** 📌 Predefined Build Options
 * ⛔ Use following build flag to disable all predefined options.
 * -D GSHEET_DISABLE_ALL_OPTIONS
 */

/**📍 For enabling filesystem
 * ⛔ Use following build flag to disable.
 * -D GSHEET_DISABLE_FS
 */
#define GSHEET_ENABLE_FS

/**📍 For enabling authentication and token
 * ⛔ Use following build flag to disable.
 * -D GSHEET_DISABLE_SERVICE_AUTH
 * -D GSHEET_DISABLE_ACCESS_TOKEN
 */
#define GSHEET_ENABLE_SERVICE_AUTH
#define GSHEET_ENABLE_ACCESS_TOKEN

#define GSHEET_ENABLE_ETHERNET_NETWORK
#define GSHEET_ENABLE_GSM_NETWORK

/** 🔖 Optional Build Options
 *
 * 🏷️ For external Ethernet module support.
 * - Should define both library name and class object name.
 * - GSHEET_ETHERNET_MODULE_LIB is the Ethernet library name with extension (.h) and
 *   should be inside "" or <> e.g. "Ethernet.h".
 * - GSHEET_ETHERNET_MODULE_CLASS is the name of static object defined from class e.g. Ethernet.
 * - GSHEET_ETHERNET_MODULE_TIMEOUT is the time out in milliseconds to wait network connection.
 *
 * #define GSHEET_ETHERNET_MODULE_LIB "EthernetLib.h"
 * #define GSHEET_ETHERNET_MODULE_CLASS EthernetClass
 * #define GSHEET_ETHERNET_MODULE_TIMEOUT 2000
 *
 * 🏷️ For native core library ENC28J60 Ethernet module support in ESP8266
 * #define GSHEET_ENABLE_ESP8266_ENC28J60_ETH
 *
 * 🏷️ For native core library W5500 Ethernet module support in ESP8266
 * #define GSHEET_ENABLE_ESP8266_W5500_ETH
 *
 * 🏷️ For native core library W5100 Ethernet module support in ESP8266
 * #define GSHEET_ENABLE_ESP8266_W5100_ETH
 *
 * 🏷️ For disabling on-board WiFI functionality in case external Client usage
 * #define GSHEET_DISABLE_ONBOARD_WIFI
 *
 * 🏷️ For disabling native (sdk) Ethernet functionality in case external Client usage
 * #define GSHEET_DISABLE_NATIVE_ETHERNET
 *
 * 🏷️ For Async TCP Client usage.
 * #define GSHEET_ENABLE_ASYNC_TCP_CLIENT
 *
 * 🏷️ For maximum async queue limit setting for an async client
 * #define GSHEET_ASYNC_QUEUE_LIMIT 10
 *
 * 🏷️ For GSheet.printf debug port
 * #define GSHEET_PRINTF_PORT Serial
 */

#if __has_include("UserConfig.h")
#include "UserConfig.h"
#endif

#include "core/Options.h"

#endif