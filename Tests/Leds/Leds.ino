#include "class.h"

led greenLeft(43);
led turnLeft(41);
led turnRight(39);
led greenRight(37);

led redStatus(36);
led status0(38);
led status1(40);
led status2(42);

void setup()
{
}

void loop()
{
    greenLeft.on();
    delay(200);
    turnLeft.on();
    delay(200);
    turnRight.on();
    delay(200);
    greenRight.on();
    delay(200);

    redStatus.on();
    delay(200);
    status0.on();
    delay(200);
    status1.on();
    delay(200);
    status2.on();
    delay(500);

    greenLeft.off();
    delay(200);
    turnLeft.off();
    delay(200);
    turnRight.off();
    delay(200);
    greenRight.off();
    delay(200);

    redStatus.off();
    delay(200);
    status0.off();
    delay(200);
    status1.off();
    delay(200);
    status2.off();
    delay(500);
}
