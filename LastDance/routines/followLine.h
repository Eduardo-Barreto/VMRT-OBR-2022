int borderThreshold = 80; // Valor mínimo para considerar que o sensor está diferente do seu oposto (direita-esquerda)
int blackThreshold = 20;  // Valor máximo para considerar que o sensor está lendo preto

byte centerRightLight; // Valor lido do sensor de luz do meio da direita
byte centerLeftLight;  // Valor lido do sensor de luz do meio da esquerda
byte rightLight;       // Valor lido do sensor de luz da direita
byte leftLight;        // Valor lido do sensor de luz da esquerda
byte borderRightLight; // Valor lido do sensor de luz da borda da direita
byte borderLeftLight;  // Valor lido do sensor de luz da borda da esquerda

bool centerRightBlack; // Indica se o valor do sensor de luz do meio da direita é preto
bool centerLeftBlack;  // Indica se o valor do sensor de luz do meio da esquerda é preto
bool rightBlack;       // Indica se o valor do sensor de luz da direita é preto
bool leftBlack;        // Indica se o valor do sensor de luz da esquerda é preto
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

    borderLeftBlack = lineSensors[0].getLight() < blackThreshold;
    leftBlack = lineSensors[1].getLight() < blackThreshold;
    centerLeftBlack = lineSensors[2].getLight() < blackThreshold;
    centerRightBlack = lineSensors[4].getLight() < blackThreshold;
    rightBlack = lineSensors[5].getLight() < blackThreshold;
    borderRightBlack = lineSensors[6].getLight() < blackThreshold;

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
    unsigned long timeout = millis() + 150;
    while (greenSensors[0].getLight() < blackThreshold && millis() < timeout)
    {
        robot.move(0, -25);
    }

    timeout = millis() + 150;
    while (greenSensors[1].getLight() < blackThreshold && millis() < timeout)
    {
        robot.move(-25, 0);
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

    delay(300);
    robot.moveCentimeters(7, 50);
    delay(150);
    robot.turn(((turnForce > 0) ? (15) : (-15)), 60);

    while (lineSensors[3].getLight() > blackThreshold + 10)
    {
        robot.move(-turnForce, turnForce);
    }

    alignLine();
    targetPower = masterPower - 5;
    lastCorrection = millis();
    greenSensors[0].green = false;
    greenSensors[1].green = false;
    rightTurnLED.off();
    greenLED.off();
    leftTurnLED.off();
    return true;
}

/**
 * @brief Busca por curvas
 *
 * @param curveForce: Força aplicada ao robo na curva
 */
bool checkTurn(int turnForce = 90)
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

    if (checkGreen())
        return true;

    robot.stop(100);
    while ((lineSensors[0].getLight() > 20) && (lineSensors[6].getLight() > 20))
        robot.move(-15, -15);
    robot.stop();

    alignGreen();
    robot.stop(100);

    readColors();

    if (checkGreen())
        return true;

    if ((borderRightLight < blackThreshold) && (borderLeftLight < blackThreshold))
    {
        robot.moveCentimeters(4, 60);
        return false;
    }

    robot.turn((turnForce < 0 ? 5 : -5), 60);
    robot.moveCentimeters(4, 70);

    while (lineSensors[3].getLight() > blackThreshold + 10)
    {
        robot.move(-turnForce, turnForce);
    }

    alignLine();
    targetPower = masterPower - 5;
    lastCorrection = millis();
    rightTurnLED.off();
    leftTurnLED.off();
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

    unsigned long timeout = millis() + 60;
    while ((centerLeftBlack || leftBlack) && millis() < timeout)
    {
        if (centerRightBlack || rightBlack)
            break;
        readColors();
        checkTurn();
        robot.move(80, -80);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    timeout = millis() + 60;
    while ((centerRightBlack || rightBlack) && millis() < timeout)
    {
        if (centerLeftBlack || leftBlack)
            break;
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
    checkTurn();
    runLineFollower();
}