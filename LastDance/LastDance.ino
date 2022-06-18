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
    pinMode(LED_BUILTIN, OUTPUT);
    // Pinos invadidos pelo giroscópio
    pinMode(16, INPUT);
    pinMode(17, INPUT);
    pinMode(18, INPUT);
    pinMode(19, INPUT);
    gyro.init();

#if CALIBRATE_LINE_SENSORS == 1
    motorLeft.on();
    motorRight.on();
    calibrateLineFollower();
    delay(500);
    calibrateLineFollower();
    delay(500);
#else
    loadCalibrationSaved();

#endif
    delay(1500);
    DebugInit(115200);

    // Botão start improvisado
    pinMode(38, INPUT_PULLUP);
    while (digitalRead(38) == 1)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(100);
    }
    while (digitalRead(38) == 0)
    {
    }
#if CALIBRATE_LINE_SENSORS
    greenSensors[0].setGreen();
    greenSensors[1].setGreen();
    delay(150);

    saveCalibration();
#endif

    delay(750);

    motorLeft.on();
    motorRight.on();
}

void debugLoop()
{
    motorLeft.off();
    motorRight.off();

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
