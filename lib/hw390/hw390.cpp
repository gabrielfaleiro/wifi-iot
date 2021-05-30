#include "dht11.h"

void Dht11::setup(uint8_t pin, DHTesp::DHT_MODEL_t model){
    dht.setup(pin, model);
}

