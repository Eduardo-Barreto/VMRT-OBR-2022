#define ledLeftTurn 22
#define ledRightTurn 23
#define ledLeftGreen 24
#define ledRightGreen 25
#define ledStatus0 26
#define ledStatus1 27
#define ledStatus2 28
#define ledStatus3 29

class led
{
private:
    byte pin;
    unsigned long lastBlink;

    void init()
    {
        pinMode(pin, OUTPUT);
    }

public:
    bool state;

    led(byte _pin)
    {
        this->pin = _pin;
        this->init();
    }

    void on()
    {
        state = 1;
        digitalWrite(pin, state);
    }

    void off()
    {
        state = 0;
        digitalWrite(pin, state);
    }

    void toggle()
    {
        state = !state;
        digitalWrite(pin, state);
    }

    void blink(int time = 100)
    {
        if (millis() < lastBlink + time)
            return;

        this->toggle();
        lastBlink = millis();
    }
};