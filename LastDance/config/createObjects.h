#include "robot.h"
#include "light.h"
#include "led.h"
#include "button.h"
#include "servo.h"

led builtInLED(LED_BUILTIN);

button startButton(3);
button F1(53);
button F2(51);
button F3(49);

stepperMotor motorRight(
    4,    // Step pin
    7,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
);

stepperMotor motorLeft(
    2,    // Step pin
    5,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
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

robotBase robot(&motorRight, &motorLeft, &gyro, 61);

Servo catcher(12);
Servo catcherBlocker(1);
