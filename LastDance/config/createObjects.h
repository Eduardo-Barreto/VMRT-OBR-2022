#include "robot.h"
#include "light.h"

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

lightSensor s0(61);
lightSensor s1(62);
lightSensor s2(63);
lightSensor s3(64);
lightSensor s4(65);
lightSensor s5(66);
lightSensor s6(67);

lightSensor lineSensors[7] = {
    s0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6};

robotBase robot(&motorRight, &motorLeft, &gyro, 61);