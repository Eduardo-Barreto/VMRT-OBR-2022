void lowerCatcher()
{
    if (catcher.pos == 0)
        return;
    catcher.write(0);
}

void raiseCatcher()
{
    if (catcher.pos == 100)
        return;
    catcher.write(100);
}

void layCatcher()
{
    if (catcher.pos == 160)
        return;
    catcher.write(160);
}

void toggleCatcher()
{
    if (catcher.pos == 100 || catcher.pos == 90)
        lowerCatcher();
    else if (catcher.pos == 0)
        layCatcher();
    else if (catcher.pos == 160)
        raiseCatcher();
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

void pickCatcher()
{
    if (catcher.pos == 17)
        return;

    catcher.write(17);
}

void catchBall(int time = 400, bool layOnFinish = true)
{
    catcher.write(30);
    openBlocker();
    lowerCatcher();
    robot.moveTime(-70, -70, time);
    delay(100);
    robot.moveTime(70, 70, time);
    if (layOnFinish)
        catcher.write(15);
    closeBlocker();
    delay(250);
    if (layOnFinish)
        layCatcher();
}

void releaseVictim()
{
    raiseCatcher();
    openBlocker();
    delay(500);
    closeBlocker();
    delay(250);
    openBlocker();
    delay(500);
    closeBlocker();
    delay(250);
}