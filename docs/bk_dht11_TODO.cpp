
#include "Arduino.h"
// #include <ESP8266WiFi.h>

// Comment DEBUG definition to avoid preprocesor reporting info trough the Serial port
#define DEBUG

// // Digital pin to read sensor
// #define DHTPIN D5
// // Sensor type
// #define DHTTYPE DHT11

#define REED_SWITCH_PIN D3

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

void toggleDigitalPin(uint_fast8_t pin){
  int value = 0;
  value = digitalRead(pin);
  digitalWrite(pin, !value);
}

void toggleLed(){
  toggleDigitalPin(LED_BUILTIN);
}

// void printDH11(){
//   float rh = 0;
//   float temp = 0;

//   rh = dht.getHumidity();
//   temp = dht.getTemperature();

//   // if (isnan(rh) || isnan(temp)) {
//   //   Serial.println("Error obteniendo los datos del sensor DHT11");
//   //   return;
//   // }
//   // else{
//     Serial.print("rh: ");
//     Serial.println(rh);
//     Serial.print("temp: ");
//     Serial.println(temp);
//   // }
// }

// SETUP //////////////////////////////////////////////////////////////////////

void setup()
{
  // #ifdef DEBUG
  Serial.begin(9600);
  // #endif

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is off when pin output is 3.3V

  pinMode(REED_SWITCH_PIN , INPUT_PULLUP); 
}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop()
{

  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

  // #ifdef DEBUG
  // printDH11();
  toggleLed();
  // #endif

  delay(1000);
}
