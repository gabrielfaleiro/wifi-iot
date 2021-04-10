
#include "Arduino.h"
#include "DHTesp.h"

class Dht11 {
    private:
        uint8_t data_pin = 0;
        DHTesp dht;
    public:
        void setup(uint8_t pin, DHTesp::DHT_MODEL_t model);
};