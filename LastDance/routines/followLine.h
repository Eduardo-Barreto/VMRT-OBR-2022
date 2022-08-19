int borderThreshold = 80; // Valor mínimo para considerar que o sensor está diferente do seu oposto (direita-esquerda)
int blackThreshold = 19;  // Valor máximo para considerar que o sensor está lendo preto

byte centerLight;      // Valor lido do sensor de luz do meio
byte centerRightLight; // Valor lido do sensor de luz do meio da direita
byte centerLeftLight;  // Valor lido do sensor de luz do meio da esquerda
byte rightLight;       // Valor lido do sensor de luz da direita
byte leftLight;        // Valor lido do sensor de luz da esquerda
byte borderRightLight; // Valor lido do sensor de luz da borda da direita
byte borderLeftLight;  // Valor lido do sensor de luz da borda da esquerda

bool centerBlack;      // Indica se o valor do sensor de luz do meio é preto
bool centerRightBlack; // Indica se o valor do sensor de luz do meio da direita é preto
bool centerLeftBlack;  // Indica se o valor do sensor de luz do meio da esquerda é preto
bool rightBlack;       // Indica se o valor do sensor de luz da direita é preto
bool leftBlack;        // Indica  se o valor do sensor de luz da esquerda é preto
bool borderRightBlack; // Indica se o valor do sensor de luz da borda da direita é preto
bool borderLeftBlack;  // Indica se o valor do sensor de luz da borda da esquerda é preto
bool rightGreen;       // Indica se existe verde na direita
bool leftGreen;        // Indica se existe verde na esquerda

int centerDiff; // Diferença entre os valores lido do sensor de luz da direita e da esquerda
int borderDiff; // Diferença entre os valores lido do sensor de luz da borda da direita e da borda da esquerda

unsigned long lastCorrection;        // Variável para armazenar o tempo do último ajuste
unsigned long incrementVelocityTime; // Variável para armazenar o tempo do próximo ajuste de velocidade

