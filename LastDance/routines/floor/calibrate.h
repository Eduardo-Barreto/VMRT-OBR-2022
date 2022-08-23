#include <EEPROM.h>

struct calibration
{
    int minRead;
    int maxRead;

    int minGreen;
    int maxGreen;

    int minReadRed;
    int maxReadRed;
    int minReadGreen;
    int maxReadGreen;
    int minReadBlue;
    int maxReadBlue;
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

    EEPROM.get(address, calibrationRead);
    leftRGB.minRead = calibrationRead.minRead;
    leftRGB.maxRead = calibrationRead.maxRead;
    leftRGB.minGreen = calibrationRead.minGreen;
    leftRGB.maxGreen = calibrationRead.maxGreen;
    leftRGB.minReadRed = calibrationRead.minReadRed;
    leftRGB.maxReadRed = calibrationRead.maxReadRed;
    leftRGB.minReadGreen = calibrationRead.minReadGreen;
    leftRGB.maxReadGreen = calibrationRead.maxReadGreen;
    leftRGB.minReadBlue = calibrationRead.minReadBlue;
    leftRGB.maxReadBlue = calibrationRead.maxReadBlue;
    address += sizeof(calibrationRead);

    EEPROM.get(address, calibrationRead);
    rightRGB.minRead = calibrationRead.minRead;
    rightRGB.maxRead = calibrationRead.maxRead;
    rightRGB.minGreen = calibrationRead.minGreen;
    rightRGB.maxGreen = calibrationRead.maxGreen;
    rightRGB.minReadRed = calibrationRead.minReadRed;
    rightRGB.maxReadRed = calibrationRead.maxReadRed;
    rightRGB.minReadGreen = calibrationRead.minReadGreen;
    rightRGB.maxReadGreen = calibrationRead.maxReadGreen;
    rightRGB.minReadBlue = calibrationRead.minReadBlue;
    rightRGB.maxReadBlue = calibrationRead.maxReadBlue;
    address += sizeof(calibrationRead);
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

    calibrationWrite.minRead = leftRGB.minRead;
    calibrationWrite.maxRead = leftRGB.maxRead;
    calibrationWrite.minGreen = leftRGB.minGreen;
    calibrationWrite.maxGreen = leftRGB.maxGreen;
    calibrationWrite.minReadRed = leftRGB.minReadRed;
    calibrationWrite.maxReadRed = leftRGB.maxReadRed;
    calibrationWrite.minReadGreen = leftRGB.minReadGreen;
    calibrationWrite.maxReadGreen = leftRGB.maxReadGreen;
    calibrationWrite.minReadBlue = leftRGB.minReadBlue;
    calibrationWrite.maxReadBlue = leftRGB.maxReadBlue;
    EEPROM.put(address, calibrationWrite);
    address += sizeof(calibrationWrite);

    calibrationWrite.minRead = rightRGB.minRead;
    calibrationWrite.maxRead = rightRGB.maxRead;
    calibrationWrite.minGreen = rightRGB.minGreen;
    calibrationWrite.maxGreen = rightRGB.maxGreen;
    calibrationWrite.minReadRed = rightRGB.minReadRed;
    calibrationWrite.maxReadRed = rightRGB.maxReadRed;
    calibrationWrite.minReadGreen = rightRGB.minReadGreen;
    calibrationWrite.maxReadGreen = rightRGB.maxReadGreen;
    calibrationWrite.minReadBlue = rightRGB.minReadBlue;
    calibrationWrite.maxReadBlue = rightRGB.maxReadBlue;
    EEPROM.put(address, calibrationWrite);
    address += sizeof(calibrationWrite);
}

/**
 * @brief Calibra os sensores de luz para a iluminação atual
 */
void calibrateLineFollower()
{
    unsigned long start = millis();
    while (millis() - start < 1500)
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

        leftRGB.read();
        leftRGB.minRead = (leftRGB.raw < leftRGB.minRead) ? leftRGB.raw : leftRGB.minRead;
        leftRGB.maxRead = (leftRGB.raw > leftRGB.maxRead) ? leftRGB.raw : leftRGB.maxRead;
        rightRGB.read();
        rightRGB.minRead = (rightRGB.raw < rightRGB.minRead) ? rightRGB.raw : rightRGB.minRead;
        rightRGB.maxRead = (rightRGB.raw > rightRGB.maxRead) ? rightRGB.raw : rightRGB.maxRead;
    }
    delay(500);
    start = millis();
    while (millis() - start < 1500)
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

        leftRGB.read();
        leftRGB.minRead = (leftRGB.raw < leftRGB.minRead) ? leftRGB.raw : leftRGB.minRead;
        leftRGB.maxRead = (leftRGB.raw > leftRGB.maxRead) ? leftRGB.raw : leftRGB.maxRead;
        rightRGB.read();
        rightRGB.minRead = (rightRGB.raw < rightRGB.minRead) ? rightRGB.raw : rightRGB.minRead;
        rightRGB.maxRead = (rightRGB.raw > rightRGB.maxRead) ? rightRGB.raw : rightRGB.maxRead;
    }
}

