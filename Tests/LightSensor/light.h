class lightSensor
{
private:
    byte pin;

    void init()
    {
        pinMode(pin, INPUT);
    }

public:
    int minRead;
    int maxRead;
    byte blackThreshold;
    byte minGreen;
    byte maxGreen;
    int raw;
    byte light;
    bool black;

    lightSensor(byte pin, int minRead = 0, int maxRead = 1023, byte blackThreshold = 50)
    {
        this->pin = pin;
        this->minRead = minRead;
        this->maxRead = maxRead;
        this->blackThreshold = blackThreshold;
        init();
    }

    void read()
    {
        this->raw = analogRead(pin);
        this->light = constrain(map(raw, minRead, maxRead, 100, 0), 0, 100);
        this->black = light >= blackThreshold;
    }

    byte getLight()
    {
        read();
        return this->light;
    }

    int getRawRead()
    {
        read();
        return this->raw;
    }
};
