/*
Módulo de configuração dos motores

X Axis -> Direita
Y Axis -> Esquerda
*/

class stepperMotor
{
private:
    byte stepPin;
    byte directionPin;
    byte enablePin;
    int resolution;
    int minimumVelocity;
    int maximumVelocity;
    unsigned long lastStep = 0;

public:
    long motorSteps = 0;
    stepperMotor(byte stepPin, byte directionPin, byte enablePin, int resolution, int minimumVelocity, int maximumVelocity)
    {
        this->stepPin = stepPin;
        this->directionPin = directionPin;
        this->enablePin = enablePin;
        this->resolution = resolution;
        this->minimumVelocity = minimumVelocity;
        this->maximumVelocity = maximumVelocity;
        init();
    }

    void init()
    {
        pinMode(stepPin, OUTPUT);
        pinMode(directionPin, OUTPUT);
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, 0);
    }
    void run(char velocity)
    {
        digitalWrite(directionPin, ((velocity < 0)) ? 1 : 0);
        velocity = abs(velocity);

        unsigned long stepTime = map(velocity, 0, 100, minimumVelocity, maximumVelocity);

        if (micros() > (lastStep + stepTime))
        {
            digitalWrite(stepPin, 1);
            digitalWrite(stepPin, 0);
            lastStep = micros();
            motorSteps++;
        }
    };
};
