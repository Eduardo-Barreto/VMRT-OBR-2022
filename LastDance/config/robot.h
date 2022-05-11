#include "stepperMotor.h"
#include "gyro.h"
#include "calc.h"

#define PI 3.1415926535897932384626433832795

class robotBase
{
private:
    stepperMotor *motorRight;
    stepperMotor *motorLeft;
    gyroSensor *gyro;
    float wheelCircunference;
    int motorResolution;
    char maxVelocity;
    float acceleration;
    int stepsCompleteAcceleration;

public:
    /**
     * @brief Construtor da classe de base do robô
     * @param: motorRight: ponteiro do motor direito
     * @param: motorLeft: ponteiro do motor esquerdo
     * @param: gyro: ponteiro do giroscópio
     * @param: wheelCircunference: circunferência da roda
     *
     *
     * @example:
     *      robotBase robot(&motorRight, &motorLeft, &gyro, 61);
     */
    robotBase(stepperMotor *motorRight, stepperMotor *motorLeft, gyroSensor *gyro, byte wheelDiameter)
    {
        // Atribui as variáveis de acordo com os parâmetros
        this->motorRight = motorRight;
        this->motorLeft = motorLeft;
        this->gyro = gyro;
        this->wheelCircunference = wheelDiameter * PI;
        this->motorResolution = motorRight->resolution;
    }

    /**
     * @brief: Controla a velocidade do motor de passo, criando uma acelearção linear em trapézio
     * @param: velocity: ponteiro para controlar a velocidade
     * @param: steps: passo atual (para saber a posição atual do motor)
     * @param: maxSteps: máximo de passos dessa operação (para saber a posição final)
     * @param: _accelTime: tempo entre a aceleração (em milissegundos)
     * @param: accelControl: ponteiro para controlar a aceleração
     *
     * @example:
     *     robot.run(&velocity, steps, maxSteps, _accelTime, &accelControl);
     *
     * TODO: Novo cálculo
     */
    void linearAccelerate(float *velocity, int step, int maxSteps, float _accelTime, int *lastStepControl)
    {
        if (step < *lastStepControl)
            return;

        // Calcula a aceleração de acordo com amplitude de velocidade e distância
        acceleration = (abs(maxVelocity) - 1) / (float)(maxSteps / 3);

        // Se a velocidade é menor que a máxima e estamos no primeiro terço do movimento, acelera
        if ((abs(*velocity) < abs(maxVelocity)) && (step < (maxSteps / 3)))
        {
            *velocity = *velocity + (acceleration * (abs(maxVelocity) / maxVelocity));
        }

        // Se a diferença entre a velocidade atual e a mínima é maior que 3 e estamos no segundo terço do movimento, desacelera
        else if (((abs(*velocity) - 1) > 1) && (step >= maxSteps - stepsCompleteAcceleration))
        {
            *velocity = *velocity - (acceleration * (abs(maxVelocity) / maxVelocity));
        }

        if (stepsCompleteAcceleration == 0 && abs(*velocity) >= abs(maxVelocity))
        {
            stepsCompleteAcceleration = step;
        }

        // Atualiza o tempo necessário para acelerar novamente
        *lastStepControl = step;
    }

    /**
     * @brief: Move os motores do robô com a velocidade desejada
     * @param: velocityRight: velocidade do motor direito
     * @param velocityLeft: velocidade do motor esquerdo
     *
     * @example:
     *     robot.move(100, 100); // Move os motores com 100% de velocidade
     */
    void move(int velocityRight, int velocityLeft)
    {
        motorRight->run(velocityRight);
        motorLeft->run(velocityLeft);
        motorRight->pulse();
        motorLeft->pulse();
    }

    /**
     * @brief: Move os motores do robô durante o tempo desejado com a velocidade desejada
     * @param: velocityRight: velocidade do motor direito
     * @param velocityLeft: velocidade do motor esquerdo
     * @param time: tempo de movimento em milissegundos
     *
     * @example:
     *    robot.moveTime(100, 100, 1000); // Move os motores com 100% de velocidade por 1 segundo
     *
     *
     * TODO: Controlar aceleração durante o movimento;
     */
    void moveTime(int velocityRight, int velocityLeft, unsigned long time)
    {
        unsigned long endTime = millis() + time;
        while (millis() < endTime)
        {
            this->move(velocityRight, velocityLeft);
        }
    }

    /**
     * @brief: Move os motores do robô pela distância desejada com a velocidade desejada
     * @param: centimeters: distância em centímetros a ser percorrida
     * @param _velocity: velocidade desejada
     * @param _accelTime (opcional): tempo entre a aceleração (em milissegundos)
     *
     * @example:
     *      robot.moveCentimeters(100, 50); // Move os motores com 100% de velocidade por 50 centímetros
     */
    void moveCentimeters(int centimeters, float velocity, float _accelTime = 3)
    {
        maxVelocity = velocity;
        velocity = 1;
        int maxSteps = centimeters / (wheelCircunference / 10) * motorResolution;
        int RightInitialSteps = motorRight->motorSteps;
        int LeftInitialSteps = motorLeft->motorSteps;
        int RightSteps = 0;
        int LeftSteps = 0;
        stepsCompleteAcceleration = 0;
        int lastStepControl = 0;
        while (RightSteps < maxSteps && LeftSteps < maxSteps)
        {
            linearAccelerate(&velocity, LeftSteps, maxSteps, _accelTime, &lastStepControl);
            this->move(velocity, velocity);
            // Serial.println(velocity);
            RightSteps = motorRight->motorSteps - RightInitialSteps;
            LeftSteps = motorLeft->motorSteps - LeftInitialSteps;
        }
    }

    /**
     * @brief: Faz uma curva com os graus desejados com a velocidade desejada
     * @param: degrees: graus a serem percorridos
     * @param _velocity: velocidade desejada
     *
     * @example:
     *     robot.turnDegrees(90, 50); // Faz uma curva de 90 graus com 50% de velocidade
     */
    void turn(int degrees, float velocity)
    {
        char turnSide = degrees < 0 ? 1 : -1;

        degrees = abs(degrees);
        maxVelocity = velocity;
        velocity = 1;
        gyro->read();
        int lastDegree = abs(gyro->Yaw);
        int degreesCounter = 0;
        int lastStepControl;

        while (degreesCounter < degrees)
        {
            gyro->read();
            if (abs(int(gyro->Yaw) - lastDegree) > 0)
            {
                degreesCounter++;
                lastDegree = int(abs(gyro->Yaw));
            }
            linearAccelerate(&velocity, degreesCounter, degrees, 2, &lastStepControl);
            this->move(velocity * turnSide * -1, velocity * turnSide);
        }
    }
};
