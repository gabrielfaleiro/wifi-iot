
#include "Arduino.h"

#define REED_SWITCH_PIN D5

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

// SETUP //////////////////////////////////////////////////////////////////////

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);

  // Deep sleep mode for 30 seconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
  ESP.deepSleep(30e6);
  
  // Deep sleep mode until RESET pin is connected to a LOW signal (for example pushbutton or magnetic reed switch)
  // ESP.deepSleep(0);
}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop()
{
}
