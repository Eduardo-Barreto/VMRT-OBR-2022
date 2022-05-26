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

#include "calc.h"

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
    int minRead;         // Valor mínimo lido na calibração
    int maxRead;         // Valor máximo lido na calibração
    byte blackThreshold; // Valor mínimo para considerar que o sensor está lendo preto
    byte minGreen;       // Valor mínimo do intervalo considerado como verde
    byte maxGreen;       // Valor máximo do intervalo considerado como verde
    int raw;             // Valor cru lido do sensor de luz
    byte light;          // Valor tratado de iluminação do sensor de luz (0%~100%)
    bool black;          // Indica se o sensor está lendo preto
    bool green;          // Indica se o sensor está lendo verde

    /**
     * @brief Construtor do sensor de luz
     *
     * @param pin: Pino do sensor de luz
     * @param minRead: Valor mínimo lido na calibração
     * @param maxRead: Valor máximo lido na calibração
     * @param blackThreshold: Valor mínimo para considerar que o sensor está lendo preto
     */
    lightSensor(byte pin, int minRead = 0, int maxRead = 1023, byte blackThreshold = 50)
    {
        this->pin = pin;
        this->minRead = minRead;
        this->maxRead = maxRead;
        this->blackThreshold = blackThreshold;
        init();
    }

    /**
     * @brief Atualiza o valor lido do sensor de luz
     */
    void read()
    {
        this->raw = analogRead(pin);
        this->light = constrain(map(raw, minRead, maxRead, 100, 0), 0, 100);
        this->black = light >= blackThreshold;
        this->green = (light >= minGreen && light <= maxGreen);
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
     * @brief Atualiza o intervalo de luz considerado como verde
     */
    void setGreen(byte _read, byte _interval)
    {
        this->minGreen = _read - _interval;
        this->maxGreen = _read + _interval;
    }
};
