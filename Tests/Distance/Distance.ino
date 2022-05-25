#include <Ultrasonic.h>
#include "ultra.h"

#define ultraTrigger 30
#define ultraEcho 9

Ultrasonic ultraa(ultraTrigger, ultraEcho);
UltrasonicSensor ultraTest(&ultraa, 200);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println(ultraTest.read());
}
