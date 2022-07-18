class Servo
{
private:
    byte pin;
    byte pos;
    int minPulseWidth;
    int maxPulseWidth;
    int refreshRate;

public:
    Servo(byte _pin, byte initPosition = 0, int _minPulseWidth = 544, int _maxPulseWidth = 2400, int _refreshRate = 20000)
    {
        this->pin = _pin;
        this->minPulseWidth = _minPulseWidth;
        this->maxPulseWidth = _maxPulseWidth;
        this->refreshRate = _refreshRate;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        this->write(initPosition);
    }

    void write(byte targetPos)
    {
        targetPos = constrain(targetPos, 0, 180);
        int delayToMove = map(targetPos, 0, 180, minPulseWidth, maxPulseWidth);
        for(int i = 0; i <= abs(targetPos - this->pos); i++){
            digitalWrite(pin, HIGH);
            delayMicroseconds(delayToMove);
            digitalWrite(pin, LOW);
            delayMicroseconds(refreshRate - delayToMove);
        }
        this->pos = targetPos;
    }

    byte read()
    {
        return this->pos;
    }
};
