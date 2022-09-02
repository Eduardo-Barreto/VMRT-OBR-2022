class Servo
{
private:
    byte pin;          // Pino do servo
    int minPulseWidth; // Largura mínima do pulso
    int maxPulseWidth; // Largura máxima do pulso
    int refreshRate;   // Tempo entre pulsos

public:
    byte pos; // Posição atual do servo

    /**
     * @brief Construtor do servo
     *
     * @param pin: (byte) Pino do servo
     * @param initPosition: (byte) Posição inicial do servo
     * @param minPulseWidth: (int) Largura mínima do pulso
     * @param maxPulseWidth: (int) Largura máxima do pulso
     * @param refreshRate: (int) Tempo entre pulsos
     */
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

    /**
     * @brief Envia o servo para uma posição
     *
     * @param pos: (byte) Posição do servo
     */
    void write(byte targetPos)
    {
        targetPos = constrain(targetPos, 0, 180);
        int delayToMove = map(targetPos, 0, 180, minPulseWidth, maxPulseWidth);
        for (int i = 0; i <= abs(targetPos - this->pos); i++)
        {
            digitalWrite(pin, HIGH);
            delayMicroseconds(delayToMove);
            digitalWrite(pin, LOW);
            delayMicroseconds(refreshRate - delayToMove);
        }
        this->pos = targetPos;
    }

    /**
     * @brief Retorna a posição atual do servo
     * @return (byte) Posição atual do servo
     */
    byte read()
    {
        return this->pos;
    }
};
