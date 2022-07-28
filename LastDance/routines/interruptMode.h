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

        if (F2.fallingEdge())
        {
            state = !state;
        }

        if (F3.fallingEdge())
        {
            int adress = sizeof(calibration) * 9 + 1;
            EEPROM.write(adress, gyro.Yaw);
        }
    }
    robot.turnOnMotors();
    greenLED.off();
}