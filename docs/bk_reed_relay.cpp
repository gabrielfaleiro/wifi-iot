
#include "Arduino.h"

#define REED_SWITCH_PIN D5

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

// SETUP //////////////////////////////////////////////////////////////////////

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is off when pin output is 3.3V

  // pinMode(REED_SWITCH_PIN , INPUT_PULLUP);
  pinMode(REED_SWITCH_PIN , INPUT);
}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop()
{
  uint8_t value = digitalRead(REED_SWITCH_PIN);
  digitalWrite(LED_BUILTIN, value); // LED is off when pin output is 3.3V
}
