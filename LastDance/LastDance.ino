#define DEBUG 0
#define DEBUG_LOG 0

#if DEBUG_LOG == 1
#define DebugLog(x) Serial.print(String(x))
#define DebugLogln(x) Serial.println(String(x))
#else
#define DebugLog(x)
#define DebugLogln(x)
#endif

#include "config/createObjects.h"
#include "routines/followLine.h"

void setup()
{
    gyro.init();
    calibrateLineFollower();
    delay(500);
    calibrateLineFollower();
    delay(500);
    /*
    delay(2000);
    for (int i = 0; i < 7; i++)
    {
        lineSensors[i].read();
        lineSensors[i].setGreen(lineSensors[i].light, 5);
    }
    */
    delay(1500);
    Serial.begin(115200);
}

void loop()
{
    unsigned long timeout = millis() + 5000;
    while (millis() < timeout)
        proportional(80, -0.25, 0, 0);

    // checkTurn();
}
