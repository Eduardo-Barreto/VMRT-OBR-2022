#include "config/robot.h"

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

gyroSensor gyro;

robotBase robot(&motorRight, &motorLeft, &gyro, 61);

void setup()
{
    // Serial.begin(2000000);
    gyro.init();
    delay(500);
}

void loop()
{
    robot.moveCentimeters(50, 100, 10);
    delay(1500);
    robot.moveCentimeters(50, -100, 10);
    delay(1500);
}
