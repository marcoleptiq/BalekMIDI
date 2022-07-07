#include "Leds.h"
#include <Arduino.h>

Leds::Leds(int ledPin[4]){
    for (int i = 0; i < 4; i++)
    {
        this->ledPin[i] = ledPin[i];
        pinMode(this->ledPin[i], OUTPUT);
    }
    
}

void Leds::Sync() {
    
}