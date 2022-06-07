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
    pinMode(LED_BUILTIN, OUTPUT);
    gyro.init();
#if CALIBRATE_LINE_SENSORS == 1
    calibrateLineFollower();
    delay(500);
    calibrateLineFollower();
    delay(500);
#else
    lineSensors[0].minRead = 384;
    lineSensors[0].maxRead = 947;
    lineSensors[1].minRead = 119;
    lineSensors[1].maxRead = 917;
    lineSensors[2].minRead = 241;
    lineSensors[2].maxRead = 936;
    lineSensors[3].minRead = 437;
    lineSensors[3].maxRead = 960;
    lineSensors[4].minRead = 141;
    lineSensors[4].maxRead = 927;
    lineSensors[5].minRead = 82;
    lineSensors[5].maxRead = 920;
    lineSensors[6].minRead = 43;
    lineSensors[6].maxRead = 896;
    // TODO: Calibrar sensores do verde
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
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(100);
    }
    while (digitalRead(38) == 0)
    {
    }
    greenRight.setGreen();
    greenLeft.setGreen();

    delay(3000);

    motorLeft.on();
    motorRight.on();
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
    if (greenLeft.getGreen() || greenRight.getGreen())
    {
        delay(100);
    }
#endif
}
