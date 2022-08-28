int lastRightReads[350];
int lastLeftReads[350];

bool deuGreen()
{

    float leftBias = 0.5f;
    float rightBias = 0.5f;

    long rightGreenCount = 0;
    long leftGreenCount = 0;
    long rightCount = 0;
    long leftCount = 0;

    int i = 0;
    for (i = 0; i < 350; i++)
    {
        lastRightReads[i] = 1024;
        lastLeftReads[i] = 1024;
    }

    delay(500);
    i = 0;

    while (greenSensors[0].getLight() > 5 && greenSensors[1].getLight() > 5)
    {
        if (i >= 340)
            break;
        robot.move(10, 10);

        lastRightReads[i] = greenSensors[1].getRawRead();
        lastLeftReads[i] = greenSensors[0].getRawRead();
        i++;
    }
    delay(500);

    for (int read : lastRightReads)
    {
        if (read == 1024)
            break;
        rightCount++;
        if (greenSensors[1].checkGreen(read))
            rightGreenCount++;
    }

    for (int read : lastLeftReads)
    {
        if (read == 1024)
            break;
        leftCount++;
        if (greenSensors[0].checkGreen(read))
            leftGreenCount++;
    }

    leftGreen = leftGreenCount >= (leftCount * leftBias);
    rightGreen = rightGreenCount >= (rightCount * rightBias);

    // count * bias = x < greencount
    DebugLog(leftCount);
    DebugLog(" * ");
    DebugLog(leftBias);
    DebugLog(" = ");
    DebugLog(leftCount * leftBias);
    DebugLog(" < ");
    DebugLog(leftGreenCount);
    DebugLog(" = ");
    DebugLogln(leftGreen);

    DebugLog(rightCount);
    DebugLog(" * ");
    DebugLog(rightBias);
    DebugLog(" = ");
    DebugLog(rightCount * rightBias);
    DebugLog(" < ");
    DebugLog(rightGreenCount);
    DebugLog(" = ");
    DebugLogln(rightGreen);
    DebugLogln("---");
}

void returnRoutine()
{
    robot.stop(5);
    robot.stop(50);
    alignLine();
    targetPower = masterPower - 5;
    lastCorrection = millis();
    greenSensors[0].green = false;
    greenSensors[1].green = false;
    rightTurnLED.off();
    greenLED.off();
    leftTurnLED.off();
}

bool checkDeadEnd(int turnForce = 60)
{
    if (rightGreen && leftGreen)
    {
        rightTurnLED.on();
        greenLED.on();
        leftTurnLED.on();

        robot.stop();

        robot.moveCentimeters(7, 50);

        robot.turn(175, turnForce);

        while (lineSensors[3].getLight() > blackThreshold + 10)
        {
            robot.move(-turnForce, turnForce);
        }

        robot.moveTime(-20, -20, 400);
        robot.stop(50);
        returnRoutine();
        return true;
    }
    else
    {
        return false;
    }
}

bool checkGreen(int turnForce = 60)
{
    if (checkDeadEnd())
        return true;

    if (rightGreen)
    {
        turnForce = turnForce;
        leftTurnLED.off();
        rightTurnLED.on();
        greenLED.on();
    }
    else if (leftGreen)
    {
        turnForce = -turnForce;
        leftTurnLED.on();
        rightTurnLED.off();
        greenLED.on();
    }
    else
        return false;

    if (checkDeadEnd())
        return true;

    robot.alignAngle();

    readColors();
    if (checkDeadEnd())
        return true;

    delay(300);
    robot.moveCentimeters(5, 50);
    robot.turn(((turnForce > 0) ? (85) : (-85)), 60);

    int targetAngle = gyro.Yaw + (10 * (turnForce / abs(turnForce)));
    while (lineSensors[3].getLight() > blackThreshold + 10)
    {
        robot.move(-turnForce, turnForce);
        gyro.read();
        if (proximity(gyro.Yaw, targetAngle, 1.5f))
        {
            break;
        }
    }

    // robot.moveTime(-20, -20, 400);
    robot.stop(50);
    returnRoutine();
    return true;
}

/**
 * @brief Busca por curvas
 *
 * @param curveForce: Força aplicada ao robo na curva
 */
bool checkTurn(int turnForce = 60)
{

    if (borderRightBlack)
    {
        turnForce = turnForce;
        rightTurnLED.on();
    }
    else if (borderLeftBlack)
    {
        turnForce = -turnForce;
        leftTurnLED.on();
    }
    else
        return false;

    leftGreen = false;
    rightGreen = false;

    robot.stop(50);

    unsigned long timeout = 0;
    if (turnForce > 0)
    {
        timeout = millis() + 500;
        while (((lineSensors[5].getLight() > blackThreshold - 15 || lineSensors[4].getLight() > blackThreshold - 15) && (lineSensors[1].getLight() > blackThreshold - 15 || lineSensors[2].getLight() > blackThreshold - 15)) && millis() < timeout)
        {
            robot.move(-15, 15);
        }
    }
    else
    {
        timeout = millis() + 500;
        while (((lineSensors[5].getLight() > blackThreshold - 15 || lineSensors[4].getLight() > blackThreshold - 15) && (lineSensors[1].getLight() > blackThreshold - 15 || lineSensors[2].getLight() > blackThreshold - 15)) && millis() < timeout)
        {
            robot.move(15, -15);
        }
    }
    robot.stop(50);

    leftGreen = greenSensors[0].getGreen();
    rightGreen = greenSensors[1].getGreen();
    deuGreen();

    if (checkGreen())
        return true;

    if ((borderRightLight < blackThreshold) && (borderLeftLight < blackThreshold))
    {
        robot.moveCentimeters(4, 60);
        rightTurnLED.off();
        leftTurnLED.off();
        return false;
    }

    robot.turn((turnForce < 0 ? 10 : -10), 60);
    robot.moveCentimeters(4, 70);
    gyro.read();
    int targetAngle = gyro.Yaw + (95 * (turnForce / abs(turnForce)));

    while (lineSensors[3].getLight() > blackThreshold + 10)
    {
        robot.move(-turnForce, turnForce);
        gyro.read();
        if (proximity(gyro.Yaw, targetAngle, 1.5f))
        {
            break;
        }
    }

    // robot.moveTime(-20, -20, 275);
    returnRoutine();
    return true;
}