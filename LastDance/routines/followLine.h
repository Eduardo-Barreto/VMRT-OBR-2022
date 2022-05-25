#include "../config/robot.h"

class lineFollower
{
private:
    robotBase *robot;
    byte blackThreshold;
    lightSensor (*lineSensors)[7];

public:
    /**
     * @brief Construtor da classe de seguir linha
     * @param: robot: ponteiro do robô
     * @param: blackThreshold: limiar de preto
     * @param: lineSensors: ponteiro da lista de sensores de cor
     *
     * @example:
     *      lineFollower lineFollower(&robot, 100, lineSensors);
     */
    lineFollower(robotBase *_robot, byte _blackThreshold, lightSensor (*_lineSensors)[7])
    {
        this->robot = _robot;
        this->blackThreshold = _blackThreshold;
        this->lineSensors = _lineSensors;
    }

    /**
     * @brief: Calibra os sensores de luz para a iluminação atual
     */
    void calibrate()
    {
        unsigned long start = millis();
        while (millis() - start < 1000)
        {
            robot->move(20, 20);
            for (int i = 0; i < 7; i++)
            {
                lineSensors[i]->read();
                lineSensors[i]->minRead = (lineSensors[i]->raw < lineSensors[i]->minRead) ? lineSensors[i]->raw : lineSensors[i]->minRead;
                lineSensors[i]->maxRead = (lineSensors[i]->raw > lineSensors[i]->maxRead) ? lineSensors[i]->raw : lineSensors[i]->maxRead;
            }
        }
        delay(500);
        start = millis();
        while (millis() - start < 1000)
        {
            robot->move(-20, -20);
            for (int i = 0; i < 7; i++)
            {
                lineSensors[i]->read();
                lineSensors[i]->minRead = (lineSensors[i]->raw < lineSensors[i]->minRead) ? lineSensors[i]->raw : lineSensors[i]->minRead;
                lineSensors[i]->maxRead = (lineSensors[i]->raw > lineSensors[i]->maxRead) ? lineSensors[i]->raw : lineSensors[i]->maxRead;
            }
        }
    }

    /**
     * @brief: Segue a linha
     *
     * @param: targetPower: Velocidade padrão para o movimento quando está centralizado na linha
     * @param: turnPower: diferença da velocidade para o movimento quando está na borda da linha
     */
    void run(byte targetPower, byte turnPower)
    {
        for (int i = 0; i < 7; i++)
        {
            lineSensors[i]->read();
        }
        byte rightLight = abs(lineSensors[2]->getLight());
        byte leftLight = abs(lineSensors[4]->getLight());

        char diff = (rightLight - leftLight);

        if (diff >= blackThreshold)
        {
            // esquerda
            robot->move(targetPower + turnPower, 0);
        }
        else if (diff <= -blackThreshold)
        {
            // direita
            robot->move(0, targetPower + turnPower);
        }
        else
        {
            robot->move(targetPower, targetPower);
        }
    }

    /**
     * @brief: Busca por curvas
     */
    void checkTurn()
    {
        for (int i = 0; i < 7; i++)
        {
            lineSensors[i]->read();
        }
        byte borderRightLight = abs(lineSensors[0]->getLight());
        byte borderLeftLight = abs(lineSensors[6]->getLight());

        int diff = (borderRightLight - borderLeftLight);

        if (diff >= blackThreshold)
        {
            // direita
            robot->moveCentimeters(7, 70, 0);
            // mover à direita até o sensor 3 encontrar o preto
            while (lineSensors[3]->getLight() > blackThreshold)
            {
                robot->move(50, -50);
            }
            robot->moveTime(50, -50, 200);
        }
        else if (diff <= -blackThreshold)
        {
            // esquerda
            robot->moveCentimeters(7, 70, 0);
            // mover à esquerda até o sensor 3 encontrar o preto
            while (lineSensors[3]->getLight() > blackThreshold)
            {
                robot->move(-50, 50);
            }
            robot->moveTime(-50, 50, 200);
        }
    }

    void align()
    {
        char diff = (lineSensors[2]->getLight() - lineSensors[4]->getLight());
        bool right = 0;

        while (diff > blackThreshold)
        {
            diff = (lineSensors[2]->getLight() - lineSensors[4]->getLight());
            robot->move(50, -50);
        }
        while (diff < -blackThreshold)
        {
            diff = (lineSensors[2]->getLight() - lineSensors[4]->getLight());
            robot->move(-50, 50);
        }
    }

    void test()
    {
        while (1)
        {
            // show all sensor values
            robot->move(20, 20);
            for (int i = 0; i < 7; i++)
            {
                lineSensors[i]->read();
                lineSensors[i]->minRead = (lineSensors[i]->raw < lineSensors[i]->minRead) ? lineSensors[i]->raw : lineSensors[i]->minRead;
                lineSensors[i]->maxRead = (lineSensors[i]->raw > lineSensors[i]->maxRead) ? lineSensors[i]->raw : lineSensors[i]->maxRead;
            }
        }
    }
};
