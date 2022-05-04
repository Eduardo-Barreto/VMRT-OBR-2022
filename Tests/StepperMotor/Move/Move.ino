#include "robot.h"

stepperMotor motorRight(
    2,    // Step pin
    5,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
);

stepperMotor motorLeft(
    3,    // Step pin
    6,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
);

robotBase robotBase(&motorRight, &motorLeft, 61);

void setup()
{
    mpuSetup();
    Serial.begin(9600);
}

void loop()
{
    robotBase.moveCentimeters(100, 100, 10);
    delay(3000);
    robotBase.moveCentimeters(-100, -100, 10);
    delay(3000);
}