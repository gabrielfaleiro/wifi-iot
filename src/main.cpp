
#include "Arduino.h"
#include "ESP8266WiFi.h"

#include "env.h" // This header file includes secrets that won't be commited for security. Contains:
  // WIFI_SSID
  // WIFI_PASS

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;


uint init_time = 0;
uint end_time = 0;

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

// SETUP //////////////////////////////////////////////////////////////////////

void setup() { 
  Serial.begin(9600);
  Serial.println("");

  init_time = millis();

  // Connect to WiFi
  // WiFi.mode(WIFI_STA); // Station Mode: Devices that connect to Wi-Fi networks are called stations (STA).
  // WiFi.mode(WIFI_AP); // Access Point Mode: Connection to Wi-Fi is provided by an access point (AP), that acts as a hub for one or more stations.
  WiFi.mode(WIFI_RESUME);
    // It is the user’s responsibility to preserve the WiFiState between WIFI_SHUTDOWN and WIFI_RESUME, e.g. by storing it in RTC user data and/or flash memory.
    // REF: https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiShutdown/WiFiShutdown.ino
  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi ");
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  
  end_time = millis();

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address
  Serial.println("");
  Serial.print("The MAC Address of ESP8266 Module is: ");
  Serial.print(WiFi.macAddress());
  Serial.println("");
}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop() {
  uint time = end_time - init_time;
  Serial.print("Millis to connect to wifi: ");
  Serial.println(time);

  delay(5000);
}
