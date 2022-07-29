bool checkRamp()
{
    unsigned long startTime = 0;
    while (rightUltra.read() < 15 && leftUltra.read() < 15)
    {
        if (startTime == 0)
            startTime = millis();
        if (millis() - startTime > 5000 && (rightUltra.read() < 15 && leftUltra.read() < 15))
            return true;

        targetPower = 35;
        followRamp();
    }
    return false;
}

bool checkFloor()
{
    unsigned long startTime = 0;
    while (interval(centerUltra.read(), 25, 125) && (rightUltra.read() > 20 || leftUltra.read() > 20))
    {
        rightTurnLED.on();
        leftTurnLED.on();
        if (startTime == 0)
            startTime = millis();

        robot.move(50, 50);

        if (millis() > startTime + 750)
        {
            robot.moveCentimeters(10, 60);
            return true;
        }
    }
    return false;
}