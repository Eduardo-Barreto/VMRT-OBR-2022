int borderThreshold = 80; // Valor mínimo para considerar que o sensor está diferente do seu oposto (direita-esquerda)
int blackThreshold = 20;  // Valor máximo para considerar que o sensor está lendo preto

byte centerRightLight; // Valor lido do sensor de luz do meio da direita
byte centerLeftLight;  // Valor lido do sensor de luz do meio da esquerda
byte rightLight;       // Valor lido do sensor de luz da direita
byte leftLight;        // Valor lido do sensor de luz da esquerda
byte borderRightLight; // Valor lido do sensor de luz da borda da direita
byte borderLeftLight;  // Valor lido do sensor de luz da borda da esquerda
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

/**
 * @brief Atualiza os valores lido dos sensores de luz e as variáveis de controle
 */
void readColors()
{
    borderLeftLight = lineSensors[0].getLight();
    leftLight = lineSensors[1].getLight();
    centerLeftLight = lineSensors[2].getLight();
    centerRightLight = lineSensors[4].getLight();
    rightLight = lineSensors[5].getLight();
    borderRightLight = lineSensors[6].getLight();

    leftGreen = greenSensors[0].getGreen();
    rightGreen = greenSensors[1].getGreen();

    centerDiff = (centerRightLight + rightLight * 1.15) - (centerLeftLight + leftLight * 1.15);
    borderDiff = borderRightLight - borderLeftLight;
}

void alignLine(int force = 50, int _timeout = 750)
{
    char diff = (lineSensors[2].getLight() - lineSensors[4].getLight());

    unsigned long timeout = millis() + _timeout;

    while ((diff > borderThreshold) && (millis() < timeout))
    {
        if (greenSensors[0].getGreen() || greenSensors[1].getGreen())
            break;

        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(-force, force);
    }

    timeout = millis() + _timeout;
    while ((diff < -borderThreshold) && (millis() < timeout))
    {
        if (greenSensors[0].getGreen() || greenSensors[1].getGreen())
            break;

        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(force, -force);
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

bool checkGreen(int turnForce = 80)
{
    if (rightGreen)
        turnForce = turnForce;
    else if (leftGreen)
        turnForce = -turnForce;
    else
        return false;

    delay(300);

    robot.moveCentimeters(7, 50);
    delay(150);
    robot.turn(((turnForce > 0) ? (15) : (-15)), 60);

    while (lineSensors[3].getLight() > blackThreshold)
    {
        robot.move(-turnForce, turnForce);
    }

    alignLine();
    targetPower = masterPower - 5;
    lastCorrection = millis();
    greenSensors[0].green = false;
    greenSensors[0].countGreen = 0;
    greenSensors[1].green = false;
    greenSensors[1].countGreen = 0;
    return true;
}

/**
 * @brief Busca por curvas
 *
 * @param curveForce: Força aplicada ao robo na curva
 */
bool checkTurn(int turnForce = 90)
{
    // if (checkGreen())
    //  return true;

    if ((borderRightLight < blackThreshold) && (borderLeftLight < blackThreshold))
    {
        robot.moveCentimeters(3, targetPower);
        return false;
    }

    if (borderRightLight < blackThreshold)
        turnForce = turnForce;
    else if (borderLeftLight < blackThreshold)
        turnForce = -turnForce;
    else
        return false;

    robot.stop(100);
    while ((lineSensors[0].getLight() > 20) && (lineSensors[6].getLight() > 20))
        robot.move(-15, -15);
    robot.stop(500);

    if (checkGreen())
        return true;

    robot.turn((turnForce < 0 ? 5 : -5), 60);
    robot.moveCentimeters(4, 70);

    readColors();
    while (lineSensors[3].getLight() > blackThreshold)
    {
        robot.move(-turnForce, turnForce);
    }

    alignLine();
    targetPower = masterPower - 5;
    lastCorrection = millis();
    return true;
}

/**
 * @brief Segue a linha
 *
 * @param checkForTurns: Se deve verificar por curvas ou somente seguir a linha
 */
void runLineFollower()
{
    if (targetPower < maxPower && millis() > incrementVelocityTime)
    {
        incrementVelocityTime = millis() + 50;
        targetPower++;
    }

    unsigned long timeout = millis() + 50;
    while ((centerLeftLight < blackThreshold) && millis() < timeout)
    {
        readColors();
        checkTurn();
        robot.move(80, -80);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    timeout = millis() + 50;
    while ((centerRightLight < blackThreshold) && millis() < timeout)
    {
        readColors();
        checkTurn();
        robot.move(-80, 80);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    timeout = millis() + 60;
    while ((leftLight < blackThreshold) && millis() < timeout)
    {
        readColors();
        checkTurn();
        robot.move(80, -80);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    timeout = millis() + 60;
    while ((rightLight < blackThreshold) && millis() < timeout)
    {
        readColors();
        checkTurn();
        robot.move(-80, 80);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    robot.moveTime(targetPower, targetPower, 15);
}

void runFloor()
{
    readColors();
    // checkGreen();
    checkTurn();
    runLineFollower();
}