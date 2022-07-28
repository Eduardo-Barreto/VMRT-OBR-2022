#define DEBUG 0
#define DEBUG_LOG 0

#if DEBUG_LOG == 1
#define DebugInit(x) Serial.begin(x)
#define DebugLog(x) Serial.print(String(x))
#define DebugLogln(x) Serial.println(String(x))
#else
#define DebugInit(x)
#define DebugLog(x)
#define DebugLogln(x)
#endif

volatile bool state = 1;            // Variável de estado (usada para debug)
int masterPower = 60;               // Controlador principal de base da velocidade
int maxPower = 75;                  // Velocidade máxima do robô ao seguir linha
int targetPower = masterPower + 10; // Velocidade atual do robô ao seguir linha

#include <avr/wdt.h>
#include "config/createObjects.h"
#include "routines/followLine.h"
#include "routines/calibrate.h"
#include "routines/catcher.h"
#include "routines/interruptMode.h"

void setup()
{
    wdt_disable();
    rightTurnLED.on();
    greenLED.on();
    leftTurnLED.on();
    gyro.init();
    DebugInit(115200);

    startButton.waitForRelease([]() -> void
                               {
            builtInLED.blink(200);
            rightTurnLED.blink(200);
            greenLED.blink(200);
            leftTurnLED.blink(200); });

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

    loadCalibrationSaved();
    layCatcher();
    builtInLED.off();
    rightTurnLED.off();
    greenLED.off();
    leftTurnLED.off();
    layCatcher();
    delay(300);

    attachInterrupt(digitalPinToInterrupt(startButton.pin), interruptMenu, LOW);
}

void debugLoop()
{
    int turnForce = -1;
    DebugLog(turnForce);
    robot.turn((turnForce < 0 ? 5 : -5), 60);
    F1.waitForPressAndRelease();
    turnForce = 1;
    DebugLog(turnForce);
    robot.turn((turnForce < 0 ? 5 : -5), 60);
    F1.waitForPressAndRelease();
}

void loop()
{
#if DEBUG == 1
    debugLoop();
#else
    runFloor();
#endif
}
