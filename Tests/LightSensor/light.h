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
    int raw;
    int light;

    lightSensor(byte pin, int minRead, int maxRead)
    {
        this->pin = pin;
        this->minRead = minRead;
        this->maxRead = maxRead;
        init();
    }

    void read()
    {
        this->raw = analogRead(pin);
        this->light = constrain(map(raw, minRead, maxRead, 100, 0), 0, 100);
    }

    int getRead()
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
