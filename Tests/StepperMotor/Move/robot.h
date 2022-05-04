#include "stepperMotor.h"
#include "gyro.h"
#include <math.h>

#define E 2.71828182845904523536

class robotBase
{
private:
    stepperMotor *motorRight;
    stepperMotor *motorLeft;
    float wheelCircunference;
    int motorResolution;
    unsigned long accelTime = 0;
    char maxVelocityRight;
    char maxVelocityLeft;
    char accelVector = 1;
    double mediumX = 0;
    float slope = 0.3;

public:
    robotBase(stepperMotor *motorRight, stepperMotor *motorLeft, byte wheelDiameter)
    {
        this->motorRight = motorRight;
        this->motorLeft = motorLeft;
        this->wheelCircunference = wheelDiameter * M_PI;
        motorResolution = motorRight->resolution;
    }

    void linearAccelerate(int *velocityRight, int *velocityLeft)
    {
        if (millis() > accelTime)
        {
            if (maxVelocityRight > 0)
            {
                *velocityRight = (*velocityRight < maxVelocityRight) ? *velocityRight + 1 : *velocityRight;
            }
            else
            {
                *velocityRight = (*velocityRight > maxVelocityRight) ? *velocityRight - 1 : *velocityRight;
            }
            if (maxVelocityLeft > 0)
            {
                *velocityLeft = (*velocityLeft < maxVelocityLeft) ? *velocityLeft + 1 : *velocityLeft;
            }
            else
            {
                *velocityLeft = (*velocityLeft > maxVelocityLeft) ? *velocityLeft - 1 : *velocityLeft;
            }
            accelTime = millis() + 10;
        }
    }

    void sigmoideAccel(int *velocity, int maxVelocity, int maxSteps, int step)
    {
        char minVelocity = maxVelocity / abs(maxVelocity);
        char amplitude = maxVelocity - minVelocity;

        long mediumX = ((-1) * log(amplitude / ((maxVelocity * 0.99999) - minVelocity) - 1));

        long maxVelocitySteps = maxSteps - ((int)(mediumX)*4);

        if (step <= mediumX * 2)
        {
            accelVector = 1;
            long i = mediumX * 2 - step;
            *velocity = (int)((amplitude / (1 + pow(E, (accelVector * slope * (i - mediumX))))) + minVelocity);
        }

        else if (step >= maxSteps - (mediumX * 2))
        {
            accelVector = -1;
            long reference = step - (maxSteps - (mediumX * 2));
            long i = (mediumX * 2) - reference;
            *velocity = (int)((amplitude / (1 + pow(E, (accelVector * slope * (i - mediumX))))) - (minVelocity - (1 * minVelocity)));
        }

        else
        {
            *velocity = maxVelocity;
        }
    }

    void move(int velocityRight, int velocityLeft)
    {
        motorRight->run(velocityRight);
        motorLeft->run(velocityLeft);
        motorRight->pulse();
        motorLeft->pulse();
    }

    void moveTime(int velocityRight, int velocityLeft, unsigned long time)
    {
        unsigned long endTime = millis() + time;
        while (millis() < endTime)
        {
            this->move(velocityRight, velocityLeft);
        }
    }

    void moveCentimeters(int velocityRight, int velocityLeft, int centimeters)
    {
        maxVelocityRight = velocityRight;
        maxVelocityLeft = velocityLeft;
        velocityRight = 0;
        velocityLeft = 0;
        int maxSteps = centimeters / (wheelCircunference / 10) * motorResolution;
        int RightInitialSteps = motorRight->motorSteps;
        int LeftInitialSteps = motorLeft->motorSteps;
        int RightSteps = 0;
        int LeftSteps = 0;
        while (RightSteps < maxSteps && LeftSteps < maxSteps)
        {
            sigmoideAccel(&velocityRight, maxVelocityRight, maxSteps, RightSteps);
            sigmoideAccel(&velocityLeft, maxVelocityLeft, maxSteps, LeftSteps);
            this->move(velocityRight, velocityLeft);
            Serial.println(velocityRight);
            RightSteps = motorRight->motorSteps - RightInitialSteps;
            LeftSteps = motorLeft->motorSteps - LeftInitialSteps;
        }
    }
};
