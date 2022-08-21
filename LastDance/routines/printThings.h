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

void printAllLightSensors(bool ignoreGreen = false)
{
    for (byte i = 0; i < 7; i++)
    {
        DebugLog(lineSensors[i].getLight());
        DebugLog("\t");
    }

    if (ignoreGreen)
        return;

    for (byte j = 0; j < 2; j++)
    {
        DebugLog(greenSensors[j].getLight());
        DebugLog("\t");
    }
}

void printAllRawReads(bool ignoreGreen = false)
{
    for (byte i = 0; i < 7; i++)
    {
        DebugLog(lineSensors[i].getRawRead());
        DebugLog("\t");
    }

    if (ignoreGreen)
        return;

    for (byte j = 0; j < 2; j++)
    {
        DebugLog(greenSensors[j].getRawRead());
        DebugLog("\t");
    }
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

void printRGBValues()
{
    DebugLog("rgb(");
    DebugLog(leftRGB.getRGBRed() * 255);
    DebugLog(", ");
    DebugLog(leftRGB.getRGBGreen() * 255);
    DebugLog(", ");
    DebugLog(leftRGB.getRGBBlue() * 255);
    DebugLog(")");

    DebugLog("\t");

    DebugLog("rgb(");
    DebugLog(rightRGB.getRGBRed() * 255);
    DebugLog(", ");
    DebugLog(rightRGB.getRGBGreen() * 255);
    DebugLog(", ");
    DebugLog(rightRGB.getRGBBlue() * 255);
    DebugLog(")");
}