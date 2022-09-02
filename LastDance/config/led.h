class led
{
private:
    unsigned long lastBlink; // Última vez que o LED foi piscado

    void init()
    {
        // Configura o pino do LED
        pinMode(pin, OUTPUT);
    }

public:
    byte pin;   // Pino do LED
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
     * @brief Seta um estado para o led
     * @param _state: (bool) Estado do LED
     *
     * @example
     *     meuLed.setState(true);
     *    // LED ligado
     */
    void set(bool _state)
    {
        state = _state;
        digitalWrite(pin, state);
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