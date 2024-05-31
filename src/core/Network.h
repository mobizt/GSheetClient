/**
 * Created May 30, 2024
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
#ifndef GSHEET_CORE_NETWORK_H
#define GSHEET_CORE_NETWORK_H

#include <Arduino.h>
#include "./Config.h"
#include <vector>

typedef void (*GSheetNetworkConnectionCallback)(void);
typedef void (*GSheetNetworkStatusCallback)(bool &);

#if __has_include(<esp_idf_version.h>)
#include <esp_idf_version.h>
#endif

#if __has_include(<IPAddress.h>)
#include <IPAddress.h>
#endif

#if defined(ESP8266) || defined(CORE_ARDUINO_PICO)

#include <string>

//__GNUC__
//__GNUC_MINOR__
//__GNUC_PATCHLEVEL__

#ifdef __GNUC__
#if __GNUC__ > 4 || __GNUC__ == 10
#if defined(ARDUINO_ESP8266_GIT_VER)
#if ARDUINO_ESP8266_GIT_VER > 0
#define ESP8266_CORE_SDK_V3_X_X
#endif
#endif
#endif
#endif

#endif

#if !defined(GSHEET_DISABLE_NATIVE_ETHERNET)

#if defined(ESP32) && __has_include(<ETH.h>)
#include <ETH.h>
#define GSHEET_LWIP_ETH_IS_AVAILABLE
#elif defined(ESP8266) && defined(ESP8266_CORE_SDK_V3_X_X)

#if __has_include(<LwipIntfDev.h>)
#include <LwipIntfDev.h>
#endif

#if __has_include(<ENC28J60lwIP.h>)&& defined(ENABLE_ESP8266_ENC28J60_ETH)
#define INC_ENC28J60_LWIP
#include <ENC28J60lwIP.h>
#endif

#if __has_include(<W5100lwIP.h>) && defined(ENABLE_ESP8266_W5100_ETH)
#define INC_W5100_LWIP
#include <W5100lwIP.h>
#endif

#if __has_include(<W5500lwIP.h>)&& defined(ENABLE_ESP8266_W5500_ETH)
#define INC_W5500_LWIP
#include <W5500lwIP.h>
#endif

#if defined(CORE_ARDUINO_PICO)

#endif

#if defined(INC_ENC28J60_LWIP) || defined(INC_W5100_LWIP) || defined(INC_W5500_LWIP)
#define GSHEET_LWIP_ETH_IS_AVAILABLE
#endif

#endif

#endif

#if __has_include(<Ethernet.h>) ||  (defined(GSHEET_ETHERNET_MODULE_LIB) && defined(GSHEET_ETHERNET_MODULE_CLASS))

#if defined(ESP8266)
#undef MAX_SOCK_NUM
#endif

#if (defined(ESP8266) && defined(GSHEET_DISABLE_NATIVE_ETHERNET)) || (!defined(ESP8266) && !defined(ARDUINO_NANO_RP2040_CONNECT))

#if defined(GSHEET_ETHERNET_MODULE_LIB) && defined(GSHEET_ETHERNET_MODULE_CLASS)
#if __has_include(GSHEET_ETHERNET_MODULE_LIB)
#include GSHEET_ETHERNET_MODULE_LIB
#define GSHEET_ETHERNET_MODULE_CLASS_IMPL GSHEET_ETHERNET_MODULE_CLASS
#elif __has_include(<Ethernet.h>)
#include <Ethernet.h>
#define GSHEET_ETHERNET_MODULE_CLASS_IMPL Ethernet
#endif
#else
#include <Ethernet.h>
#define GSHEET_ETHERNET_MODULE_CLASS_IMPL Ethernet
#endif

#if defined(GSHEET_ETHERNET_MODULE_CLASS_IMPL)

#define GSHEET_ETHERNET_MODULE_IS_AVAILABLE

// milliseconds
#if !defined(GSHEET_ETHERNET_MODULE_TIMEOUT)
#define GSHEET_ETHERNET_MODULE_TIMEOUT 2000
#elif GSHEET_ETHERNET_MODULE_TIMEOUT <= 0 || GSHEET_ETHERNET_MODULE_TIMEOUT > 120 * 1000
#undef GSHEET_ETHERNET_MODULE_TIMEOUT
#define GSHEET_ETHERNET_MODULE_TIMEOUT 2000
#endif

#endif

#endif

#if defined(ESP8266)
#undef MAX_SOCK_NUM
#endif

#endif

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || \
    defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_PORTENTA_C33) ||                \
    defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ||         \
    __has_include(<WiFiNINA.h>) ||__has_include(<WiFi101.h>)

#if !defined(GSHEET_DISABLE_ONBOARD_WIFI)

#define GSHEET_WIFI_IS_AVAILABLE

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>) || defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#elif __has_include(<WiFiC3.h>) || defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif __has_include(<WiFi.h>)
#include <WiFi.h>
#endif

#if defined(ARDUINO_ARCH_SAMD) ||      \
    defined(ARDUINO_UNOWIFIR4) ||      \
    defined(ARDUINO_GIGA) ||           \
    defined(ARDUINO_PORTENTA_C33) ||   \
    defined(ARDUINO_PORTENTA_H7_M7) || \
    defined(ARDUINO_PORTENTA_H7_M4) || \
    defined(ARDUINO_PORTENTA_X8)

#if !defined(GSHEET_HAS_ARDUINO_WIFISSLCLIENT)
#define GSHEET_HAS_ARDUINO_WIFISSLCLIENT
#endif

#endif

#if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
#if !defined(GSHEET_HAS_WIFICLIENTSECURE)
#define GSHEET_HAS_WIFICLIENTSECURE
#endif
#endif

#if !defined(ARDUINO_RASPBERRY_PI_PICO_W) && \
    !defined(CORE_ARDUINO_ARCH_SAMD) &&      \
    !defined(CORE_ARDUINO_MBED_PORTENTA) &&  \
    !defined(ARDUINO_UNOWIFIR4) &&           \
    !defined(ARDUINO_PORTENTA_C33) &&        \
    !defined(ARDUINO_NANO_RP2040_CONNECT) && \
    !defined(ARDUINO_GIGA) &&                \
    !defined(ARDUINO_PORTENTA_H7_M7) &&      \
    !defined(ARDUINO_PORTENTA_H7_M4)
#define GSHEET_HAS_WIFI_DISCONNECT
#endif

#if defined(ARDUINO_PORTENTA_C33)
#define GSHEET_CONST_STRING_CAST char * // C33 WiFi library SSID is char array
#else
#define GSHEET_CONST_STRING_CAST const char *
#endif

// WiFiS3 getTime is currently return 0 (not implemented)
#if __has_include(<WiFiNINA.h>) || __has_include(<WiFi101.h>)
#define GSHEET_HAS_WIFI_TIME
#endif

#if defined(CORE_ARDUINO_PICO) && __has_include(<WiFiMulti.h>)
#include <WiFiMulti.h>
#define GSHEET_HAS_WIFIMULTI
#endif

#if defined(ESP8266) && __has_include(<ESP8266WiFiMulti.h>)
#include <ESP8266WiFiMulti.h>
#define GSHEET_HAS_WIFIMULTI
#endif

#endif

#endif

#if defined(TINY_GSM_MODEM_SIM800) ||     \
    defined(TINY_GSM_MODEM_SIM808) ||     \
    defined(TINY_GSM_MODEM_SIM868) ||     \
    defined(TINY_GSM_MODEM_SIM900) ||     \
    defined(TINY_GSM_MODEM_SIM7000) ||    \
    defined(TINY_GSM_MODEM_SIM7000) ||    \
    defined(TINY_GSM_MODEM_SIM7000SSL) || \
    defined(TINY_GSM_MODEM_SIM7070) ||    \
    defined(TINY_GSM_MODEM_SIM7080) ||    \
    defined(TINY_GSM_MODEM_SIM7090) ||    \
    defined(TINY_GSM_MODEM_SIM5320) ||    \
    defined(TINY_GSM_MODEM_SIM5360) ||    \
    defined(TINY_GSM_MODEM_SIM5300) ||    \
    defined(TINY_GSM_MODEM_SIM7100) ||    \
    defined(TINY_GSM_MODEM_SIM7600) ||    \
    defined(TINY_GSM_MODEM_SIM7800) ||    \
    defined(TINY_GSM_MODEM_SIM7500) ||    \
    defined(TINY_GSM_MODEM_UBLOX) ||      \
    defined(TINY_GSM_MODEM_SARAR4) ||     \
    defined(TINY_GSM_MODEM_M95) ||        \
    defined(TINY_GSM_MODEM_BG96) ||       \
    defined(TINY_GSM_MODEM_A6) ||         \
    defined(TINY_GSM_MODEM_A7) ||         \
    defined(TINY_GSM_MODEM_M590) ||       \
    defined(TINY_GSM_MODEM_MC60) ||       \
    defined(TINY_GSM_MODEM_MC60E) ||      \
    defined(TINY_GSM_MODEM_XBEE) ||       \
    defined(TINY_GSM_MODEM_SEQUANS_MONARCH)
#define GSHEET_TINYGSM_IS_AVAILABLE
#endif

#if defined(GSHEET_TINYGSM_IS_AVAILABLE) && __has_include(<TinyGsmClient.h>)
#include <TinyGsmClient.h>
#define GSHEET_GSM_MODEM_IS_AVAILABLE
#endif

#if defined(GSHEET_WIFI_IS_AVAILABLE)
#define GSHEET_WIFI_CONNECTED (WiFi.status() == WL_CONNECTED)
#else
#define GSHEET_WIFI_CONNECTED false
#endif

typedef struct gsheet_spi_ethernet_module_t
{
#if defined(ESP8266) && defined(ESP8266_CORE_SDK_V3_X_X)
#ifdef INC_ENC28J60_LWIP
    ENC28J60lwIP *enc28j60 = nullptr;
#endif
#ifdef INC_W5100_LWIP
    Wiznet5100lwIP *w5100 = nullptr;
#endif
#ifdef INC_W5500_LWIP
    Wiznet5500lwIP *w5500 = nullptr;
#endif

#endif

} GSheet_SPI_ETH_Module;

struct GSheetWiFi_credential_t
{
    String ssid;
    String password;
};

class GSheetWiFi
{
    friend class GSheetAsyncClientClass;

public:
    GSheetWiFi() {}
    ~GSheetWiFi()
    {
        clearAP();
        clearMulti();
    };
    void addAP(const String &ssid, const String &password)
    {
        GSheetWiFi_credential_t data;
        data.ssid = ssid;
        data.password = password;
        credentials.push_back(data);
    }
    void clearAP()
    {
        credentials.clear();
    }
    size_t size() { return credentials.size(); }

    GSheetWiFi_credential_t operator[](size_t index)
    {
        return credentials[index];
    }

private:
    std::vector<GSheetWiFi_credential_t> credentials;
#if defined(GSHEET_HAS_WIFIMULTI)
#if defined(ESP8266)
    ESP8266WiFiMulti *multi = nullptr;
#else
    WiFiMulti *multi = nullptr;
#endif
#endif
    void reconnect()
    {
        if (credentials.size())
        {
            disconnect();
            connect();
        }
    }

    void connect()
    {
#if defined(GSHEET_HAS_WIFIMULTI)
        clearMulti();
#if defined(ESP8266)
        multi = new ESP8266WiFiMulti();
#else
        multi = new WiFiMulti();
#endif
        for (size_t i = 0; i < credentials.size(); i++)
            multi->addAP(credentials[i].ssid.c_str(), credentials[i].password.c_str());

        if (credentials.size() > 0)
            multi->run();

#elif defined(GSHEET_WIFI_IS_AVAILABLE)
        WiFi.begin((GSHEET_CONST_STRING_CAST)credentials[0].ssid.c_str(), credentials[0].password.c_str());
#endif
    }

    void disconnect()
    {
#if defined(GSHEET_WIFI_IS_AVAILABLE)
        WiFi.disconnect();
#endif
    }

    void clearMulti()
    {
#if defined(GSHEET_HAS_WIFIMULTI)
        if (multi)
            delete multi;
        multi = nullptr;
#endif
    }
};

typedef enum
{
    gsheet_network_data_undefined,
    gsheet_network_data_default_network,
    gsheet_network_data_generic_network,
    gsheet_network_data_gsm_network,
    gsheet_network_data_ethernet_network

} gsheet_network_data_type;

typedef struct gsheet_client_static_address
{
    friend class GSheetAsyncClientClass;

public:
    /**
     * The GSheet Client Static address provider class.
     * 
     * @param ipAddress The static IP.
     * @param netMask The subnet IP.
     * @param defaultGateway The default gateway IP.
     * @param dnsServer The dns server IP.
     * @param optional The boolean option to force use static IP only (not use DHCP).
     */
    gsheet_client_static_address(IPAddress ipAddress, IPAddress netMask, IPAddress defaultGateway, IPAddress dnsServer, bool optional)
    {
        this->ipAddress = ipAddress;
        this->netMask = netMask;
        this->defaultGateway = defaultGateway;
        this->dnsServer = dnsServer;
        this->optional = optional;
    };

private:
    IPAddress ipAddress;
    IPAddress netMask;
    IPAddress defaultGateway;
    IPAddress dnsServer;
    bool optional = false;
} GSheet_StaticIP;

#endif