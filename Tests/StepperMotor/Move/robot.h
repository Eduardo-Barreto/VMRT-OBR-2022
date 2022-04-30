#include "stepperMotor.h"
#include "gyro.h"

class robotBase
{
private:
    stepperMotor *motorRight;
    stepperMotor *motorLeft;
    float wheelCircunference;

public:
    robotBase(stepperMotor *motorRight, stepperMotor *motorLeft, byte wheelDiameter)
    {
        this->motorRight = motorRight;
        this->motorLeft = motorLeft;
        this->wheelCircunference = wheelDiameter * 3.14159265359f;
    }

    void move(int velocityRight, int velocityLeft)
    {
        motorRight->run(velocityRight);
        motorLeft->run(velocityLeft);
    }

    void moveTime(int velocityRight, int velocityLeft, unsigned long time)
    {
        unsigned long endTime = millis() + time;
        while (millis() < endTime)
        {
            motorRight->run(velocityRight);
            motorLeft->run(velocityLeft);
        }
    }

    void moveCentimeters(int velocityRight, int velocityLeft, int centimeters)
    {
        int maxSteps = centimeters / (wheelCircunference / 10) * 400;
        int RightInitialSteps = motorRight->motorSteps;
        int LeftInitialSteps = motorLeft->motorSteps;
        int RightSteps = 0;
        int LeftSteps = 0;
        while (RightSteps < maxSteps && LeftSteps < maxSteps)
        {
            motorRight->run(velocityRight);
            motorLeft->run(velocityLeft);
            RightSteps = motorRight->motorSteps - RightInitialSteps;
            LeftSteps = motorLeft->motorSteps - LeftInitialSteps;
        }
    }
};