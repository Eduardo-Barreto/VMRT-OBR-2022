void interruptMenu()
{
    robot.turnOffMotors();
    bool wdtStarted = false;
    while (!digitalRead(startButton.pin))
    {
        if (!wdtStarted && !digitalRead(F1.pin))
        {
            wdtStarted = true;
            wdt_enable(WDTO_2S);
            wdt_reset();
        }

        if (wdtStarted && digitalRead(F1.pin))
        {
            wdtStarted = false;
            wdt_reset();
            wdt_disable();
        }
    }
    robot.turnOnMotors();
}