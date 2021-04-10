
#include "Arduino.h"
// #include <ESP8266WiFi.h>

#include <Adafruit_Sensor.h>
#include "DHT.h"

// Correct internal values. They work swapped for the LED_BUILTIN
// #define LOW 0x1
// #define HIGH 0x0

// Comment DEBUG definition to avoid preprocesor reporting info trough the Serial port
#define DEBUG 1

// Digital pin to read sensor
#define DHTPIN D5
// Sensor type
#define DHTTYPE DHT11

#define REED_SWITCH_PIN D3

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

// DHT11 Sensor initialisation
DHT dht(DHTPIN, DHTTYPE);

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

void toggleLed(){
  int value = 0;
  value = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !value);
}

void printDH11(){
  float rh = 0;
  float temp = 0;

  rh = dht.readHumidity();
  temp = dht.readTemperature();

  // if (isnan(rh) || isnan(temp)) {
  //   Serial.println("Error obteniendo los datos del sensor DHT11");
  //   return;
  // }
  // else{
    Serial.print("rh: ");
    Serial.println(rh);
    Serial.print("temp: ");
    Serial.println(temp);
  // }
}

// SETUP //////////////////////////////////////////////////////////////////////

void setup()
{
  // #ifdef DEBUG
  Serial.begin(9600);
  // #endif

  // Initialise DHT
  pinMode(DHTPIN, INPUT);
  dht.begin();

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is off when pin output is 3.3V

  pinMode(REED_SWITCH_PIN , INPUT_PULLUP); 
}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop()
{
  // #ifdef DEBUG
  printDH11();
  toggleLed();
  // #endif

  delay(2000);
}
