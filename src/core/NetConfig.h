/**
 * Created May 31, 2024
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
#ifndef GSHEET_NETWORK_CONFIG_H
#define GSHEET_NETWORK_CONFIG_H

#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/Network.h"
#include "./core/Memory.h"

#define GSHEET_NET_RECONNECT_TIMEOUT_SEC 10000

struct gsheet_network_config_data
{
    friend class GSheetDefaultNetwork;
    friend class GSheetGenericNetwork;
    friend class GSheetEthernetNetwork;
    friend class GSheetGSMNetwork;
    friend class GSheetDefaultEthernetNetwork;
    friend class GSheetDefaultWiFiNetwork;
    friend class GSheetAsyncClientClass;

private:
    struct generic_data
    {
        GSheetNetworkConnectionCallback net_con_cb = NULL;
        GSheetNetworkStatusCallback net_status_cb = NULL;

    public:
        void copy(generic_data &rhs)
        {
            this->net_con_cb = rhs.net_con_cb;
            this->net_status_cb = rhs.net_status_cb;
        }
        void clear()
        {
            net_con_cb = NULL;
            net_status_cb = NULL;
        }
    };

#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE) && defined(GSHEET_ENABLE_GSM_NETWORK)

    enum gsm_conn_status
    {
        gsm_conn_status_idle,
        gsm_conn_status_waits_network,
        gsm_conn_status_network_connected,
        gsm_conn_status_waits_gprs
    };

    struct gsm_data
    {
        String pin, apn, user, password;
        void *modem = nullptr;
        gsm_conn_status conn_status;

    public:
        void copy(gsm_data &rhs)
        {
            this->modem = rhs.modem;
            this->pin = rhs.pin;
            this->apn = rhs.apn;
            this->user = rhs.user;
            this->password = rhs.password;
        }
        void clear()
        {
            modem = nullptr;
            pin.clear();
            apn.clear();
            user.clear();
            password.clear();
        }
    };
#endif
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)

    enum ethernet_conn_status
    {
        ethernet_conn_status_idle,
        ethernet_conn_status_rst_pin_unselected,
        ethernet_conn_status_rst_pin_selected,
        ethernet_conn_status_rst_pin_released,
        ethernet_conn_status_begin,
        ethernet_conn_status_waits
    };

    // SPI Ethernet Module Data
    struct ethernet_data
    {
        int ethernet_reset_pin = -1;
        int ethernet_cs_pin = -1;
        uint8_t *ethernet_mac = nullptr;
        GSheet_StaticIP *static_ip = nullptr;
        ethernet_conn_status conn_satatus;
        unsigned long stobe_ms = 0;

    public:
        void copy(ethernet_data &rhs)
        {
            this->ethernet_reset_pin = rhs.ethernet_reset_pin;
            this->ethernet_cs_pin = rhs.ethernet_cs_pin;
            this->ethernet_mac = rhs.ethernet_mac;
            this->static_ip = rhs.static_ip;
        }
        void clear()
        {
            ethernet_mac = nullptr;
            ethernet_cs_pin = -1;
            ethernet_reset_pin = -1;
            static_ip = nullptr;
        }
    };
#endif

    gsheet_network_data_type network_data_type = gsheet_network_data_undefined;

    generic_data generic;
#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE) && defined(GSHEET_ENABLE_GSM_NETWORK)
    gsm_data gsm;
#endif
#if defined(GSHEET_ENABLE_ETHERNET_NETWORK)
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE)
    ethernet_data ethernet;
#endif
#if defined(GSHEET_LWIP_ETH_IS_AVAILABLE)
    GSheet_SPI_ETH_Module *eth = NULL;
#endif
#endif
    bool initialized = false;
    bool network_status = false;
    bool reconnect = true;
    GSheetWiFi *wifi = nullptr;
    GSheetTimer net_timer;
    GSheetTimer eth_timer;

public:
    ~gsheet_network_config_data() { clear(); }
    gsheet_network_config_data &operator=(gsheet_network_config_data rhs)
    {
        copy(rhs);
        return *this;
    }

    void copy(gsheet_network_config_data &rhs)
    {
        this->network_data_type = rhs.network_data_type;
        this->initialized = rhs.initialized;
        this->reconnect = rhs.reconnect;
        generic.copy(rhs.generic);
#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE) && defined(GSHEET_ENABLE_GSM_NETWORK)
        gsm.copy(rhs.gsm);
#endif

#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        ethernet.copy(rhs.ethernet);
#endif

        this->initialized = rhs.initialized;
        this->network_status = rhs.network_status;
        this->reconnect = rhs.reconnect;
        this->wifi = rhs.wifi;
#if defined(GSHEET_LWIP_ETH_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        this->eth = rhs.eth;
#endif
        this->net_timer = rhs.net_timer;
        this->net_timer.start();
    }

    void clear()
    {
        generic.clear();
#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE) && defined(GSHEET_ENABLE_GSM_NETWORK)
        gsm.clear();
#endif
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        ethernet.clear();
#endif
        network_data_type = gsheet_network_data_undefined;
        initialized = false;
        network_status = false;
        reconnect = true;
        wifi = nullptr;
#if defined(GSHEET_LWIP_ETH_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        eth = NULL;
#endif
        net_timer.stop();
        net_timer.setInterval(0);
    }
};

class GSheetDefaultNetwork
{

public:
    /**
     * The base class that provides the network data or default network class for built-in WiFi boards.
     *
     * @param reconnect The boolean option for network reconnection which applied for WiFi network only.
     */
    GSheetDefaultNetwork(bool reconnect = true)
    {
        init();
        network_data.network_data_type = gsheet_network_data_default_network;
        network_data.reconnect = reconnect;
    }
    ~GSheetDefaultNetwork() { clear(); }

    /**
     * Clear the internal network data.
     */
    void clear()
    {
        network_data.clear();
    }

    /**
     * Get the reference to the internal gsheet_network_config_data.
     *
     * @return gsheet_network_config_data & The reference to internal gsheet_network_config_data.
     */
    gsheet_network_config_data &get() { return network_data; }

