#include "ultra.h"
Ultrasonic centerUltra(32, 11);
Ultrasonic leftUltra(31, 10);
Ultrasonic rightUltra(30, 9);
Ultrasonic lowerUltra(33, 13);
#include "robot.h"
#include "led.h"
led rightTurnLED(27);
led greenLED(25);
led leftTurnLED(23);

led redLedSensor(48);
led blueLedSensor(50);
led greenLedSensor(46);

#include "light.h"
#include "button.h"
#include "servo.h"

button startButton(3);
button F1(53);
button F2(51);
button F3(49);
button bumper(52);

stepperMotor motorRight(
    4,    // Step pin
    7,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    4490, // Minimum velocity
    256   // Maximum velocity
);

stepperMotor motorLeft(
    2,    // Step pin
    5,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    4490, // Minimum velocity
    256   // Maximum velocity
);

gyroSensor gyro;

lightSensor lineSensors[7] = {
    {61},
    {62},
    {63},
    {64},
    {65},
    {66},
    {67}};

lightSensor greenSensors[2] = {
    {68},
    {69}};

lightSensor leftRGB(55, 0, 100);
lightSensor rightRGB(54, 0, 100);

robotBase robot(&motorRight, &motorLeft, &gyro, 39, 180);
Servo catcher(35, 90);
Servo catcherBlocker(37, 90);

int ortogonals[5] = {
    0,
    90,
    180,
    270,
    360};
