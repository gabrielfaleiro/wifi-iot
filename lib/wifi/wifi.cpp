#include "wifi.h"


IoTWiFi::IoTWiFi(){

    sta_state_save = false;
    sta_enable = true;

    load_from_memory();
}


void IoTWiFi::load_from_memory(){
    ESP.rtcUserMemoryRead(0, reinterpret_cast<uint32_t *>(&sta_ip), sizeof(sta_ip)); // size: 8
    ESP.rtcUserMemoryRead(8, reinterpret_cast<uint32_t *>(&sta_gw), sizeof(sta_gw)); // size: 8
    ESP.rtcUserMemoryRead(16, reinterpret_cast<uint32_t *>(&sta_net), sizeof(sta_net)); // size: 8
    ESP.rtcUserMemoryRead(24, reinterpret_cast<uint32_t *>(&sta_ssid), sizeof(sta_ssid)); // size: 100
    ESP.rtcUserMemoryRead(124, reinterpret_cast<uint32_t *>(&sta_pass), sizeof(sta_pass)); // size: 100
    ESP.rtcUserMemoryRead(STA_STATE_OFFSET, reinterpret_cast<uint32_t *>(&sta_state), sizeof(sta_state)); // size: 152
    ESP.rtcUserMemoryRead(STA_DHCP_OFFSET, reinterpret_cast<uint32_t *>(&sta_dhcp), sizeof(sta_dhcp)); // size: 1
    ESP.rtcUserMemoryRead(AP_ENABLE_OFFSET, reinterpret_cast<uint32_t *>(&ap_enable), sizeof(ap_enable)); // size: 1
    // TODO: check that this is correct.
}


void IoTWiFi::sta_connect(){
    int init_time = 0;
    int wifi_time = 0;

    init_time = millis();

    // May be necessary after deepSleep. Otherwise you may get "error: pll_cal exceeds 2ms!!!" when trying to connect
    while(millis() < 2);

    if (!WiFi.resumeFromShutdown(sta_state)
        || (WiFi.waitForConnectResult(WIFI_CONNECT_TIMEOUT) != WL_CONNECTED)) {

        DEBUG_SERIAL_PRINTLN("Cannot resume WiFi connection, connecting via begin...");
        sta_state_save = true;
        WiFi.persistent(false);

        // TODO: apply sta_dhcp on WiFi.begin -> create an internal method
        // TODO: apply ap_enable on WiFi.mode -> create an internal method
        if (!wifi_mode()
            || !sta_begin()
            || (WiFi.waitForConnectResult(WIFI_CONNECT_TIMEOUT) != WL_CONNECTED)) {

            DEBUG_SERIAL_PRINTLN("Cannot connect to WiFi!");
            sta_enable = false;
            ap_enable = true;
            wifi_mode();
            // TODO: deploy only AT
            return;
        }
    }

    wifi_time = millis();

    DEBUG_SERIAL_PRINT("Local IP address: ");
    DEBUG_SERIAL_PRINTLN(WiFi.localIP());

    DEBUG_SERIAL_PRINT("channel: ");
    DEBUG_SERIAL_PRINTLN(WiFi.channel());

    DEBUG_SERIAL_PRINT("BSSID: ");
    DEBUG_SERIAL_PRINTLN(WiFi.BSSIDstr());

    DEBUG_SERIAL_PRINT("millis to connect wifi = ");
    DEBUG_SERIAL_PRINTLN(wifi_time-init_time);
}


void IoTWiFi::sta_disconnect(){
    WiFi.shutdown(sta_state);
    if(sta_state_save){
        // TODO: when working, uncomment to save flash livetime
        //ESP.rtcUserMemoryWrite(STA_STATE_OFFSET, reinterpret_cast<uint32_t *>(&sta_state), sizeof(sta_state));
    }
}


bool IoTWiFi::wifi_mode(){
    WiFiMode_t w_mode = WIFI_OFF;

    if(ap_enable){
        if(sta_enable) w_mode = WIFI_AP_STA;
        else w_mode = WIFI_AP;
    }
    else{
        if(sta_enable) w_mode = WIFI_STA;
        else w_mode = WIFI_OFF;
    }

    return WiFi.mode(w_mode);
}

bool IoTWiFi::sta_begin(){
    if(!sta_dhcp){
        WiFi.config(sta_ip, sta_gw, sta_net);
    }
    return WiFi.begin(sta_ssid, sta_pass);
}