protected:
    gsheet_network_config_data network_data;

    void init()
    {
        clear();
        network_data.initialized = true;
    }
};

class GSheetGenericNetwork : public GSheetDefaultNetwork
{
public:
    /**
     * The network class works for all networking interfaces e.g. Ethernet Module, WiFI Module, Bluetooth Module, NB-IoT module, and LoRa Module that have the internet access ability.
     *
     * @param networkConnectionCB The network connection callback function.
     * @param networkStatusCB The network status callback function.
     *
     * In the networkConnectionCB function, the complete operations for the carier (network) and internet connection should be performed and waits until the internet connection was established.
     * In the networkStatusCB function, the `status` (Boolean variable) that provided in the function, should set with the network status.
     *
     */
    GSheetGenericNetwork(GSheetNetworkConnectionCallback networkConnectionCB, GSheetNetworkStatusCallback networkStatusCB)
    {
        init();
        network_data.generic.net_con_cb = networkConnectionCB;
        network_data.generic.net_status_cb = networkStatusCB;
        network_data.network_data_type = gsheet_network_data_generic_network;
    }
    ~GSheetGenericNetwork() { clear(); }
};

#if defined(GSHEET_GSM_MODEM_IS_AVAILABLE) && defined(GSHEET_ENABLE_GSM_NETWORK)
class GSheetGSMNetwork : public GSheetDefaultNetwork
{
public:
    /**
     * The GSM network class works for TinyGSM only (https://github.com/vshymanskyy/TinyGSM).
     *
     * @param modem The pointer to TinyGsm modem object. Modem should be initialized and/or set the mode before transfering the data.
     * @param pin The SIM pin
     * @param apn The GPRS APN (Access Point Name).
     * @param user The GPRS user.
     * @param password The GPRS password.
     *
     * This required two places of the GSM module macro definitions, one in your sketch and another in /src/GSheetConfig.h
     * or in your own defined config at src/UserGSheetConfig.h or adding `TINY_GSM_MODEM_XXXXXXX` in compiler build flags.
     *
     */
    GSheetGSMNetwork(TinyGsm *modem, const String &pin, const String &apn, const String &user, const String &password)
    {
        init();
        network_data.gsm.modem = modem;
        network_data.gsm.pin = pin;
        network_data.gsm.apn = apn;
        network_data.gsm.user = user;
        network_data.gsm.password = password;
        network_data.network_data_type = gsheet_network_data_gsm_network;
    }
    ~GSheetGSMNetwork() { clear(); }
};
#endif

