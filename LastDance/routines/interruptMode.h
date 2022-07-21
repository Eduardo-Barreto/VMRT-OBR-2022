void interruptMenu()
{
    robot.turnOffMotors();
    bool wdtStarted = false;
    while (!digitalRead(startButton.pin))
    {
        rightTurnLED.off();
        greenLED.on();
        leftTurnLED.off();
        if (!wdtStarted && !digitalRead(F1.pin))
        {
            wdtStarted = true;
            wdt_enable(WDTO_1S);
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
    greenLED.off();
}