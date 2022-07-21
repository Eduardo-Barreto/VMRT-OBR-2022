#include "class.h"

Ultrasonic ultra(7, 9);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    Serial.println(ultra.read());
}
