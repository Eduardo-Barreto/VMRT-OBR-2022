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

int masterPower = 95;
int maxPower = 80;
int targetPower = masterPower + 10;
int turnPower = masterPower;

#include "config/createObjects.h"
#include "routines/followLine.h"

void setup()
{
    gyro.init();
#if CALIBRATE_LINE_SENSORS == 1
    calibrateLineFollower();
    delay(500);
    calibrateLineFollower();
    delay(500);
#else
    lineSensors[0].minRead = 561;
    lineSensors[0].maxRead = 959;
    lineSensors[1].minRead = 365;
    lineSensors[1].maxRead = 940;
    lineSensors[2].minRead = 442;
    lineSensors[2].maxRead = 932;
    lineSensors[3].minRead = 616;
    lineSensors[3].maxRead = 972;
    lineSensors[4].minRead = 410;
    lineSensors[4].maxRead = 946;
    lineSensors[5].minRead = 355;
    lineSensors[5].maxRead = 935;
    lineSensors[6].minRead = 297;
    lineSensors[6].maxRead = 908;
#endif
    delay(1500);
    DebugInit(115200);

    // Pinos invadidos pelo giroscópio
    pinMode(16, INPUT);
    pinMode(17, INPUT);
    pinMode(18, INPUT);
    pinMode(19, INPUT);

    // Botão start improvisado
    pinMode(38, INPUT_PULLUP);
    while (digitalRead(38) == 1)
    {
    }
    while (digitalRead(38) == 0)
    {
    }
    delay(500);
}

void debugLoop()
{
}

void loop()
{
#if DEBUG == 1
    debugLoop();
#else
    runLineFollower();
#endif
}
