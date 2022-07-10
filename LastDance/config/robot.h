#include "stepperMotor.h"
#include "gyro.h"

#include "calc.h"

#define PI 3.1415926535897932384626433832795

class robotBase
{
private:
    stepperMotor *motorRight;      // Motor direito
    stepperMotor *motorLeft;       // Motor esquerdo
    gyroSensor *gyro;              // Sensor de giroscópio
    float wheelCircunference;      // Circunferência do roda
    int motorResolution;           // Resolução do motor (step por revolução)
    char maxVelocity;              // Velocidade máxima para controle de aceleração
    float acceleration;            // Aceleração do motor
    int stepsCompleteAcceleration; // Número de steps para acelerar

public:
    /**
     * @brief Construtor da classe de base do robô
     * @param motorRight: ponteiro do motor direito
     * @param motorLeft: ponteiro do motor esquerdo
     * @param gyro: ponteiro do giroscópio
     * @param wheelCircunference: circunferência da roda
     *
     *
     * @example
     *      robotBase robot(&motorRight, &motorLeft, &gyro, 61);
     */
    robotBase(stepperMotor *_motorRight, stepperMotor *_motorLeft, gyroSensor *_gyro, byte _wheelDiameter)
    {
        // Atribui as variáveis de acordo com os parâmetros
        this->motorRight = _motorRight;
        this->motorLeft = _motorLeft;
        this->gyro = _gyro;
        this->wheelCircunference = _wheelDiameter * PI;
        this->motorResolution = motorRight->resolution;
    }

    /**
     * @brief Controla a velocidade do motor de passo, criando uma acelearção linear em trapézio
     * @param velocity: ponteiro para controlar a velocidade
     * @param steps: passo atual (para saber a posição atual do motor)
     * @param maxSteps: máximo de passos dessa operação (para saber a posição final)
     * @param accelTime: tempo entre a aceleração (em milissegundos)
     * @param accelControl: ponteiro para controlar a aceleração
     *
     * @example
     *     robot.run(&velocity, steps, maxSteps, _accelTime, &accelControl);
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
     * @brief Move os motores do robô com a velocidade desejada
     * @param velocityRight: velocidade do motor direito
     * @param velocityLeft: velocidade do motor esquerdo
     *
     * @example
     *     robot.move(100, 100); // Move os motores com 100% de velocidade
     */
    void move(int velocityRight, int velocityLeft)
    {
        motorRight->set(velocityRight);
        motorLeft->set(velocityLeft);
        motorRight->run();
        motorLeft->run();
    }

    /**
     * @brief Para os motores durante o tempo desejado
     * @param time: tempo para parar os motores (em milissegundos)
     *
     * @example
     *    robot.stop(1000); // Para os motores durante 1 segundo
     */
    void stop(int time = 0)
    {
        move(0, 0);
        if (time > 0)
            delay(time);
    }

    void turnOnMotors()
    {
        motorRight->on();
        motorLeft->on();
    }

    void turnOffMotors()
    {
        motorRight->off();
        motorLeft->off();
    }

    /**
     * @brief Move os motores do robô durante o tempo desejado com a velocidade desejada
     * @param velocityRight: velocidade do motor direito
     * @param velocityLeft: velocidade do motor esquerdo
     * @param time: tempo de movimento em milissegundos
     *
     * @example
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
     * @brief Move os motores do robô pela distância desejada com a velocidade desejada
     * @param centimeters: distância em centímetros a ser percorrida
     * @param velocity: velocidade desejada
     * @param accelTime (opcional): tempo entre a aceleração (em milissegundos)
     *
     * @example
     *      robot.moveCentimeters(100, 50); // Move os motores com 100% de velocidade por 50 centímetros
     */
    void moveCentimeters(int centimeters, float velocity, float _accelTime = 3)
    {
        maxVelocity = velocity;
        velocity = 1;
        int maxStepsRight = motorRight->motorSteps + centimetersToSteps(centimeters);
        int maxStepsLeft = motorLeft->motorSteps + centimetersToSteps(centimeters);
        stepsCompleteAcceleration = 0;
        int lastStepControl = 0;
        while (motorRight->motorSteps < maxStepsRight && motorLeft->motorSteps < maxStepsLeft)
        {
            if (_accelTime != 0)
                linearAccelerate(&velocity, motorRight->motorSteps, maxStepsRight, _accelTime, &lastStepControl);
            else
                velocity = maxVelocity;

            this->move(velocity, velocity);
        }
    }

    /**
     * @brief Faz uma curva com os graus desejados com a velocidade desejada
     * @param degrees: graus a serem percorridos
     * @param _velocity: velocidade desejada
     *
     * @example
     *     robot.turnDegrees(90, 50); // Faz uma curva de 90 graus com 50% de velocidade
     *     robot.turnDegrees(-90, 50); // Faz uma curva de -90 graus com 50% de velocidade
     */
    void turn(int degrees, float velocity)
    {
        char turnSide = degrees < 0 ? -1 : 1;
        velocity = min(velocity, 85);

        gyro->read();
        int targetAngle = convertDegrees((gyro->Yaw + degrees));

        while (!proximity(gyro->Yaw, targetAngle, 1))
        {
            this->move(velocity * -turnSide, velocity * turnSide);
            gyro->read();
        }
    }

    /**
     * @brief Converte centímetros para steps
     *
     * @param centimeters: centímetros a serem convertidos
     *
     * @return int: steps convertidos
     *
     * @example
     *    a = robot.centimetersToSteps(100); // Converte 100 centímetros para steps e armazena na variável A
     *    // a variável `a` depende da circunferência da roda e da resolução do motor
     */
    int centimetersToSteps(int centimeters)
    {
        float _steps = centimeters / (wheelCircunference / 10) * motorResolution;
        return _steps;
    }
};
