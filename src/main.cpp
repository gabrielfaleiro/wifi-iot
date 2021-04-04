
#include "Arduino.h"
#include "DHT.h"

// Correct internal values. They work swapped for the LED_BUILTIN
#define LOW 0x1
#define HIGH 0x0

// Comment DEBUG definition to avoid preprocesor reporting info trough the Serial port
#define DEBUG 1

// Digital pin to read sensor
#define DHTPIN D6
// Sensor type
#define DHTTYPE DHT11

#define REED_SWITCH_PIN D3

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////

// Temp RH DHT11 Sensor initialisation
DHT dht(DHTPIN, DHTTYPE);

// CUSTOM FUNCTIONS ///////////////////////////////////////////////////////////

void toggleLed(){
  int value = 0;
  value = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !value);
}

// #ifdef DEBUG
void printDH11(float rh, float temp){
  if (isnan(rh) || isnan(temp)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  else{
    Serial.print("rh: ");
    Serial.println(rh);
    Serial.print("temp: ");
    Serial.println(temp);
  }
}
// #endif

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
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(REED_SWITCH_PIN , INPUT_PULLUP); 
}

// LOOP ///////////////////////////////////////////////////////////////////////

void loop()
{
  unsigned long currentMillis = 0;

  float rh = 0;
  float temp = 0;

  currentMillis = millis();

  rh = dht.readHumidity();
  temp = dht.readTemperature();

  // #ifdef DEBUG
  printDH11(rh,temp);
  toggleLed();
  // #endif

  delay(500);
}
