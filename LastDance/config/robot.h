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

public:
    robotBase(stepperMotor *motorRight, stepperMotor *motorLeft, gyroSensor *gyro, byte wheelDiameter)
    {
        this->motorRight = motorRight;
        this->motorLeft = motorLeft;
        this->gyro = gyro;
        this->wheelCircunference = wheelDiameter * PI;
        this->motorResolution = motorRight->resolution;
    }

    /*
        @brief: Controla a velocidade do motor de passo, criando uma acelearção linear em trapézio
        @param: *velocity: ponteiro para controlar a velocidade
        @param steps: passo atual (para saber a posição atual do motor)
        @param maxSteps: máximo de passos dessa operação (para saber a posição final)
        @param _accelTime: tempo entre a aceleração (em milissegundos)
        @param *accelControl: ponteiro para controlar a aceleração
    */
    void linearAccelerate(float *velocity, int step, int maxSteps, float _accelTime, unsigned long *accelControl)
    {
        if (millis() < *accelControl)
        {
            return;
        }

        acceleration = (abs(maxVelocity) - 1) / (float)(maxSteps / 3);
        if ((abs(*velocity) < abs(maxVelocity)) && (step < (maxSteps / 3)))
        {
            *velocity = *velocity + (acceleration * (abs(maxVelocity) / maxVelocity));
        }

        else if (((abs(*velocity) - 1) > 3) && (step >= (maxSteps / 3) * 2))
        {
            *velocity = *velocity - (acceleration * (abs(maxVelocity) / maxVelocity));
        }

        *accelControl = millis() + _accelTime;
    }

    /*
        @brief: Move os motores do robô com a velocidade desejada
        @param: velocityRight: velocidade do motor direito
        @param velocityLeft: velocidade do motor esquerdo
    */
    void move(int velocityRight, int velocityLeft)
    {
        motorRight->run(velocityRight);
        motorLeft->run(velocityLeft);
        motorRight->pulse();
        motorLeft->pulse();
    }

    /*
        @brief: Move os motores do robô durante o tempo desejado com a velocidade desejada
        @param: velocityRight: velocidade do motor direito
        @param velocityLeft: velocidade do motor esquerdo
        @param time: tempo de movimento em milissegundos
    */
    void moveTime(int velocityRight, int velocityLeft, unsigned long time)
    {
        unsigned long endTime = millis() + time;
        while (millis() < endTime)
        {
            this->move(velocityRight, velocityLeft);
        }
    }

    /*
        @brief: Move os motores do robô pela distância desejada com a velocidade desejada
        @param: centimeters: distância em centímetros a ser percorrida
        @param _velocity: velocidade desejada
        @param _accelTime: tempo entre a aceleração (em milissegundos)
    */
    void moveCentimeters(int centimeters, float _velocity, float _accelTime = 2)
    {
        maxVelocity = _velocity;
        float velocityRight = 1;
        float velocityLeft = 1;
        int maxSteps = centimeters / (wheelCircunference / 10) * motorResolution;
        int RightInitialSteps = motorRight->motorSteps;
        int LeftInitialSteps = motorLeft->motorSteps;
        int RightSteps = 0;
        int LeftSteps = 0;
        unsigned long accelControlRight = 0;
        unsigned long accelControlLeft = 0;
        while (RightSteps < maxSteps && LeftSteps < maxSteps)
        {
            linearAccelerate(&velocityLeft, LeftSteps, maxSteps, _accelTime, &accelControlRight);
            linearAccelerate(&velocityRight, RightSteps, maxSteps, _accelTime, &accelControlLeft);
            this->move(velocityRight, velocityLeft);
            RightSteps = motorRight->motorSteps - RightInitialSteps;
            LeftSteps = motorLeft->motorSteps - LeftInitialSteps;
        }
    }

    /*
        @brief: Faz uma curva com os graus desejados com a velocidade desejada
        @param: degrees: graus a serem percorridos
        @param _velocity: velocidade desejada
    */
    void turn(int degrees, float velocity)
    {
        char turnSide = degrees > 0 ? 1 : -1;

        degrees = abs(degrees);
        unsigned long accelController = 0;
        maxVelocity = velocity;
        velocity = 1;
        gyro->read();
        int lastDegree = abs(gyro->Yaw);
        int degreesCounter = 0;

        while (degreesCounter < degrees)
        {
            gyro->read();
            if (abs(gyro->Yaw) > convertDegrees(lastDegree + 1))
            {
                degreesCounter++;
                lastDegree = abs(gyro->Yaw);
            }
            linearAccelerate(&velocity, degreesCounter, degrees, 2, &accelController);
            this->move(velocity * turnSide, velocity * turnSide * -1);
        }
    }
};
