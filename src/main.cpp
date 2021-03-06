// Includes
#include <Arduino.h>
#include "configuration.h"
#include "utils.h"
#include "wifi.h"

void setup() {
  DEBUG_SERIAL_SETUP(9600);
  DEBUG_SERIAL_PRINTLN("Setting up device...");

  IoTWiFi wifi;

  // Network setup
  wifi.sta_connect();
  // mqtt_reconnect();
  
  DEBUG_SERIAL_PRINTLN("Restarting device...");
  wifi.sta_disconnect();
  ESP.restart();
  //ESP.deepSleep(1e6, RF_DISABLED);
  //ESP.deepSleep(1e6);

  DEBUG_SERIAL_PRINTLN("Running main loop...");
}

void loop() {

  // wifi_reconnect();
  // mqtt_reconnect();
  // mqtt_loop();
  
}