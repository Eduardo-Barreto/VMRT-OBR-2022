#define DEBUG 1
#define DEBUG_LOG 1

#define CALIBRATE_LINE_SENSORS 0

#if DEBUG_LOG == 1
#define DebugInit(x) Serial.begin(x)
#define DebugLog(x) Serial.print(String(x))
#define DebugLogln(x) Serial.println(String(x))
#else
#define DebugInit(x)
#define DebugLog(x)
#define DebugLogln(x)
#endif

int masterPower = 95;
int maxPower = 80;
int targetPower = masterPower + 10;
int turnPower = masterPower;

#include "config/createObjects.h"
#include "routines/followLine.h"
#include "routines/calibrate.h"

void setup()
{
    // Pinos invadidos pelo giroscópio
    pinMode(16, INPUT);
    pinMode(17, INPUT);
    pinMode(18, INPUT);
    pinMode(19, INPUT);
    gyro.init();

#if CALIBRATE_LINE_SENSORS == 1
#endif

    delay(1500);
    DebugInit(115200);

    startButton.waitForPressAndRelease(
        []() -> void
        { builtInLED.blink(200); },
        []() -> void
        { builtInLED.blink(100); });

    motorLeft.on();
    motorRight.on();

#if CALIBRATE_LINE_SENSORS == 1
    calibrateLineFollower();
    delay(500);
    calibrateLineFollower();
    delay(500);

    startButton.waitForPressAndRelease(
        []() -> void
        { builtInLED.blink(200); },
        []() -> void
        { builtInLED.blink(100); });

    greenSensors[0].setGreen();
    greenSensors[1].setGreen();
    delay(150);

    saveCalibration();
#endif

    loadCalibrationSaved();
    delay(750);
}

void debugLoop()
{
    /* robot.move(10, 10);
    while (greenSensors[0].getGreen())
    {
        builtInLED.blink(400);
    }

    while (greenSensors[1].getGreen())
    {
        builtInLED.blink(100);
    } */

    printCalibrationSaved();
    delay(9999999);
}

void loop()
{
#if DEBUG == 1
    debugLoop();
#else
    runFloor();
#endif
}
