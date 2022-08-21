
void returnRoutine()
{
    robot.stop(5);
    robot.moveTime(-20, -20, 100);
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
    robot.moveCentimeters(8, 50);
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

    robot.moveTime(-20, -20, 400);
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

    if (borderRightLight < blackThreshold)
    {
        turnForce = turnForce;
        rightTurnLED.on();
    }
    else if (borderLeftLight < blackThreshold)
    {
        turnForce = -turnForce;
        leftTurnLED.on();
    }
    else
        return false;

    robot.stop(50);

    unsigned long timeout = 0;
    if (turnForce > 0)
    {
        timeout = millis() + 500;
        while (((lineSensors[5].getLight() > blackThreshold - 15 || lineSensors[4].getLight() > blackThreshold - 15) && (lineSensors[1].getLight() > blackThreshold - 15 || lineSensors[2].getLight() > blackThreshold - 15)) && millis() < timeout)
        {
            robot.move(15, 0);
        }
    }
    else
    {
        timeout = millis() + 500;
        while (((lineSensors[5].getLight() > blackThreshold - 15 || lineSensors[4].getLight() > blackThreshold - 15) && (lineSensors[1].getLight() > blackThreshold - 15 || lineSensors[2].getLight() > blackThreshold - 15)) && millis() < timeout)
        {
            robot.move(0, 15);
        }
    }

    robot.moveTime(20, 20, 100);
    robot.stop(50);

    readColors();
    if (checkGreen())
        return true;

    alignGreen();
    robot.moveTime(20, 20, 50);
    alignGreen();
    robot.stop(50);

    robot.stop(1500);

    readColors();
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
    robot.moveCentimeters(8, 70);
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

    robot.moveTime(-20, -20, 275);
    returnRoutine();
    return true;
}