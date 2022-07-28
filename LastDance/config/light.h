/*
Módulo de configuração dos sensores de luz
*/

/* #define light0Pin A7
#define light1Pin A8
#define light2Pin A9
#define light3Pin A10
#define light4Pin A11
#define light5Pin A12
#define light6Pin A13

#define green0Pin A14
#define green1Pin A15

int light[7];
int green[2]; */

class lightSensor
{
private:
    byte pin; // Pino do sensor de luz

    void init()
    {
        // Configura o pino do sensor de luz
        pinMode(pin, INPUT);
    }

public:
    byte blackThreshold; // Valor mínimo para considerar que o sensor está lendo preto
    int minRead;         // Valor mínimo lido na calibração
    int maxRead;         // Valor máximo lido na calibração
    int minGreen;        // Valor mínimo do intervalo considerado como verde
    int maxGreen;        // Valor máximo do intervalo considerado como verde
    int raw;             // Valor cru lido do sensor de luz
    byte light;          // Valor tratado de iluminação do sensor de luz (0%~100%)
    bool black;          // Indica se o sensor está lendo preto
    byte countGreen;     // Flag de controle para verificar se o verde é real
    bool green;          // Indica se o sensor está lendo verde

    /**
     * @brief Construtor do sensor de luz
     *
     * @param pin: Pino do sensor de luz
     * @param minRead: Valor mínimo lido na calibração
     * @param maxRead: Valor máximo lido na calibração
     * @param blackThreshold: Valor mínimo para considerar que o sensor está lendo preto
     */
    lightSensor(byte pin, int minRead = 1023, int maxRead = 0, byte blackThreshold = 50)
    {
        this->pin = pin;
        this->minRead = minRead;
        this->maxRead = maxRead;
        this->blackThreshold = blackThreshold;
        init();
    }

    /**
     * @brief Atualiza o intervalo de luz considerado como verde
     *
     * @param offset: Valor de offset para o intervalo de luz considerado como verde
     */
    void setGreen(float offset = 5)
    {
        this->read();
        float interval = (maxRead - minRead);
        interval = (interval / 100) * offset;
        this->minGreen = raw - interval;
        this->maxGreen = raw + (interval * 1.5f);
    }

    void forceGreen(float offset = 5)
    {
        float interval = (maxRead - minRead);
        interval = (interval / 100) * offset;
        float raw = minGreen + ((maxGreen - minGreen) / 2);
        this->minGreen = raw - interval;
        this->maxGreen = raw + (interval * 1.5f);
    }

    /**
     * @brief Atualiza o valor lido do sensor de luz
     */
    void read()
    {
        this->raw = analogRead(pin);
        this->light = constrain(map(raw, minRead, maxRead, 100, 0), 0, 100);
        this->black = light <= blackThreshold;
        this->green = (raw >= minGreen && raw <= maxGreen);

        /* if (raw >= minGreen && raw <= maxGreen)
            countGreen++;
        else
        {
            this->green = false;
            countGreen = (countGreen > 0) ? countGreen - 1 : countGreen;
            return;
        }

        if (countGreen >= 6)
            this->green = true; */
    }

    /**
     * @brief Atualiza e retorna o valor cru do sensor de luz
     *
     * @return int: Valor cru lido do sensor de luz
     */
    int getRawRead()
    {
        read();
        return this->raw;
    }

    /**
     * @brief Atualiza e retorna a luz lida no sensor de luz
     *
     * @return int: Valor lido do sensor de luz
     */
    byte getLight()
    {
        read();
        return this->light;
    }

    /**
     * @brief Atualiza e retorna se o sensor está lendo verde
     *
     * @return bool: Indica se o sensor está lendo verde
     */
    bool getGreen()
    {
        read();
        return this->green;
    }
};
