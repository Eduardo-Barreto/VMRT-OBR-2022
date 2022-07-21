#define DEBUG 0
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

int masterPower = 80;
int maxPower = 95;
int targetPower = masterPower + 10;
int turnPower = masterPower;

#include <avr/wdt.h>
#include "config/createObjects.h"
#include "routines/followLine.h"
#include "routines/calibrate.h"
#include "routines/catcher.h"
#include "routines/interruptMode.h"

void setup()
{
    wdt_disable();
    gyro.init();

    DebugInit(115200);

    startButton.waitForRelease();

    startButton.waitForPressAndRelease(
        []() -> void
        {
            builtInLED.blink(200);
            rightTurnLED.blink(200);
            greenLED.blink(200);
            leftTurnLED.blink(200);
        },
        []() -> void
        {
            builtInLED.blink(100);
            rightTurnLED.blink(100);
            greenLED.blink(100);
            leftTurnLED.blink(100);
        });

    motorLeft.on();
    motorRight.on();

#if CALIBRATE_LINE_SENSORS == 1
    greenLED.off();
    rightTurnLED.on();
    leftTurnLED.on();
    calibrateLineFollower();
    startButton.waitForPressAndRelease(
        []() -> void
        {
            builtInLED.blink(200);
            rightTurnLED.blink(200);
            leftTurnLED.blink(200);
        },
        []() -> void
        {
            builtInLED.blink(100);
            rightTurnLED.blink(100);
            leftTurnLED.blink(100);
        });
    calibrateLineFollower();
    rightTurnLED.off();
    leftTurnLED.off();
    delay(500);

    startButton.waitForPressAndRelease(
        []() -> void
        { greenLED.blink(200); },
        []() -> void
        { greenLED.blink(100); });

    greenLED.on();
    greenSensors[0].setGreen();
    greenSensors[1].setGreen();
    delay(150);
    saveCalibration();
#endif

    loadCalibrationSaved();
    delay(750);
    builtInLED.off();
    rightTurnLED.off();
    greenLED.off();
    leftTurnLED.off();

    attachInterrupt(digitalPinToInterrupt(startButton.pin), interruptMenu, LOW);
}

int angle = 0;
void debugLoop()
{
    robot.turnOffMotors();

    DebugLogln(centerUltra.read());
}

void loop()
{
#if DEBUG == 1
    debugLoop();
#else
    runFloor();
#endif
}
