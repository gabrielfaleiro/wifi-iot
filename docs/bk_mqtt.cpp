
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <PubSubClient.h>

#include "env.h" // This header file includes secrets that won't be commited for security. Contains:
  // WIFI_SSID
  // WIFI_PASS

#define MSG_BUFFER_SIZE	(50)

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* mqtt_server = MQTT_BROKER_ADDRESS;

uint init_time = 0;
uint end_time = 0;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// SETUP //////////////////////////////////////////////////////////////////////

void setup() { 
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is off when pin output is 3.3V

  Serial.begin(9600);
  Serial.println("");

  init_time = millis();

  // Connect to WiFi
  WiFi.mode(WIFI_STA); // Station Mode: Devices that connect to Wi-Fi networks are called stations (STA).
  // WiFi.mode(WIFI_AP); // Access Point Mode: Connection to Wi-Fi is provided by an access point (AP), that acts as a hub for one or more stations.
  // WiFi.mode(WIFI_RESUME); // TODO: Explore to speed up reconnection
    // It is the user’s responsibility to preserve the WiFiState between WIFI_SHUTDOWN and WIFI_RESUME, e.g. by storing it in RTC user data and/or flash memory.
    // REF: https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiShutdown/WiFiShutdown.ino
  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi ");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(20);
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

  uint time = end_time - init_time;
  Serial.print("Millis to connect to wifi: ");
  Serial.println(time);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%d", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    // client.publish("outTopic", msg, 1); // last parameter is undesrtood as a boolean and set the retain configuration
  }
}
