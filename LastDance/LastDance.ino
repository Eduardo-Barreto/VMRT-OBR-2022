#define DEBUG 0
#define DEBUG_LOG 1

#if DEBUG_LOG == 1
#define DebugInit(x) Serial.begin(x)
#define DebugLog(x) Serial.print(String(x))
#define DebugLogln(x) Serial.println(String(x))
#else
#define DebugInit(x)
#define DebugLog(x)
#define DebugLogln(x)
#endif

volatile int state = 1;             // Variável de estado
int masterPower = 60;               // Controlador principal de base da velocidade
int maxPower = 75;                  // Velocidade máxima do robô ao seguir linha
int targetPower = masterPower + 10; // Velocidade atual do robô ao seguir linha
int turnPower = 17;                 // Velocidade do robô ao virar seguindo linha

#include <avr/wdt.h>
#include "config/createObjects.h"
#include "routines/floor/calibrate.h"
#include "routines/catcher.h"
#include "routines/interruptMode.h"
#include "routines/floor/followLine.h"
#include "routines/rescue.h"
#include "config/buzzer.h"
#include "routines/printThings.h"

void setup()
{
    wdt_disable();

    rightTurnLED.on();
    greenLED.on();
    leftTurnLED.on();

    gyro.init();

    DebugInit(115200);

    startButton.waitForRelease(
        []() -> void
        {
            rightTurnLED.blink(200);
            greenLED.blink(200);
            leftTurnLED.blink(200);
        });

    startButton.waitForPressAndRelease(
        []() -> void
        {
            if (F1.isPressed() && F3.isPressed())
            {
                robot.turnOnMotors();
                runCalibration();
            }
            if (F2.isPressed())
            {
                toggleCatcher();
            }
            if (bumper.isPressed())
            {
                // modo balada
                redLedSensor.on();
                greenLedSensor.off();
                blueLedSensor.off();
                delay(200);
                redLedSensor.off();
                greenLedSensor.on();
                blueLedSensor.off();
                delay(200);
                redLedSensor.off();
                greenLedSensor.off();
                blueLedSensor.on();
                delay(200);
            }
            greenLED.blink(200);
            leftTurnLED.set(greenSensors[0].getGreen());
            rightTurnLED.set(greenSensors[1].getGreen());
        },
        []() -> void
        {
            rightTurnLED.blink(100);
            greenLED.blink(100);
            leftTurnLED.blink(100);
            if (bumper.isPressed())
            {
                redLedSensor.on();
                greenLedSensor.off();
                blueLedSensor.off();
                delay(200);
                redLedSensor.off();
                greenLedSensor.on();
                blueLedSensor.off();
                delay(200);
                redLedSensor.off();
                greenLedSensor.off();
                blueLedSensor.on();
                delay(200);
            }
        });

    motorLeft.on();
    motorRight.on();

    loadCalibrationSaved();

    rightTurnLED.off();
    greenLED.off();
    leftTurnLED.off();
    delay(300);

    if (catcher.pos == 0)
    {
        state = 2;
        layCatcher();
    }
    else
    {
        layCatcher();
        state = 1;
    }

    attachInterrupt(digitalPinToInterrupt(startButton.pin), interruptMenu, LOW);
    redLedSensor.off();
    greenLedSensor.off();
    blueLedSensor.off();
}

void debugLoop()
{
    buzzer();
    delay(500);
}

void loop()
{
#if DEBUG == 1
    debugLoop();
#else
    switch (state)
    {
    case 2:
        followRamp();
        if (checkFloor())
        {
            state = 3;
        }
        break;

    case 3:
        rescue();
        break;

    default:
        runFloor();
        break;
    }
#endif
}
