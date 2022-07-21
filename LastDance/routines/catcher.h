void raiseCatcher()
{
    catcher.write(90);
}

void lowerCatcher()
{
    catcher.write(0);
}

void layCatcher()
{
    catcher.write(160);
}

void catchBall()
{
    lowerCatcher();
    delay(150);
    robot.moveTime(50, 50, 750);
    robot.moveTime(-50, -50, 750);
}

void openBlocker()
{
    catcherBlocker.write(110);
}

void closeBlocker()
{
    catcherBlocker.write(85);
}
