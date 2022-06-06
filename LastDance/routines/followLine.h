int borderThreshold = 50; // Valor mínimo para considerar que o sensor está diferente do seu oposto (direita-esquerda)
int blackThreshold = 35;  // Valor máximo para considerar que o sensor está lendo preto

byte rightLight;       // Valor lido do sensor de luz da direita
byte leftLight;        // Valor lido do sensor de luz da esquerda
byte borderRightLight; // Valor lido do sensor de luz da borda da direita
byte borderLeftLight;  // Valor lido do sensor de luz da borda da esquerda

int centerDiff; // Diferença entre os valores lido do sensor de luz da direita e da esquerda
int borderDiff; // Diferença entre os valores lido do sensor de luz da borda da direita e da borda da esquerda

unsigned long lastCorrection;        // Variável para armazenar o tempo do último ajuste
unsigned long incrementVelocityTime; // Variável para armazenar o tempo do próximo ajuste de velocidade

/**
 * @brief Atualiza os valores lido dos sensores de luz e as variáveis de controle
 */
void readColors()
{
    rightLight = lineSensors[2].getLight();
    leftLight = lineSensors[4].getLight();
    borderRightLight = lineSensors[0].getLight();
    borderLeftLight = lineSensors[6].getLight();

    centerDiff = rightLight - leftLight;
    borderDiff = borderRightLight - borderLeftLight;
}

/**
 * @brief Calibra os sensores de luz para a iluminação atual
 */
void calibrateLineFollower()
{
    unsigned long start = millis();
    while (millis() - start < 1000)
    {
        robot.move(20, 20);
        for (int i = 0; i < 7; i++)
        {
            lineSensors[i].read();
            lineSensors[i].minRead = (lineSensors[i].raw < lineSensors[i].minRead) ? lineSensors[i].raw : lineSensors[i].minRead;
            lineSensors[i].maxRead = (lineSensors[i].raw > lineSensors[i].maxRead) ? lineSensors[i].raw : lineSensors[i].maxRead;
        }
    }
    delay(500);
    start = millis();
    while (millis() - start < 1000)
    {
        robot.move(-20, -20);
        for (int i = 0; i < 7; i++)
        {
            lineSensors[i].read();
            lineSensors[i].minRead = (lineSensors[i].raw < lineSensors[i].minRead) ? lineSensors[i].raw : lineSensors[i].minRead;
            lineSensors[i].maxRead = (lineSensors[i].raw > lineSensors[i].maxRead) ? lineSensors[i].raw : lineSensors[i].maxRead;
        }
    }
}

void printCalibrationFollower()
{
    // print all calibration for lineSensors
    for (int i = 0; i < 7; i++)
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
}

void alignLine(int force = 50, int _timeout = 750)
{
    char diff = (lineSensors[2].getLight() - lineSensors[4].getLight());

    unsigned long timeout = millis() + _timeout;

    while ((diff > borderThreshold) && (millis() < timeout))
    {
        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(force, -force);
    }

    timeout = millis() + _timeout;
    while ((diff < -borderThreshold) && (millis() < timeout))
    {
        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(-force, force);
    }
}

/**
 * @brief Busca por curvas
 *
 * @param curveForce: Força aplicada ao robo na curva
 */
bool checkTurn(int curveForce = 90)
{

    if (borderDiff >= borderThreshold)
    {
        alignLine();
        // direita
        robot.turn(-5, 60);

        readColors();

        robot.moveCentimeters(7, 70);
        // mover à direita até o sensor 3 ou 4 encontrar o preto
        while (
            lineSensors[3].getLight() > blackThreshold &&
            lineSensors[4].getLight() > blackThreshold)
        {
            robot.move(curveForce, -curveForce);
        }
        alignLine();
        targetPower = masterPower - 5;
        lastCorrection = millis();
        return true;
    }
    else if (borderDiff <= -borderThreshold)
    {
        alignLine();
        // esquerda
        robot.turn(5, 60);

        readColors();

        robot.moveCentimeters(7, 70);
        // mover à esquerda até o sensor 3 ou 4 encontrar o preto
        while (
            lineSensors[3].getLight() > blackThreshold &&
            lineSensors[2].getLight() > blackThreshold)
        {
            robot.move(-curveForce, curveForce);
        }
        alignLine();
        targetPower = masterPower - 5;
        lastCorrection = millis();
        return true;
    }

    return false;
}

/**
 * @brief Segue a linha
 *
 * @param increaseTargetPower: Diferença na velocidade padrão para o movimento quando está centralizado na linha
 * @param increaseTurnPower: diferença da velocidade para o movimento quando está na borda da linha
 */
void runLineFollower(byte increaseTargetPower = 0)
{
    if (targetPower < maxPower && millis() > incrementVelocityTime)
    {
        incrementVelocityTime = millis() + 50;
        targetPower++;
    }

    readColors();

    if (checkTurn(targetPower))
        return;

    unsigned long timeout = millis() + 250;
    while ((centerDiff > borderThreshold) && (millis() < timeout))
    {
        readColors();
        if (checkTurn())
            return;

        robot.move(targetPower, -targetPower);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    timeout = millis() + 250;
    while ((centerDiff < -borderThreshold) && (millis() < timeout))
    {
        readColors();
        if (checkTurn())
            return;

        robot.move(-targetPower, targetPower);
        lastCorrection = millis();
        targetPower = masterPower;
    }

    robot.move(targetPower, targetPower);
}
