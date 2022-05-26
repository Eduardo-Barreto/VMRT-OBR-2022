int borderThreshold = 50;
int blackThreshold = 35;

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

/**
 * @brief Segue a linha
 *
 * @param targetPower: Velocidade padrão para o movimento quando está centralizado na linha
 * @param turnPower: diferença da velocidade para o movimento quando está na borda da linha
 */
void runLineFollower(byte targetPower, byte turnPower)
{
    for (int i = 0; i < 7; i++)
    {
        lineSensors[i].read();
    }
    byte rightLight = abs(lineSensors[2].getLight());
    byte leftLight = abs(lineSensors[4].getLight());

    char diff = (rightLight - leftLight);

    if (diff >= borderThreshold)
    {
        // esquerda
        robot.move(targetPower + turnPower, 0);
    }
    else if (diff <= -borderThreshold)
    {
        // direita
        robot.move(0, targetPower + turnPower);
    }
    else
    {
        robot.move(targetPower, targetPower);
    }
}

void proportional(int targetPower, int KP)
{
    for (int i = 0; i < 7; i++)
    {
        lineSensors[i].read();
    }
    byte rightLight = abs(lineSensors[2].getLight());
    byte leftLight = abs(lineSensors[4].getLight());

    char diff = (rightLight - leftLight);

    int leftPower = targetPower + (diff * KP);
    int rightPower = targetPower - (diff * KP);

    robot.move(leftPower, rightPower);
}

void alignLine(int _timeout = 750)
{
    char diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
    bool right = 0;

    unsigned long timeout = millis() + _timeout;

    while ((diff > borderThreshold) && (millis() < timeout))
    {
        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(50, -50);
    }

    timeout = millis() + _timeout;
    while ((diff < -borderThreshold) && (millis() < timeout))
    {
        diff = (lineSensors[2].getLight() - lineSensors[4].getLight());
        robot.move(-50, 50);
    }
}

/**
 * @brief Busca por curvas
 */
void checkTurn()
{
    // Lê todos os sensores
    for (int i = 0; i < 7; i++)
    {
        lineSensors[i].read();
    }

    // Armazena as leituras dos sensores das pontas
    byte borderRightLight = abs(lineSensors[0].getLight());
    byte borderLeftLight = abs(lineSensors[6].getLight());

    // Calcula a diferença entre as leituras
    int diff = (borderRightLight - borderLeftLight);

    if (diff >= borderThreshold)
    {
        // direita
        robot.moveCentimeters(7, 70, 0);
        // mover à direita até o sensor 3 encontrar o preto
        while (lineSensors[3].getLight() > blackThreshold)
        {
            robot.move(50, -50);
        }
    }
    else if (diff <= -borderThreshold)
    {
        // esquerda
        robot.moveCentimeters(7, 70, 0);
        // mover à esquerda até o sensor 3 encontrar o preto
        while (lineSensors[3].getLight() > blackThreshold)
        {
            robot.move(-50, 50);
        }
    }
    alignLine();
}
