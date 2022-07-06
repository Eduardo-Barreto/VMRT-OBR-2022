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

/**
 * @brief Atualiza os valores lido dos sensores de luz e as variáveis de controle
 */
void readColors()
{
    borderRightLight = lineSensors[0].getLight();
    rightLight = lineSensors[1].getLight();
    centerRightLight = lineSensors[2].getLight();
    centerLeftLight = lineSensors[4].getLight();
    leftLight = lineSensors[5].getLight();
    borderLeftLight = lineSensors[6].getLight();

    rightGreen = greenSensors[0].getGreen();
    leftGreen = greenSensors[1].getGreen();

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
        robot.move(force, -force);
    }

    timeout = millis() + _timeout;
    while ((diff < -borderThreshold) && (millis() < timeout))
    {
        if (greenSensors[0].getGreen() || greenSensors[1].getGreen())
            break;

        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(-force, force);
    }
}

void runLineFollowerGreenSensors()
{
    int greenDiff = (greenSensors[1].getLight() - greenSensors[0].getLight());

    if (greenDiff > borderThreshold)
    {
        robot.move(targetPower, -targetPower);
    }
    else if (greenDiff < -borderThreshold)
    {
        robot.move(-targetPower, targetPower);
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

    alignLine();

    robot.moveCentimeters(10, 70);
    robot.turn((turnForce > 0 ? 15 : -15), turnForce);

    while (lineSensors[3].getLight() > blackThreshold)
    {
        robot.move(turnForce, -turnForce);
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
        return;

    if (borderRightLight < blackThreshold)
        turnForce = turnForce;
    else if (borderLeftLight < blackThreshold)
        turnForce = -turnForce;
    else
        return false;

    alignLine();

    int maxStepsRight = motorRight.motorSteps + robot.centimetersToSteps(4);
    int maxStepsLeft = motorLeft.motorSteps + robot.centimetersToSteps(4);
    builtInLED.on();
    while ((motorRight.motorSteps < maxStepsRight) && (motorLeft.motorSteps < maxStepsLeft))
    {
        readColors();
        if (checkGreen())
            return true;
        runLineFollowerGreenSensors();
    }
    builtInLED.off();

    alignLine();

    if (checkGreen())
        return true;

    robot.turn((turnForce > 0 ? -5 : 5), 60);
    robot.moveCentimeters(4, 70);

    readColors();
    while (lineSensors[3].getLight() > blackThreshold)
    {
        readColors();
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
void runLineFollower(bool checkForTurns = true)
{
    if (targetPower < maxPower && millis() > incrementVelocityTime)
    {
        incrementVelocityTime = millis() + 50;
        targetPower++;
    }

    if (centerLeftLight < blackThreshold)
    {
        robot.moveTime(80, -80, 50);
        lastCorrection = millis();
        targetPower = masterPower;
    }
    if (centerRightLight < blackThreshold)
    {
        robot.moveTime(-80, 80, 50);
        lastCorrection = millis();
        targetPower = masterPower;
    }
    if (leftLight < blackThreshold)
    {
        robot.moveTime(80, -80, 60);
        lastCorrection = millis();
        targetPower = masterPower;
    }
    if (rightLight < blackThreshold)
    {
        robot.moveTime(-80, 80, 60);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    robot.move(targetPower, targetPower);
}

void runFloor()
{
    readColors();
    // checkGreen();
    checkTurn();
    runLineFollower();
}