void calibrateRGBSensor()
{
    redLedSensor.on();
    greenLedSensor.off();
    blueLedSensor.off();

    unsigned long timeout = millis() + 1500;
    while (millis() < timeout)
    {
        robot.move(20, 20);
        leftRGB.read();
        leftRGB.minReadRed = (leftRGB.raw < leftRGB.minReadRed) ? leftRGB.raw : leftRGB.minReadRed;
        leftRGB.maxReadRed = (leftRGB.raw > leftRGB.maxReadRed) ? leftRGB.raw : leftRGB.maxReadRed;
        rightRGB.read();
        rightRGB.minReadRed = (rightRGB.raw < rightRGB.minReadRed) ? rightRGB.raw : rightRGB.minReadRed;
        rightRGB.maxReadRed = (rightRGB.raw > rightRGB.maxReadRed) ? rightRGB.raw : rightRGB.maxReadRed;
    }
    timeout = millis() + 1500;
    while (millis() < timeout)
    {
        robot.move(-20, -20);
        leftRGB.read();
        leftRGB.minReadRed = (leftRGB.raw < leftRGB.minReadRed) ? leftRGB.raw : leftRGB.minReadRed;
        leftRGB.maxReadRed = (leftRGB.raw > leftRGB.maxReadRed) ? leftRGB.raw : leftRGB.maxReadRed;
        rightRGB.read();
        rightRGB.minReadRed = (rightRGB.raw < rightRGB.minReadRed) ? rightRGB.raw : rightRGB.minReadRed;
        rightRGB.maxReadRed = (rightRGB.raw > rightRGB.maxReadRed) ? rightRGB.raw : rightRGB.maxReadRed;
    }

    redLedSensor.off();
    greenLedSensor.on();
    blueLedSensor.off();

    timeout = millis() + 1500;
    while (millis() < timeout)
    {
        robot.move(20, 20);
        leftRGB.read();
        leftRGB.minReadGreen = (leftRGB.raw < leftRGB.minReadGreen) ? leftRGB.raw : leftRGB.minReadGreen;
        leftRGB.maxReadGreen = (leftRGB.raw > leftRGB.maxReadGreen) ? leftRGB.raw : leftRGB.maxReadGreen;
        rightRGB.read();
        rightRGB.minReadGreen = (rightRGB.raw < rightRGB.minReadGreen) ? rightRGB.raw : rightRGB.minReadGreen;
        rightRGB.maxReadGreen = (rightRGB.raw > rightRGB.maxReadGreen) ? rightRGB.raw : rightRGB.maxReadGreen;
    }
    timeout = millis() + 1500;
    while (millis() < timeout)
    {
        robot.move(-20, -20);
        leftRGB.read();
        leftRGB.minReadGreen = (leftRGB.raw < leftRGB.minReadGreen) ? leftRGB.raw : leftRGB.minReadGreen;
        leftRGB.maxReadGreen = (leftRGB.raw > leftRGB.maxReadGreen) ? leftRGB.raw : leftRGB.maxReadGreen;
        rightRGB.read();
        rightRGB.minReadGreen = (rightRGB.raw < rightRGB.minReadGreen) ? rightRGB.raw : rightRGB.minReadGreen;
        rightRGB.maxReadGreen = (rightRGB.raw > rightRGB.maxReadGreen) ? rightRGB.raw : rightRGB.maxReadGreen;
    }

    redLedSensor.off();
    greenLedSensor.off();
    blueLedSensor.on();

    timeout = millis() + 1500;
    while (millis() < timeout)
    {
        robot.move(20, 20);
        leftRGB.read();
        leftRGB.minReadBlue = (leftRGB.raw < leftRGB.minReadBlue) ? leftRGB.raw : leftRGB.minReadBlue;
        leftRGB.maxReadBlue = (leftRGB.raw > leftRGB.maxReadBlue) ? leftRGB.raw : leftRGB.maxReadBlue;
        rightRGB.read();
        rightRGB.minReadBlue = (rightRGB.raw < rightRGB.minReadBlue) ? rightRGB.raw : rightRGB.minReadBlue;
        rightRGB.maxReadBlue = (rightRGB.raw > rightRGB.maxReadBlue) ? rightRGB.raw : rightRGB.maxReadBlue;
    }
    timeout = millis() + 1500;
    while (millis() < timeout)
    {
        robot.move(-20, -20);
        leftRGB.read();
        leftRGB.minReadBlue = (leftRGB.raw < leftRGB.minReadBlue) ? leftRGB.raw : leftRGB.minReadBlue;
        leftRGB.maxReadBlue = (leftRGB.raw > leftRGB.maxReadBlue) ? leftRGB.raw : leftRGB.maxReadBlue;
        rightRGB.read();
        rightRGB.minReadBlue = (rightRGB.raw < rightRGB.minReadBlue) ? rightRGB.raw : rightRGB.minReadBlue;
        rightRGB.maxReadBlue = (rightRGB.raw > rightRGB.maxReadBlue) ? rightRGB.raw : rightRGB.maxReadBlue;
    }
}

void runCalibration()
{
    greenLED.off();
    rightTurnLED.on();
    leftTurnLED.on();
    redLedSensor.on();
    greenLedSensor.on();
    blueLedSensor.on();
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
    startButton.waitForPressAndRelease();
    calibrateRGBSensor();
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
            rightRGB.setGreen();
            blinkRight = false;
        }

        if (F3.isPressed())
        {
            greenSensors[0].setGreen();
            leftRGB.setGreen();
            blinkLeft = false;
        }
    }
    redLedSensor.off();
    greenLedSensor.off();
    blueLedSensor.off();

    greenLED.off();
    delay(150);
    saveCalibration();
}

void forceCalibrationFollower()
{
    F1.waitForPressAndRelease();
    for (byte i = 0; i < 7; i++)
    {
        lineSensors[i].minRead = lineSensors[i].getRawRead();
    }

    F1.waitForPressAndRelease();
    for (byte i = 0; i < 7; i++)
    {
        lineSensors[i].maxRead = lineSensors[i].getRawRead();
    }
    delay(150);
    saveCalibration();
}