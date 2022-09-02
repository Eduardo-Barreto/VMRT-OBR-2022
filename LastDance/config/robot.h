#include "stepperMotor.h"
#include "gyro.h"

#include "calc.h"

#define PI 3.1415926535897932384626433832795

class robotBase
{
private:
    char maxVelocity;                        // Velocidade máxima para controle de aceleração
    int motorResolution;                     // Resolução do motor (step por revolução)
    unsigned long stepsCompleteAcceleration; // Número de steps para acelerar
    float wheelCircunference;                // Circunferência do roda
    float robotCircunference;                // Circunferência do robô
    float acceleration;                      // Aceleração do motor
    stepperMotor *motorRight;                // Motor direito
    stepperMotor *motorLeft;                 // Motor esquerdo
    gyroSensor *gyro;                        // Sensor de giroscópio

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
    robotBase(stepperMotor *_motorRight, stepperMotor *_motorLeft, gyroSensor *_gyro, byte _wheelDiameter, int motorDiameter)
    {
        // Atribui as variáveis de acordo com os parâmetros
        this->motorRight = _motorRight;
        this->motorLeft = _motorLeft;
        this->gyro = _gyro;
        this->wheelCircunference = _wheelDiameter * PI;
        this->robotCircunference = motorDiameter * PI;
        this->motorResolution = motorRight->resolution;
    }

    /**
     * @brief Retorna a quantidade de steps do motor da direita
     * @return unsigned long: quantidade de steps do motor da direita
     */
    unsigned long getRightSteps()
    {
        return motorRight->motorSteps;
    }