void readAllLightSensors()
{
    for (byte i = 0; i < 7; i++)
    {
        lineSensors[i].read();
    }
    for (byte j = 0; j < 2; j++)
    {
        greenSensors[j].read();
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

/**
 * @brief Atualiza os valores lido dos sensores de luz e as variáveis de controle
 */
void readColors(int thresholdDiff = 0)
{
    borderLeftLight = lineSensors[0].getLight();
    leftLight = lineSensors[1].getLight();
    centerLeftLight = lineSensors[2].getLight();
    centerLight = lineSensors[3].getLight();
    centerRightLight = lineSensors[4].getLight();
    rightLight = lineSensors[5].getLight();
    borderRightLight = lineSensors[6].getLight();

    borderLeftBlack = lineSensors[0].getLight() < blackThreshold + thresholdDiff;
    leftBlack = lineSensors[1].getLight() < blackThreshold + thresholdDiff;
    centerLeftBlack = lineSensors[2].getLight() < blackThreshold + thresholdDiff;
    centerBlack = lineSensors[3].getLight() < blackThreshold + thresholdDiff;
    centerRightBlack = lineSensors[4].getLight() < blackThreshold + thresholdDiff;
    rightBlack = lineSensors[5].getLight() < blackThreshold + thresholdDiff;
    borderRightBlack = lineSensors[6].getLight() < blackThreshold + thresholdDiff;

    leftGreen = greenSensors[0].getGreen();
    rightGreen = greenSensors[1].getGreen();

    centerDiff = (centerRightLight + rightLight * 1.15) - (centerLeftLight + leftLight * 1.15);
    borderDiff = borderRightLight - borderLeftLight;
}

void alignLine(int force = 50, int _timeout = 750)
{
    unsigned long timeout = millis() + 60;
    while ((centerLeftBlack || leftBlack) && millis() < timeout)
    {
        if (centerRightBlack || rightBlack)
            break;
        robot.move(80, -80);
    }

    timeout = millis() + 60;
    while ((centerRightBlack || rightBlack) && millis() < timeout)
    {
        if (centerLeftBlack || leftBlack)
            break;
        robot.move(80, -80);
    }
}

void alignGreen()
{
    unsigned long timeout = millis() + 200;
    while (greenSensors[0].getLight() < blackThreshold && millis() < timeout)
    {
        robot.move(18, -18);
    }

    timeout = millis() + 200;
    while (greenSensors[1].getLight() < blackThreshold && millis() < timeout)
    {
        robot.move(-18, 18);
    }
}

void runLineFollowerGreenSensors()
{
    int greenDiff = (greenSensors[0].getLight() - greenSensors[1].getLight());

    if (greenDiff > borderThreshold)
    {
        robot.move(-targetPower, targetPower);
    }
    else if (greenDiff < -borderThreshold)
    {
        robot.move(targetPower, -targetPower);
    }
    else
    {
        robot.move(targetPower, targetPower);
    }
}

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
        timeout = millis() + 300;
        while (((lineSensors[5].getLight() > blackThreshold - 10 || lineSensors[4].getLight() > blackThreshold - 10) && (lineSensors[1].getLight() > blackThreshold - 10 || lineSensors[2].getLight() > blackThreshold - 10)) && millis() < timeout)
        {
            robot.move(15, 0);
        }
    }
    else
    {
        timeout = millis() + 300;
        while (((lineSensors[5].getLight() > blackThreshold - 10 || lineSensors[4].getLight() > blackThreshold - 10) && (lineSensors[1].getLight() > blackThreshold - 10 || lineSensors[2].getLight() > blackThreshold - 10)) && millis() < timeout)
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
    robot.stop(50);

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

/**
 * @brief Segue a linha
 *
 * @param ignoreUltra: Se deve ignorar leituras do sensor ultrassônico enquanto segue a linha
 */
void runLineFollower(bool ignoreUltra = false, bool ignoreTurns = false)
{
    if (targetPower < maxPower && millis() > incrementVelocityTime)
    {
        incrementVelocityTime = millis() + 50;
        targetPower++;
    }

    if (centerLeftBlack || leftBlack)
    {
        unsigned long timeout = millis() + 500;
        while ((!centerBlack || centerLeftBlack || leftBlack) && millis() < timeout)
        {
            if (centerRightBlack || rightBlack || (!ignoreUltra && centerUltra.read() < 20))
                break;
            readColors(15);
            if (!ignoreTurns)
                checkTurn();
            robot.move(turnPower, -turnPower);
            lastCorrection = millis();
            targetPower = masterPower;
        }
    }
    if (centerRightBlack || rightBlack)
    {
        unsigned long timeout = millis() + 500;
        while ((!centerBlack || centerRightBlack || rightBlack) && millis() < timeout)
        {
            if (centerLeftBlack || leftBlack || (!ignoreUltra && centerUltra.read() < 20))
                break;
            readColors(15);
            if (!ignoreTurns)
                checkTurn();
            robot.move(-turnPower, turnPower);
            lastCorrection = millis();
            targetPower = masterPower;
        }
    }

    robot.moveTime(targetPower, targetPower, 15, false);
}

void checkObstacle()
{
    if (centerUltra.read() < 20)
    {
        unsigned long timeout = millis() + 1500;
        while (centerUltra.read() > 7)
        {
            targetPower = 60;
            readColors();
            checkTurn();
            runLineFollower(true);
            if (millis() > timeout)
            {
                return;
            }
        }

        alignLine();
        robot.alignUltra(20, 7, 5);
        alignLine();
        robot.alignUltra(20, 7, 5);
        robot.stop();

        gyro.read();
        for (int angle : ortogonals)
        {
            if (proximity(gyro.Yaw, angle, 5))
            {
                robot.alignAngle();
            }
        }
        robot.stop(10);
        robot.turn(90, 60);
        robot.stop(100);
        unsigned long maxSteps = motorRight.motorSteps + robot.centimetersToSteps(5);
        while (leftUltra.read() > 30)
        {
            robot.move(-20, -20);
            if (motorRight.motorSteps > maxSteps)
                break;
        }
        robot.stop(100);

        maxSteps = motorRight.motorSteps + robot.centimetersToSteps(20);
        while (leftUltra.read() <= 30)
        {
            robot.move(20, 20);
            if (motorRight.motorSteps > maxSteps)
                break;
        }
        robot.moveCentimeters(10, 60);
        robot.stop(10);
        robot.turnOneWheel(-90, 85);
        robot.stop(100);

        maxSteps = motorRight.motorSteps + robot.centimetersToSteps(25);
        while (leftUltra.read() > 30)
        {
            robot.move(30, 30);
            if (motorRight.motorSteps > maxSteps)
                break;
        }
        while (leftUltra.read() < 30)
        {
            robot.move(30, 30);
            if (motorRight.motorSteps > maxSteps)
                break;
        }

        robot.moveCentimeters(12, 60);
        robot.stop(10);
        robot.turnOneWheel(-90, 85);
        robot.stop(10);

        maxSteps = motorRight.motorSteps + robot.centimetersToSteps(10);
        while (lineSensors[3].getLight() > blackThreshold + 10)
        {
            robot.move(20, 20);
            if (motorRight.motorSteps > maxSteps)
            {
                break;
            }
        }
        robot.moveCentimeters(8, 60);
        robot.stop(10);
        robot.turn(90, 60);
        robot.stop(10);
        alignLine();

        unsigned long stepOut = motorRight.motorSteps + robot.centimetersToSteps(6);
        while (bumper.isReleased())
        {
            robot.move(-15, -15);
            if (motorRight.motorSteps > stepOut)
                break;
        }
        alignLine();
    }
}

void followRamp()
{
    readColors();

    unsigned long timeout = millis() + 500;
    while (centerLeftBlack && millis() < timeout)
    {
        readColors();
        if (borderLeftBlack || borderRightBlack)
            break;
        robot.move(turnPower, -turnPower);
        lastCorrection = millis();
        targetPower = masterPower;
    }
    timeout = millis() + 500;
    while (centerRightBlack && millis() < timeout)
    {
        readColors();
        if (borderLeftBlack || borderRightBlack)
            break;
        robot.move(-turnPower, turnPower);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    robot.moveTime(targetPower, targetPower, 15);
}

#include "inclination.h"

void runFloor()
{
    readColors();
    checkTurn();
    runLineFollower();
    if (millis() > 300)
    {
        checkObstacle();
        gyro.read();
        if (checkRamp())
        {
            detachInterrupt(digitalPinToInterrupt(startButton.pin));
            lowerCatcher();
            state = 2;
            return;
        }
    }
}