#include "config/light.h"
#include "routines/followLine.h"

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

lineFollower lineFollower(&robot, 40, &lineSensors);

void setup()
{
    gyro.init();
    lineFollower.calibrate();
    delay(500);
    lineFollower.calibrate();
    delay(500);
    delay(2000);
    for (int i = 0; i < 7; i++)
    {
        lineSensors[i].read();
        lineSensors[i].setGreen(lineSensors[i].light, 5);
    }
    delay(1500);
}

void loop()
{
    lineFollower.run(50, 35);
    // lineFollower.checkTurn();
}
