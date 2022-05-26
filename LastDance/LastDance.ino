#include "config/createObjects.h"
#include "routines/followLine.h"

void setup()
{
    gyro.init();
    calibrateLineFollower();
    delay(500);
    calibrateLineFollower();
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
    while (true)
    {
        proportional(50, 8);
    }
}
