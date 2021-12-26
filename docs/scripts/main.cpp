// Includes
#include <Arduino.h>
#include "configuration.h"
#include "utils.h"
#include "mqtt.h"

void setup() {
  DEBUG_SERIAL_SETUP(9600);
  DEBUG_SERIAL_PRINTLN("Setting up device...");

  // May be necessary after deepSleep. Otherwise you may get "error: pll_cal exceeds 2ms!!!" when trying to connect
  delay(1);

  // Network setup
  wifi_connect();
  mqtt_reconnect();
  
  DEBUG_SERIAL_PRINTLN("Restarting device...");
  ESP.restart();
  //ESP.deepSleep(1e6, RF_DISABLED);
  //ESP.deepSleep(1e6);

  DEBUG_SERIAL_PRINTLN("Running main loop...");
}

void loop() {

  wifi_reconnect();
  mqtt_reconnect();
  mqtt_loop();
  
}