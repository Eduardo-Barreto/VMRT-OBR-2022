int borderThreshold = 80; // Valor mínimo para considerar que o sensor está diferente do seu oposto (direita-esquerda)
int blackThreshold = 35;  // Valor máximo para considerar que o sensor está lendo preto

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

bool checkGreen(int turnForce = 90)
{
    if (rightGreen)
        turnForce = turnForce;
    else if (leftGreen)
        turnForce = -turnForce;
    else
        return false;

    delay(1000);

    alignLine();

    robot.moveCentimeters(10, 70);
    robot.turn((turnForce > 0 ? -15 : 15), turnForce);
    delay(1000);

    while (lineSensors[3].getLight() > blackThreshold)
    {
        robot.move(turnForce, -turnForce);
    }

    alignLine();
    targetPower = masterPower - 5;
    lastCorrection = millis();
    return true;
}

/**
 * @brief Busca por curvas
 *
 * @param curveForce: Força aplicada ao robo na curva
 */
bool checkTurn(int turnForce = 90)
{
    if (checkGreen())
        return true;

    if (borderDiff >= borderThreshold) // direita
        turnForce = turnForce;
    else if (borderDiff <= -borderThreshold) // esquerda
        turnForce = -turnForce;
    else
        return false;

    alignLine();

    int maxStepsRight = motorRight.motorSteps + robot.centimetersToSteps(3);
    int maxStepsLeft = motorLeft.motorSteps + robot.centimetersToSteps(3);
    while ((motorRight.motorSteps < maxStepsRight) && (motorLeft.motorSteps < maxStepsLeft))
    {
        readColors();
        if (checkGreen())
            return true;
        robot.move(70, 70);
    }

    alignLine();

    if (checkGreen())
        return true;

    robot.turn((turnForce > 0 ? -5 : 5), 60);
    robot.moveCentimeters(4, 70);

    readColors();
    while (lineSensors[3].getLight() > blackThreshold)
    {
        readColors();
        robot.move(turnForce, -turnForce);
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

    unsigned long timeout = millis() + 250;
    while ((centerDiff > borderThreshold) && (millis() < timeout))
    {
        readColors();
        if (checkForTurns && checkTurn())
            return;

        robot.move(targetPower, -targetPower);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    timeout = millis() + 250;
    while ((centerDiff < -borderThreshold) && (millis() < timeout))
    {
        readColors();
        if (checkForTurns && checkTurn())
            return;

        robot.move(-targetPower, targetPower);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    robot.move(targetPower, targetPower);
}

void runFloor()
{
    readColors();
    checkGreen();
    checkTurn();
    runLineFollower();
}