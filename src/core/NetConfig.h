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
#ifndef GSHEET_NETWORK_CONFIG_H
#define GSHEET_NETWORK_CONFIG_H

#include <Arduino.h>
#include "./GSheetConfig.h"
#include "./core/Network.h"
#include "./core/Memory.h"
#include "./core/Timer.h"

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
    struct gsm_data
    {
        String pin, apn, user, password;
        void *modem = nullptr;

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
    struct ethernet_data
    {
        int ethernet_reset_pin = -1;
        int ethernet_cs_pin = -1;
        uint8_t *ethernet_mac = nullptr;
        Google_Sheet_Client_StaticIP *static_ip = nullptr;

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
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
    ethernet_data ethernet;
    GSheetSPIETH *eth = NULL;
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
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
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
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        eth = NULL;
#endif
        net_timer.stop();
        net_timer.setInterval(0);
    }
};

class GSheetDefaultNetwork
{

public:
    GSheetDefaultNetwork(bool reconnect = true)
    {
        init();
        network_data.network_data_type = gsheet_network_data_default_network;
        network_data.reconnect = reconnect;
    }
    ~GSheetDefaultNetwork() { clear(); }
    void clear()
    {
        network_data.clear();
    }
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
    GSheetEthernetNetwork(uint8_t macAddress[6], int csPin, int resetPin, GSheetStaticIP *staticIP = nullptr)
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
    GSheetDefaultEthernetNetwork(GSheetSPIETH &eth)
    {
        init();
#if defined(GSHEET_ETHERNET_MODULE_IS_AVAILABLE) && defined(GSHEET_ENABLE_ETHERNET_NETWORK)
        network_data.eth = &eth;
#endif
        network_data.network_data_type = gsheet_network_data_default_network;
    }

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
    GSheetDefaultWiFiNetwork(GSheetWiFi &wifi, bool reconnect = true)
    {
        init();
        network_data.wifi = &wifi;
        network_data.reconnect = reconnect;
        network_data.network_data_type = gsheet_network_data_default_network;
    }
    ~GSheetDefaultWiFiNetwork() { clear(); }
};

namespace GSHEET
{
    template <typename T>
    static gsheet_network_config_data &getNetwork(T &net) { return net.get(); }
}

#endif