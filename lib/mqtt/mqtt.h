
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "configuration.h"
#include "utils.h"

#include <include/WiFiState.h>


#ifndef _MQTT_H_
#define _MQTT_H_

#define RTC_USER_DATA_SLOT_WIFI_STATE 33u

#define WIFI_RETRY_MS 10
#define MQTT_RETRY_MS 10

int8_t wifi_connect();
void wifi_reconnect();

void mqtt_reconnect();
void mqtt_loop();

void callback(char *led_control, byte *payload, unsigned int length);
void mqtt_publish_message(char *section, char *payload, boolean retain);

#endif
