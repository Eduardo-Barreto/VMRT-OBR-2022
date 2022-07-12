#define DEBUG 0
#define DEBUG_LOG 0

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

int masterPower = 80;
int maxPower = 95;
int targetPower = masterPower + 10;
int turnPower = masterPower;

#include "config/createObjects.h"
#include "routines/followLine.h"
#include "routines/calibrate.h"

void setup()
{
    gyro.init();

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
    startButton.waitForPressAndRelease();
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
    builtInLED.off();
}

int counter = 0;
unsigned long startTime = millis();
unsigned long elapsedTime = 0;
void debugLoop()
{
    while (lineSensors[6].getLight() > 20)
        robot.move(targetPower, targetPower);
    robot.stop(100);
    while (lineSensors[6].getLight() > 20)
        robot.move(-15, -15);
    robot.stop(500);
    robot.turnOffMotors();
    startButton.waitForPressAndRelease(
        []() -> void
        {
            DebugLog(lineSensors[6].getLight() < 20);
            DebugLog("\t");
            DebugLog(greenSensors[1].minGreen);
            DebugLog(" < ");
            DebugLog(greenSensors[1].getRawRead());
            DebugLog(" > ");
            DebugLog(greenSensors[1].maxGreen);
            DebugLog(" = ");
            DebugLogln(greenSensors[1].getGreen());
        });
    robot.turnOnMotors();
}

void loop()
{
#if DEBUG == 1
    debugLoop();
#else
    runFloor();
#endif
}
