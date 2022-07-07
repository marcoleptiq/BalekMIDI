#ifndef LEDS_H
#define LEDS_H

class Leds{
private:

    int ledPin[4]={0};
    int temps = 0;
    int mesure = 0;

public:

    Leds(int ledPin[4]);
    void Sync(void);

};


#endif