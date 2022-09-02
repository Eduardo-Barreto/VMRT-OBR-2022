#define cmTime 29

/**
 * @brief Limita um valor dentro de um intervalo (constrain)
 * @param value: valor a ser limitado
 * @param min: valor mínimo do intervalo
 * @param max: valor máximo do intervalo
 * @return int: valor limitado
 *
 * @example
 *      int a = limit(5, 1, 10);
 *      // a = 5
 *
 *      int b = limit(0, 1, 10);
 *      // b = 1
 *
 *      int c = limit(15, 1, 10);
 *      // c = 10
 */
long limit(long value, long min, long max)
{
    return ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)));
}

class Ultrasonic
{
private:
    byte triggerPin, echoPin;     // Pinos do sensor
    int timeToRead = 100;         // Tempo entre leituras
    long lastReadTime = 0;        // Tempo da última leitura
    unsigned long readingTimeout; // Tempo de timeout da leitura

    /**
     * @brief Converte centímetros para tempo
     *
     * @param cm: (int) Distância em centímetros
     * @return int: Tempo em microssegundos
     *
     * @example
     *     int a = cmToTime(10);
     *      // a = 580
     */
    float centimetersToTime(int cm)
    {
        return (cm * cmTime * 2);
    }

    /**
     * @brief Converte tempo para centímetros
     *
     * @param time: (int) Tempo em microssegundos
     * @return int: Distância em centímetros
     *
     * @example
     *     int a = timeToCentimeters(580);
     *      // a = 10
     */
    float timeToCentimeters(int time)
    {
        return (time / cmTime / 2);
    }

    /**
     * @brief Envia o trigger para o sensor
     */
    void sendTrigger()
    {
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
    }

    /**
     * @brief Lê a distância medida pelo sensor
     * @return int: Distância em centímetros
     */
    int measure()
    {
        long previousMicros = micros();
        while (!digitalRead(echoPin) && (micros() - previousMicros) <= readingTimeout)
        {
        }
        previousMicros = micros();
        while (digitalRead(echoPin) && (micros() - previousMicros) <= readingTimeout)
        {
        }
        return timeToCentimeters(micros() - previousMicros); // duration
    }

public:
    int lastRead = 0; // Valor da última leitura

    /**
     * @brief Construtor da classe
     *
     * @param triggerPin: (byte) Pino do trigger
     * @param echoPin: (byte) Pino do echo
     * @param readingTimeout: (unsigned long) Tempo de timeout da leitura
     */
    Ultrasonic(byte _triggerPin, byte _echoPin, unsigned long _readingTimeout = 20000UL)
    {
        this->triggerPin = _triggerPin;
        this->echoPin = _echoPin;
        this->lastReadTime = 0;
        this->readingTimeout = _readingTimeout;
        pinMode(this->triggerPin, OUTPUT);
        pinMode(this->echoPin, INPUT);
    }

    /**
     * @brief Lê a distância medida pelo sensor
     * @return int: Distância em centímetros
     */
    int read()
    {
        if (millis() < lastReadTime + timeToRead)
            return lastRead;

        sendTrigger();
        lastRead = limit(measure(), 0, 150);
        lastReadTime = millis();
        return lastRead;
    }
};
