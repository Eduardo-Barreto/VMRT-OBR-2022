
void lowerCatcher()
{
    if (catcher.pos == 0)
        return;
    catcher.write(0);
}

void raiseCatcher()
{
    if (catcher.pos == 90)
        return;
    catcher.write(90);
}

void layCatcher()
{
    if (catcher.pos == 160)
        return;
    catcher.write(160);
}

void toggleCatcher()
{
    if (catcher.pos == 90)
        layCatcher();
    else if (catcher.pos == 0)
        raiseCatcher();
    else if (catcher.pos == 160)
        lowerCatcher();
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
    if (catcherBlocker.pos == 110)
        return;
    catcherBlocker.write(110);
}

void closeBlocker()
{
    if (catcherBlocker.pos == 85)
        return;
    catcherBlocker.write(85);
}
