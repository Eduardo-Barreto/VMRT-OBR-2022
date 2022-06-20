// pressed
// released
// falling edge
// rising edge
// wait for press
// wait for press and release

class button
{
private:
    byte pin;
    byte debounceTime;
    unsigned long nextReadTime;
    bool _hasChanged;

    void init()
    {
        pinMode(pin, INPUT_PULLUP);
    }

public:
    bool state;

    button(byte _pin, byte _debounceTime = 100)
    {
        this->pin = _pin;
        this->debounceTime = _debounceTime;
        this->init();
    }

    bool read()
    {
        if (millis() > nextReadTime && digitalRead(pin) != state)
        {
            state = !state;
            _hasChanged = true;
            nextReadTime = millis() + debounceTime;
        }

        return state;
    }

    bool hasChanged()
    {
        read();
        if (_hasChanged)
        {
            _hasChanged = false;
            return true;
        }
        return false;
    }

    bool isPressed()
    {
        read();
        return !state;
    }

    bool isReleased()
    {
        read();
        return state;
    }

    bool risingEdge()
    {
        return !read() && hasChanged();
    }

    bool fallingEdge()
    {
        return read() && hasChanged();
    }

    void waitForPress(void (*doWhileWait)())
    {
        while (!isPressed())
        {
            doWhileWait();
        }
    }

    void waitForPressAndRelease(void (*doBeforePress)(), void (*doBeforeRelease)())
    {
        while (!isPressed())
        {
            doBeforePress();
        }
        while (!isReleased())
        {
            doBeforeRelease();
        }
    }
};
