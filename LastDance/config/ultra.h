#define cmTime 29

long limit(long amt, long low, long high)
{
    return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

class Ultrasonic
{
private:
    byte triggerPin, echoPin;
    unsigned long readingTimeout;
    int lastRead = 0;
    int timeToRead = 100;
    long lastReadTime = 0;

    float centimetersToTime(int cm)
    {
        return (cm * cmTime * 2);
    }

    float timeToCentimeters(int time)
    {
        return (time / cmTime / 2);
    }

    void sendTrigger()
    {
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
    }

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
    Ultrasonic(byte _triggerPin, byte _echoPin, unsigned long _readingTimeout = 20000UL)
    {
        this->triggerPin = _triggerPin;
        this->echoPin = _echoPin;
        this->readingTimeout = _readingTimeout;
        pinMode(this->triggerPin, OUTPUT);
        pinMode(this->echoPin, INPUT);
    }

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
