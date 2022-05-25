/*
Módulo de configuração dos sensores de luz
*/

/* #define light0Pin A7
#define light1Pin A8
#define light2Pin A9
#define light3Pin A10
#define light4Pin A11
#define light5Pin A12
#define light6Pin A13

#define green0Pin A14
#define green1Pin A15

int light[7];
int green[2]; */

#include "calc.h"

class lightSensor
{
private:
    byte pin;

    void init()
    {
        pinMode(pin, INPUT);
    }

public:
    int minRead;
    int maxRead;
    byte blackThreshold;
    byte minGreen;
    byte maxGreen;
    int raw;
    byte light;
    bool black;
    bool green;

    lightSensor(byte pin, int minRead = 0, int maxRead = 1023, byte blackThreshold = 50)
    {
        this->pin = pin;
        this->minRead = minRead;
        this->maxRead = maxRead;
        this->blackThreshold = blackThreshold;
        init();
    }

    void read()
    {
        this->raw = analogRead(pin);
        this->light = constrain(map(raw, minRead, maxRead, 100, 0), 0, 100);
        this->black = light >= blackThreshold;
        this->green = (light >= minGreen && light <= maxGreen);
    }

    byte getLight()
    {
        read();
        return this->light;
    }

    int getRawRead()
    {
        read();
        return this->raw;
    }

    void setGreen(byte _read, byte _interval)
    {
        this->minGreen = _read - _interval;
        this->maxGreen = _read + _interval;
    }
};
