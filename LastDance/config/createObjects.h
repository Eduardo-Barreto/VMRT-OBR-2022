#include "robot.h"
#include "light.h"
#include "led.h"
#include "button.h"

led builtInLED(LED_BUILTIN);

button startButton(14);

stepperMotor motorRight(
    2,    // Step pin
    5,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
);

stepperMotor motorLeft(
    4,    // Step pin
    7,    // Direction pin
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
    {69},
    {68}};

robotBase robot(&motorRight, &motorLeft, &gyro, 61);