#include <EEPROM.h>

struct calibration
{
    int minRead;
    int maxRead;

    int minGreen;
    int maxGreen;
};

void loadCalibrationSaved()
{
    int address = 0;
    calibration calibrationRead;

    for (byte i = 0; i < 7; i++)
    {
        EEPROM.get(address, calibrationRead);

        lineSensors[i].minRead = calibrationRead.minRead;
        lineSensors[i].maxRead = calibrationRead.maxRead;

        address += sizeof(calibrationRead);
    }

    for (byte i = 0; i < 2; i++)
    {
        EEPROM.get(address, calibrationRead);

        greenSensors[i].minRead = calibrationRead.minRead;
        greenSensors[i].maxRead = calibrationRead.maxRead;
        greenSensors[i].minGreen = calibrationRead.minGreen;
        greenSensors[i].maxGreen = calibrationRead.maxGreen;

        address += sizeof(calibrationRead);
    }
}

void saveCalibration()
{
    int address = 0;
    calibration calibrationWrite;

    for (byte i = 0; i < 7; i++)
    {
        calibrationWrite.minRead = lineSensors[i].minRead;
        calibrationWrite.maxRead = lineSensors[i].maxRead;

        EEPROM.put(address, calibrationWrite);

        address += sizeof(calibrationWrite);
    }

    for (byte i = 0; i < 2; i++)
    {
        calibrationWrite.minRead = greenSensors[i].minRead;
        calibrationWrite.maxRead = greenSensors[i].maxRead;
        calibrationWrite.minGreen = greenSensors[i].minGreen;
        calibrationWrite.maxGreen = greenSensors[i].maxGreen;

        EEPROM.put(address, calibrationWrite);

        address += sizeof(calibrationWrite);
    }
}

/**
 * @brief Calibra os sensores de luz para a iluminação atual
 */
void calibrateLineFollower()
{
    unsigned long start = millis();
    while (millis() - start < 1000)
    {
        robot.move(20, 20);
        for (byte i = 0; i < 7; i++)
        {
            lineSensors[i].read();
            lineSensors[i].minRead = (lineSensors[i].raw < lineSensors[i].minRead) ? lineSensors[i].raw : lineSensors[i].minRead;
            lineSensors[i].maxRead = (lineSensors[i].raw > lineSensors[i].maxRead) ? lineSensors[i].raw : lineSensors[i].maxRead;
        }
        for (byte j = 0; j < 2; j++)
        {
            greenSensors[j].read();
            greenSensors[j].minRead = (greenSensors[j].raw < greenSensors[j].minRead) ? greenSensors[j].raw : greenSensors[j].minRead;
            greenSensors[j].maxRead = (greenSensors[j].raw > greenSensors[j].maxRead) ? greenSensors[j].raw : greenSensors[j].maxRead;
        }
    }
    delay(500);
    start = millis();
    while (millis() - start < 1000)
    {
        robot.move(-20, -20);
        for (byte i = 0; i < 7; i++)
        {
            lineSensors[i].read();
            lineSensors[i].minRead = (lineSensors[i].raw < lineSensors[i].minRead) ? lineSensors[i].raw : lineSensors[i].minRead;
            lineSensors[i].maxRead = (lineSensors[i].raw > lineSensors[i].maxRead) ? lineSensors[i].raw : lineSensors[i].maxRead;
        }
        for (byte j = 0; j < 2; j++)
        {
            greenSensors[j].read();
            greenSensors[j].minRead = (greenSensors[j].raw < greenSensors[j].minRead) ? greenSensors[j].raw : greenSensors[j].minRead;
            greenSensors[j].maxRead = (greenSensors[j].raw > greenSensors[j].maxRead) ? greenSensors[j].raw : greenSensors[j].maxRead;
        }
    }
}

void printCalibrationSaved()
{
    int address = 0;
    calibration calibrationRead;

    for (byte i = 0; i < 7; i++)
    {
        EEPROM.get(address, calibrationRead);

        DebugLog("Sensor ");
        DebugLog(i);
        DebugLog(": ");
        DebugLog(calibrationRead.minRead);
        DebugLog(" ~ ");
        DebugLogln(calibrationRead.maxRead);

        address += sizeof(calibrationRead);
    }

    DebugLogln();

    for (byte i = 0; i < 2; i++)
    {
        EEPROM.get(address, calibrationRead);

        DebugLog("Sensor verde ");
        DebugLog(i);
        DebugLog(": ");
        DebugLog(calibrationRead.minRead);
        DebugLog(" ~ ");
        DebugLogln(calibrationRead.maxRead);

        DebugLog("\tValor verde ");
        DebugLog(i);
        DebugLog(": ");
        DebugLog(calibrationRead.minGreen);
        DebugLog(" ~ ");
        DebugLogln(calibrationRead.maxGreen);

        address += sizeof(calibrationRead);
    }
}

void printCalibrationFollower()
{
    // print all calibration for lineSensors
    for (byte i = 0; i < 7; i++)
    {
        DebugLog("lineSensors[");
        DebugLog(i);
        DebugLog("].minRead = ");
        DebugLog(lineSensors[i].minRead);
        DebugLogln(";");
        DebugLog("lineSensors[");
        DebugLog(i);
        DebugLog("].maxRead = ");
        DebugLog(lineSensors[i].maxRead);
        DebugLogln(";");
    }
    for (byte j = 0; j < 2; j++)
    {
        DebugLog("greenSensors[");
        DebugLog(j);
        DebugLog("].minRead = ");
        DebugLog(greenSensors[j].minRead);
        DebugLogln(";");
        DebugLog("greenSensors[");
        DebugLog(j);
        DebugLog("].maxRead = ");
        DebugLog(greenSensors[j].maxRead);
        DebugLogln(";");
    }

    for (byte k = 0; k < 2; k++)
    {
        DebugLog("greenSensors[");
        DebugLog(k);
        DebugLog("].minGreen = ");
        DebugLog(greenSensors[k].minGreen);
        DebugLogln(";");
        DebugLog("greenSensors[");
        DebugLog(k);
        DebugLog("].maxGreen = ");
        DebugLog(greenSensors[k].maxGreen);
        DebugLogln(";");
    }
}

void runCalibration()
{
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

    bool blinkRight = true;
    bool blinkLeft = true;
    while (startButton.isReleased())
    {
        greenLED.blink(200);
        if (blinkLeft)
        {
            if (greenSensors[0].getLight() > 20 && greenSensors[0].getLight() <= 32)
                leftTurnLED.on();
            else
                leftTurnLED.off();
        }
        else
            leftTurnLED.off();

        if (blinkRight)
        {
            if (greenSensors[1].getLight() > 20 && greenSensors[1].getLight() <= 32)
                rightTurnLED.on();
            else
                rightTurnLED.off();
        }
        else
            rightTurnLED.off();

        if (F1.isPressed())
        {
            greenSensors[1].setGreen();
            blinkRight = false;
        }

        if (F3.isPressed())
        {
            greenSensors[0].setGreen();
            blinkLeft = false;
        }
    }

    greenLED.off();
    delay(150);
    saveCalibration();
}

void printRawCalibration()
{
    for (byte i = 0; i < 7; i++)
    {
        DebugLog(lineSensors[i].minRead);
        DebugLogln("~");
        DebugLog(lineSensors[i].maxRead);
        DebugLog(",");
    }
    DebugLog(greenSensors[0].minRead);
    DebugLogln("~");
    DebugLog(greenSensors[0].maxRead);
    DebugLog(",");
    DebugLog(greenSensors[1].minRead);
    DebugLogln("~");
    DebugLogln(greenSensors[1].maxRead);
}