    /**
     * @brief Retorna a quantidade de steps do motor da esquerda
     * @return unsigned long: quantidade de steps do motor da esquerda
     */
    unsigned long getLeftSteps()
    {
        return motorLeft->motorSteps;
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

    /**
     * @brief converte graus (rotação) em steps do motor
     * @param degrees: graus a serem convertidos
     * @return int: steps convertidos
     *
     * @example
     *     a = robot.degreesToSteps(90); // Converte 90 graus para steps e armazena na variável A
     */
    int turnDegreesToSteps(int degrees)
    {
        return ((this->robotCircunference / wheelCircunference) / 360 * degrees) * this->motorResolution;
    }

    /**
     * @brief Liga os motores do robô
     */
    void turnOnMotors()
    {
        motorRight->on();
        motorLeft->on();
    }

    /**
     * @brief Desliga os motores do robô
     */
    void turnOffMotors()
    {
        motorRight->off();
        motorLeft->off();
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

    /**
     * @brief Move o robô por uma quantidade especificada de steps
     * @param (int) velocityRight: velocidade do motor direito
     * @param (int) velocityLeft: velocidade do motor esquerdo
     * @param (int) steps: número de steps para mover
     *
     * @example
     *    robot.moveSteps(100, 100, 1000); // Move os motores com 100% de velocidade por 1000 steps
     */
    void moveSteps(int velocityRight, int velocityLeft, unsigned long _steps)
    {
        unsigned long endSteps = motorRight->motorSteps + _steps;
        while (motorRight->motorSteps <= endSteps)
        {
            this->move(velocityRight, velocityLeft);
        }
        this->stop();
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
    void moveTime(int velocityRight, int velocityLeft, unsigned long time, bool stopAfter = true)
    {
        unsigned long endTime = millis() + time;
        while (millis() < endTime)
        {
            this->move(velocityRight, velocityLeft);
        }
        if (stopAfter)
            this->stop();
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
    void linearAccelerate(float *velocity, int step, unsigned long maxSteps, float _accelTime, int *lastStepControl)
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
        char moveSide = (velocity > 0) ? 1 : -1;
        maxVelocity = abs(velocity);
        velocity = 1;
        unsigned long maxStepsRight = motorRight->motorSteps + centimetersToSteps(centimeters);
        unsigned long maxStepsLeft = motorLeft->motorSteps + centimetersToSteps(centimeters);
        stepsCompleteAcceleration = 0;
        int lastStepControl = 0;
        while (motorRight->motorSteps < maxStepsRight && motorLeft->motorSteps < maxStepsLeft)
        {
            if (_accelTime != 0)
                linearAccelerate(&velocity, motorRight->motorSteps, maxStepsRight, _accelTime, &lastStepControl);
            else
                velocity = maxVelocity;

            this->move(velocity * moveSide, velocity * moveSide);
        }
        this->stop();
    }

    /**
     * @brief Faz uma curva com os graus desejados com a velocidade desejada
     * @param degrees: graus a serem percorridos
     * @param velocity: velocidade desejada
     *
     * @example
     *     robot.turnDegrees(90, 50); // Faz uma curva de 90 graus com 50% de velocidade
     *     robot.turnDegrees(-90, 50); // Faz uma curva de -90 graus com 50% de velocidade
     */
    void turn(int degrees, int velocity)
    {
        char turnSide = degrees < 0 ? -1 : 1;
        gyro->read();
        int targetAngle = convertDegrees((gyro->Yaw + degrees));
        degrees = abs(degrees);
        velocity = min(abs(velocity), 85);

        int offsetMotors = 35;
        if (gyro->Yaw < 0)
            offsetMotors = 10;

        unsigned long maxStepsRight = motorRight->motorSteps + turnDegreesToSteps(degrees + offsetMotors);
        unsigned long maxStepsLeft = motorLeft->motorSteps + turnDegreesToSteps(degrees + offsetMotors);

        while (!proximity(gyro->Yaw, targetAngle, 1))
        {
            gyro->read();
            if (motorRight->motorSteps <= maxStepsRight)
                motorRight->move(velocity * -turnSide);
            else
                motorRight->move(0);

            if (motorLeft->motorSteps <= maxStepsLeft)
                motorLeft->move(velocity * turnSide);
            else
                motorLeft->move(0);

            if (motorRight->motorSteps >= maxStepsRight && motorLeft->motorSteps >= maxStepsLeft)
            {
                break;
            }
        }
        this->stop();
    }

    /**
     * @brief Faz uma curva com os graus desejados com a velocidade desejada, mas somente com uma roda
     * @param degrees: graus a serem percorridos
     * @param velocity: velocidade desejada
     *
     * @example
     *     robot.turnOneWheel(90, 50); // Faz uma curva de 90 graus com 50% de velocidade
     *     robot.turnOneWheel(-90, 50); // Faz uma curva de -90 graus com 50% de velocidade
     */
    void turnOneWheel(int degrees, int velocity)
    {
        char turnSide = degrees < 0 ? -1 : 1;
        degrees = abs(degrees);
        velocity = min(abs(velocity), 75);

        int offsetMotors = 40;
        gyro->read();

        int targetAngle = convertDegrees((gyro->Yaw + degrees));

        if (turnSide == -1)
        {
            // direita
            unsigned long maxSteps = motorRight->motorSteps + (turnDegreesToSteps(degrees + offsetMotors) * 2);
            motorLeft->move(0);
            while (motorRight->motorSteps < maxSteps)
            {
                motorRight->move(velocity);
                gyro->read();
                if (proximity(gyro->Yaw, targetAngle, 1))
                    break;
            }
        }
        else
        {
            // esquerda
            unsigned long maxSteps = motorLeft->motorSteps + (turnDegreesToSteps(degrees + offsetMotors) * 2);
            motorRight->move(0);
            while (motorLeft->motorSteps < maxSteps)
            {
                motorLeft->move(velocity);
                gyro->read();
                if (proximity(gyro->Yaw, targetAngle, 1))
                    break;
            }
        }
        this->stop();
    }

    /**
     * @brief Alinha o robô com algum objeto usando o ultrassônico da frente
     * @param velocity: velocidade desejada
     * @param distance: distância desejada do objeto
     * @param times: quantidade de vezes que o robô irá alinhar
     *
     * @example
     *     robot.align(50, 20, 3); // Alinha o robô com o objeto a 20cm de distância com 50% de velocidade 3 vezes
     */
    void alignUltra(int velocity, int distance, byte times = 2)
    {
        for (int i = 0; i <= times; i++)
        {
            while (centerUltra.read() > distance)
            {
                this->move(velocity, velocity);
            }
            this->stop();
            while (centerUltra.read() < distance)
            {
                this->move(-velocity, -velocity);
            }
            this->stop();
            velocity = velocity / 3;
        }
    }

    /**
     * @brief Rotaciona o robô para um determinado ângulo especificado com uma velocidade especificada
     * @param targetAngle: ângulo desejado
     * @param velocity: velocidade desejada
     *
     * @example
     *    robot.moveToAngle(90, 50); // Rotaciona o robô para 90 graus com 50% de velocidade
     */
    void moveToAngle(int targetAngle, int velocity = 60)
    {
        gyro->read();
        char turnSide = (targetAngle > gyro->Yaw) ? 1 : -1;
        turnSide = (abs(targetAngle - gyro->Yaw) > 180) ? -turnSide : turnSide;
        while (((targetAngle != 0) && !proximity(gyro->Yaw, targetAngle, 1)) || ((targetAngle == 0) && (gyro->Yaw < 358 && gyro->Yaw > 2)))
        {
            this->move(-velocity * turnSide, velocity * turnSide);
            gyro->read();
        }
        this->stop();
    }

    /**
     * @brief Alinha o robô com um ângulo ortogonal em uma velocidade especificada
     * @param velocity: velocidade desejada
     */
    void alignAngle(int velocity = 40)
    {
        this->stop(50);
        gyro->read();
        int angle = gyro->Yaw;
        if (angle < 0)
            return;

        if ((angle > 315) || (angle <= 45))
        {
            this->moveToAngle(0);
        }
        else if ((angle > 45) && (angle <= 135))
        {
            this->moveToAngle(90);
        }
        else if ((angle > 135) && (angle <= 225))
        {
            this->moveToAngle(180);
        }
        else if ((angle > 225) && (angle <= 315))
        {
            this->moveToAngle(270);
        }
    }

    /**
     * @brief Para o robô, desliga o motor e não faz mais nada até o reset
     */
    void die()
    {
        this->turnOffMotors();
        while (true)
        {
        }
    }
};
