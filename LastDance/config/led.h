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
    byte pin;                // Pino do LED
    unsigned long lastBlink; // Última vez que o LED foi piscado

    void init()
    {
        // Configura o pino do LED
        pinMode(pin, OUTPUT);
    }

public:
    bool state; // Indica se o LED está ligado ou desligado

    /**
     * @brief Construtor do LED
     *
     * @param pin: (byte) Pino do LED
     */
    led(byte _pin)
    {
        this->pin = _pin;
        this->init();
    }

    /**
     * @brief Liga o LED
     */
    void on()
    {
        state = 1;
        digitalWrite(pin, state);
    }

    /**
     * @brief Desliga o LED
     */
    void off()
    {
        state = 0;
        digitalWrite(pin, state);
    }

    /**
     * @brief Inverte o estado do led
     */
    void toggle()
    {
        state = !state;
        digitalWrite(pin, state);
    }

    /**
     * @brief Pisca o LED
     *
     * @param delay: (int) Tempo de espera entre os pulsos
     */
    void blink(int time = 100)
    {
        if (millis() < lastBlink + time)
            return;

        this->toggle();
        lastBlink = millis();
    }
};