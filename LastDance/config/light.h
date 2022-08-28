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
    void init()
    {
        // Configura o pino do sensor de luz
        pinMode(pin, INPUT);
    }

public:
    byte pin;         // Pino do sensor de luz
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
    byte light;       // Valor tratado de iluminação do sensor de luz (0%~100%)
    int lightRed;     // Valor tratado de iluminação com luz vermelha
    int lightGreen;   // Valor tratado de iluminação com luz verde
    int lightBlue;    // Valor tratado de iluminação com luz azul
    int RGBRed;       // Porcentagem de vermelho lida do sensor de luz
    int RGBGreen;     // Porcentagem de verde lida do sensor de luz
    int RGBBlue;      // Porcentagem de azul lida do sensor de luz
    byte countGreen;  // Flag de controle para verificar se o verde é real
    bool green;       // Indica se o sensor está lendo verde
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

    byte getLightRed()
    {
        read();
        return this->lightRed;
    }

    byte getLightGreen()
    {
        read();
        return this->lightGreen;
    }

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

    byte getRGBRed()
    {
        readRGB();
        return this->RGBRed;
    }

    byte getRGBGreen()
    {
        readRGB();
        return this->RGBGreen;
    }

    byte getRGBBlue()
    {
        readRGB();
        return this->RGBBlue;
    }

    byte checkGreen(int value)
    {
        return (value >= minGreen && value <= maxGreen);
    }
};