#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
class GSheetEthernetNetwork : public GSheetDefaultNetwork
{

public:
    /**
     * The Ethernet network class for generic Ethernet modules.
     *
     * @param macAddress The mac address.
     * @param csPin The Ethernet module chip select/enable pin.
     * @param resetPin The Ethernet module reset pin. Assign -1 if not used.
     * @param staticIP (Optional) The pointer to GSheet_StaticIP object that holds the static ip configuration.
     *
     * The GSheet_StaticIP class constructor parameters for static IP are following.
     * @param ipAddress The static IP.
     * @param netMask The subnet IP.
     * @param defaultGateway The default gateway IP.
     * @param dnsServer The dns server IP.
     * @param optional The boolean option to force use static IP only (not use DHCP).
     *
     * By default the external Ethernet module can be used with the library when the macro GSHEET_ENABLE_ETHERNET_NETWORK was defined and Ethernet library was included in the user sketch.
     * The user defined Ethernet class and header other than `Ethernet.h` and `Ethernet` can be used.
     */
    GSheetEthernetNetwork(uint8_t macAddress[6], int csPin, int resetPin, GSheet_StaticIP *staticIP = nullptr)
    {
        init();
        network_data.ethernet.ethernet_mac = macAddress;
        network_data.ethernet.ethernet_cs_pin = csPin;
        network_data.ethernet.ethernet_reset_pin = resetPin;
        network_data.ethernet.static_ip = staticIP;
        network_data.network_data_type = gsheet_network_data_ethernet_network;
    }
    ~GSheetEthernetNetwork() { clear(); }
};

#endif

class GSheetDefaultEthernetNetwork : public GSheetDefaultNetwork
{

public:
    /**
     * The default Ethernet class for ESP8266 with lwIP Ethernet.
     *
     * @param eth The ESP8266 core SPI ethernet driver class that work with external SPI Ethernet modules that currently supported e.g. ENC28J60, Wiznet W5100 and Wiznet 5500.
     *
     * This eth should be defined at the same usage scope of GSheetAsyncClientCalss.
     *
     * To use ESP8266 native lwIP Ethernet, the one of following macros, GSHEET_ENABLE_ESP8266_ENC28J60_ETH, GSHEET_ENABLE_ESP8266_W5500_ETH
     * and GSHEET_ENABLE_ESP8266_W5100_ETH should be defined in src/GSheetConfig.h or in your own defined config at src/UserGSheetConfig.h
     * or adding GSHEET_ENABLE_ESP8266_ENC28J60_ETH, GSHEET_ENABLE_ESP8266_W5500_ETH and GSHEET_ENABLE_ESP8266_W5100_ETH in the compiler build flags.
     *
     * Use GSheet_SPI_ETH_Module::enc28j60, GSheet_SPI_ETH_Module::w5100 and GSheet_SPI_ETH_Module::w5500 to assign the pointer to
     * ENC28J60lwIP, Wiznet5100lwIP and Wiznet5500lwIP classes objects respectively.
     *
     * In PlatformIO IDE, please set the 'lib_ldf_mode = chain+' option in platformio.ini.
     *
     */
    GSheetDefaultEthernetNetwork(GSheet_SPI_ETH_Module &eth)
    {
        init();
#if defined(GSHEET_LWIP_ETH_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        network_data.eth = &eth;
#endif
        network_data.network_data_type = gsheet_network_data_default_network;
    }

    /**
     * The default Ethernet class for ESP32 with native Ethernet boards.
     */
    GSheetDefaultEthernetNetwork()
    {
        init();
        network_data.network_data_type = gsheet_network_data_default_network;
    }
    ~GSheetDefaultEthernetNetwork() { clear(); }
};

class GSheetDefaultWiFiNetwork : public GSheetDefaultNetwork
{

public:
    /**
     * The WiFi network class with multiple APs supports.
     * @param wifi The GSheetWiFi class object that used for keeping the network credentials (WiFi APs and WiFi passwords).
     * @param reconnect  The bool option for network reconnection. It should set with `false` when the WiFi reconnection was controlled by your code or third-party library e.g. WiFiManager.
     *
     * The GSheetWiFi class holds the WiFi credentials list. The AP and password can be added to list with GSheetWiFi::addAP.
     * The GSheetWiFi object should be defined at the same usage scope of DefaultWiFiNetwork and GSheetAsyncClientClass.
     */
    GSheetDefaultWiFiNetwork(GSheetWiFi &wifi, bool reconnect = true)
    {
        init();
        network_data.wifi = &wifi;
        network_data.reconnect = reconnect;
        network_data.network_data_type = gsheet_network_data_default_network;
    }
    ~GSheetDefaultWiFiNetwork() { clear(); }
};

namespace gsheet
{
    template <typename T>
    static gsheet_network_config_data &getNetwork(T &net) { return net.get(); }
}

#endif