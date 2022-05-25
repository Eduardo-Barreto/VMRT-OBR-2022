#define ultraRightTrigger 30
#define ultraRightEcho 9

#define ultraLeftTrigger 31
#define ultraLeftEcho 10

#define ultraFrontTrigger 999
#define ultraFrontEcho 999

#include <Ultrasonic.h>

class UltrasonicSensor
{
private:
    int timeout;
    unsigned long lastRead;
    unsigned long readTime;
    int distance;
    Ultrasonic *ultra;

public:
    UltrasonicSensor(Ultrasonic *ultra, int timeout)
    {
        this->ultra = ultra;
        this->timeout = timeout;
    }

    int read()
    {
        if (millis() > (lastRead + timeout))
        {
            distance = ultra->read();
            return distance;
        }
    }
};