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

    borderLeftBlack = borderLeftLight < blackThreshold + thresholdDiff;
    leftBlack = leftLight < blackThreshold + thresholdDiff;
    centerLeftBlack = centerLeftLight < blackThreshold + thresholdDiff;
    centerBlack = centerLight < blackThreshold + thresholdDiff;
    centerRightBlack = centerRightLight < blackThreshold + thresholdDiff;
    rightBlack = rightLight < blackThreshold + thresholdDiff;
    borderRightBlack = borderRightLight < blackThreshold + thresholdDiff;

    leftGreen = greenSensors[0].getGreen();
    rightGreen = greenSensors[1].getGreen();
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
    unsigned long timeout = millis() + 500;
    while (greenSensors[0].getLight() < blackThreshold - 15 && millis() < timeout)
    {
        robot.move(18, -18);
    }

    timeout = millis() + 500;
    while (greenSensors[1].getLight() < blackThreshold - 15 && millis() < timeout)
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

#include "crosspath.h"

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

#include "obstacle.h"

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