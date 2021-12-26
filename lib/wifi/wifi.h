
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <include/WiFiState.h>
#include "configuration.h"
#include "utils.h"


#ifndef _WIFI_H_
#define _WIFI_H_

#define WIFI_CONNECT_TIMEOUT    10000

#define STA_STATE_OFFSET        224
#define STA_DHCP_OFFSET         376
#define AP_ENABLE_OFFSET         377

// TODO: set wifi AP
// TODO: handle wifi error
// TODO: set output power

class IoTWiFi {
    private:
        WiFiClient client;
        bool sta_state_save;
        bool sta_enable;

        // From flash memory
        IPAddress sta_ip;
        IPAddress sta_gw;
        IPAddress sta_net;

        char sta_ssid[100];
        char sta_pass[100];

        WiFiState sta_state;

        bool sta_dhcp;


        bool ap_enable;


        bool wifi_mode();
        bool sta_begin();

    public: 
        IoTWiFi();
        void load_from_memory();
        
        void sta_connect();
        void sta_disconnect();
        wl_status_t sta_status();

};


#endif
