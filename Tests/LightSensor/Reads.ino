#include "light.h"

#define minRead 0
#define maxRead 1023

lightSensor s0(A7, 950, 1012);
lightSensor s1(A8, 300, 945);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    s0.read();
    s1.read();
    Serial.println((String)s0.raw + "," + (String)s0.light + "," + (String)s1.raw + "," + (String)s1.light);
    delay(150);
}
