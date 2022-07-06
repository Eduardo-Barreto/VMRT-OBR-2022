// pressed
// released
// falling edge
// rising edge
// wait for press
// wait for press and release

class button
{
private:
    byte pin;                   // Pino do botão
    byte debounceTime;          // Tempo de debounce (falha mecânica)
    unsigned long nextReadTime; // Próxima leitura do botão (ignorando bounce)
    bool _hasChanged;           // Indica se o botão foi alterado

    void init()
    {
        // Configura o pino do botão
        pinMode(pin, INPUT_PULLUP);
    }

public:
    bool state; // Indica se o botão está pressionado ou não (false = pressionado)

    /**
     * @brief Construtor do botão
     *
     * @param pin: (byte) Pino do botão
     * @param debounceTime: (byte) Tempo de debounce (falha mecânica)
     */
    button(byte _pin, byte _debounceTime = 100)
    {
        this->pin = _pin;
        this->debounceTime = _debounceTime;
        this->init();
    }

    /**
     * @brief Atualiza o estado do botão
     *
     * @return bool: Estado do botão (false = pressionado)
     */
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

    /**
     * @brief Indica se o botão foi alterado
     *
     * @return bool: Indica se o botão foi alterado
     */
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

    /**
     * @brief Indica se o botão está pressionado
     *
     * @return bool: Indica se o botão está pressionado
     */
    bool isPressed()
    {
        read();
        return !state;
    }

    /**
     * @brief Indica se o botão está solto
     *
     * @return bool: Indica se o botão está solto
     */
    bool isReleased()
    {
        read();
        return state;
    }

    /**
     * @brief Indica se houve uma borda de subida (alterou de não pressionado para pressionado)
     *
     * @return bool: Indica se houve uma borda de subida
     */
    bool risingEdge()
    {
        return !read() && hasChanged();
    }

    /**
     * @brief Indica se houve uma borda de descida (alterou de pressionado para não pressionado)
     *
     * @return bool: Indica se houve uma borda de descida
     */
    bool fallingEdge()
    {
        return read() && hasChanged();
    }

    /**
     * @brief Espera o botão ser pressionado enquando executa uma ação
     *
     * @param doWhileWait: (void lambda) Ação a ser executada enquanto o botão estiver pressionado
     *
     * @example
     *     button.waitWhilePressed([]() -> void {
     *        // Ação a ser executada enquanto o botão estiver pressionado
     *    });
     */
    void waitForPress(void (*doWhileWait)() = ([]() -> void {}))
    {
        while (!isPressed())
        {
            doWhileWait();
        }
    }

    /**
     * @brief Espera o botão ser pressionado enquanto executa uma ação e depois espera ser solto executando outra ação (lambda)
     *
     * @param doBeforePress: (void lambda) Ação a ser executada enquanto o botão estiver solto (lambda)
     * @param doBeforeRelease: (void lambda) Ação a ser executada enquanto o botão estiver pressionado (lambda)
     *
     * @example
     *      button.waitForPressAndRelease(
     *          []() -> void {
     *              // Ação a ser executada enquanto o botão estiver solto
     *          },
     *          []() -> void {
     *              // Ação a ser executada enquanto o botão estiver pressionado
     *          }
     *      );
     */
    void waitForPressAndRelease(void (*doBeforePress)() = ([]() -> void {}), void (*doBeforeRelease)() = ([]() -> void {}))
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
