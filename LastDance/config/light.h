/*
Módulo de configuração dos sensores de luz
*/

class lightSensor
{
private:
    void init()
    {
        // Configura o pino do sensor de luz
        pinMode(pin, INPUT);
    }

public:
    byte pin;         // Pino do sensor de luz
    byte countGreen;  // Flag de controle para verificar se o verde é real
    byte light;       // Valor tratado de iluminação do sensor de luz (0%~100%)
    bool green;       // Indica se o sensor está lendo verde
    int minRead;      // Valor mínimo lido na calibração
    int maxRead;      // Valor máximo lido na calibração
    int minReadRed;   // Valor mínimo lido na calibração do vermelho
    int maxReadRed;   // Valor máximo lido na calibração do vermelho
    int minReadGreen; // Valor mínimo lido na calibração do verde
    int maxReadGreen; // Valor máximo lido na calibração do verde
    int minReadBlue;  // Valor mínimo lido na calibração do azul
    int maxReadBlue;  // Valor máximo lido na calibração do azul
    int minGreen;     // Valor mínimo do intervalo considerado como verde
    int maxGreen;     // Valor máximo do intervalo considerado como verde
    int raw;          // Valor cru lido do sensor de luz
    int lightRed;     // Valor tratado de iluminação com luz vermelha
    int lightGreen;   // Valor tratado de iluminação com luz verde
    int lightBlue;    // Valor tratado de iluminação com luz azul
    int RGBRed;       // Porcentagem de vermelho lida do sensor de luz
    int RGBGreen;     // Porcentagem de verde lida do sensor de luz
    int RGBBlue;      // Porcentagem de azul lida do sensor de luz
    int minOut;
    int maxOut;

    /**
     * @brief Construtor do sensor de luz
     *
     * @param pin: Pino do sensor de luz
     * @param minRead: Valor mínimo lido na calibração
     * @param maxRead: Valor máximo lido na calibração
     * @param blackThreshold: Valor mínimo para considerar que o sensor está lendo preto
     */
    lightSensor(byte pin, int minOut = 100, int maxOut = 0, int minRead = 1023, int maxRead = 0)
    {
        this->pin = pin;
        this->minRead = minRead;
        this->maxRead = maxRead;
        this->minOut = minOut;
        this->maxOut = maxOut;
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


    /**
     * @brief Configura e recalcula o intervalo de luz considerado como verde
     * @param offset: Valor de offset para o intervalo de luz considerado como verde
    */
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
        this->light = constrain(map(raw, minRead, maxRead, minOut, maxOut), 0, 100);
        this->lightRed = constrain(map(raw, minReadRed, maxReadRed, minOut, maxOut), 0, 100);
        this->lightGreen = constrain(map(raw, minReadGreen, maxReadGreen, minOut, maxOut), 0, 100);
        this->lightBlue = constrain(map(raw, minReadBlue, maxReadBlue, minOut, maxOut), 0, 100);
        this->green = (raw >= minGreen && raw <= maxGreen);
    }

    /**
     * @brief Atualiza o valor bruto lido do sensor RGB
     */
    void readAbsoluteRGB()
    {
        blueLedSensor.off();
        greenLedSensor.off();

        redLedSensor.on();
        delay(100);
        this->lightRed = this->getRawRead();
        delay(100);
        redLedSensor.off();

        greenLedSensor.on();
        delay(100);
        this->lightGreen = this->getRawRead();
        delay(100);
        greenLedSensor.off();

        blueLedSensor.on();
        delay(100);
        this->lightBlue = this->getRawRead();
        delay(100);
        blueLedSensor.off();
    }

    /**
     * @brief Atualiza o valor de luz lido do sensor RGB
     */
    void readRGB()
    {
        blueLedSensor.off();
        greenLedSensor.off();

        redLedSensor.on();
        delay(50);
        byte preRed = this->getLightRed();
        delay(50);
        redLedSensor.off();

        greenLedSensor.on();
        delay(50);
        byte preGreen = this->getLightGreen();
        delay(50);
        greenLedSensor.off();

        blueLedSensor.on();
        delay(50);
        byte preBlue = this->getLightBlue();
        delay(50);
        blueLedSensor.off();

        int totalRGB = (preRed + preGreen + preBlue) + 1;
        this->RGBRed = (byte)(map(preRed, 0, totalRGB, 0, 100));
        this->RGBGreen = (byte)(map(preGreen, 0, totalRGB, 0, 100));
        this->RGBBlue = (byte)(map(preBlue, 0, totalRGB, 0, 100));
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
     * @brief Atualiza e retorna a luz lida no sensor de luz com a cor vermelha
     *
     * @return int: Valor lido do sensor de luz com a cor vermelha
     */
    byte getLightRed()
    {
        read();
        return this->lightRed;
    }

    /**
     * @brief Atualiza e retorna a luz lida no sensor de luz com a cor verde
     *
     * @return int: Valor lido do sensor de luz com a cor verde
     */
    byte getLightGreen()
    {
        read();
        return this->lightGreen;
    }

    /**
     * @brief Atualiza e retorna a luz lida no sensor de luz com a cor azul
     *
     * @return int: Valor lido do sensor de luz com a cor azul
     */
    byte getLightBlue()
    {
        read();
        return this->lightBlue;
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

    /**
     * @brief Atualiza e retorna a luz lida no sensor RGB com a cor vermelha
     *
     * @return int: Valor lido do sensor RGB com a cor vermelha
     */
    byte getRGBRed()
    {
        readRGB();
        return this->RGBRed;
    }

    /**
     * @brief Atualiza e retorna a luz lida no sensor RGB com a cor verde
     *
     * @return int: Valor lido do sensor RGB com a cor verde
     */
    byte getRGBGreen()
    {
        readRGB();
        return this->RGBGreen;
    }

    /**
     * @brief Atualiza e retorna a luz lida no sensor RGB com a cor azul
     *
     * @return int: Valor lido do sensor RGB com a cor azul
     */
    byte getRGBBlue()
    {
        readRGB();
        return this->RGBBlue;
    }

    /**
     * @brief Atualiza e retorna a luz lida no sensor RGB com a cor vermelha
     *
     * @return int: Valor lido do sensor RGB com a cor vermelha
     */
    byte checkGreen(int value)
    {
        return (value >= minGreen && value <= maxGreen);
    }
};
