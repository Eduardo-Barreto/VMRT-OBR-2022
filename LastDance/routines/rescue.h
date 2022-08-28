int diagonal = 3550;

char turnSide = 0;
byte triangle = 3;
unsigned long ySteps = 0;
unsigned long xSteps = 0;
int distanceMid = 0;

void alignFrontTriangle(int i)
{
    if (i == 1)
        robot.moveToAngle(convertDegrees(360 + (225 * turnSide)));
    if (i == 2)
        robot.moveToAngle(convertDegrees(360 + (135 * turnSide)));
    if (i == 3)
        robot.moveToAngle(convertDegrees(360 + (315 * turnSide)));
}

void findTriangle()
{
    robot.moveTime(70, 70, 400);
    catchBall();
    if (rightUltra.read() > leftUltra.read())
    {
        turnSide = -1;
        robot.turn(90, 60);
    }
    else
    {
        turnSide = 1;
        robot.turn(-90, 60);
    }

    catchBall(400, false);
    lowerCatcher();
    unsigned long timeout = millis() + 3000;
    while (millis() < timeout)
    {
        robot.move(-60, -60);
    }
    gyro.init();
    robot.alignAngle();
    layCatcher();
    robot.moveTime(75, 75, 700);
    robot.turn(45 * turnSide, 45);

    for (int i = 1; i <= 3; i++)
    {
        alignFrontTriangle(i);
        pickCatcher();
        alignFrontTriangle(i);

        if (i == i)
            robot.moveTime(75, 75, diagonal);
        else
            robot.moveTime(75, 75, diagonal / 2);

        alignFrontTriangle(i);
        catchBall(400, false);
        lowerCatcher();
        delay(250);
        robot.moveTime(-30, -30, 500);
        raiseCatcher();

        robot.turn(45 * turnSide, 45);
        robot.alignAngle();
        robot.turn(90 * turnSide, 45);
        robot.alignAngle();
        robot.turn(45 * turnSide, 45);

        catchBall(400, false);
        lowerCatcher();
        unsigned long timeout = millis() + 1750;
        while (millis() < timeout)
        {
            robot.move(-60, -60);
            if (bumper.isPressed())
            {
                robot.stop();
                raiseCatcher();
                triangle = i;
                robot.moveTime(80, 80, 500);
                robot.turn(-45 * turnSide, 60);
                robot.alignAngle();
                robot.turn(-90 * turnSide, 60);
                robot.alignAngle();
                robot.turn(-45 * turnSide, 60);
                robot.moveTime(32, 32, 1500);
                releaseVictim();
                return;
            }
        }
        robot.moveTime(75, 75, diagonal / 2);
    }
}

void rescue()
{
    findTriangle();
    robot.moveTime(-75, -75, diagonal / 2.25f);
    robot.turn(-45 * turnSide, 45);
    robot.alignAngle();
    catchBall(400, false);
    robot.moveTime(75, 75, 2500);
    robot.alignAngle();
    robot.moveTime(-75, -75, 500);
    layCatcher();
    robot.alignUltra(40, 5, 3);
    robot.alignAngle();
    robot.turn(-90 * turnSide, 45);
    robot.alignAngle();
    lowerCatcher();
    robot.moveTime(75, 75, diagonal / 2);
    robot.moveTime(-35, -35, 400);
    layCatcher();
    robot.alignAngle();

    robot.turn(-90 * turnSide, 60);
    robot.alignAngle();
    robot.turn(-90 * turnSide, 60);
    robot.alignAngle();
    while (!proximity(centerUltra.read(), 38, 3))
    {
        robot.move(50, 50);
    }

    robot.alignAngle();
    while (centerUltra.read() >= 35)
    {
        robot.move(25, 25);
    }

    robot.turn(45 * turnSide, 45);
    robot.moveCentimeters(26, 80);

    robot.turn(-45 * turnSide, 45);
    robot.alignAngle();
    robot.turn(-45 * turnSide, 45);

    robot.moveTime(50, 50, 750);
    releaseVictim();

    for (int j = 1; j <= 3; j++)
    {
        robot.moveTime(-75, -75, 500 * j);
        robot.turn(-45 * turnSide, 45);
        robot.alignAngle();
        robot.turn(-90 * turnSide, 45);

        for (int k = 1; k <= 3; k++)
        {
            robot.alignAngle();
            lowerCatcher();
            robot.moveTime(75, 75, diagonal);
            if (k == 1)
                gyro.init();
            robot.moveTime(-75, -75, 500);
            raiseCatcher();
            robot.alignAngle();
            robot.alignUltra(40, 5 + ((j - 1) * 10), 3);
            robot.alignAngle();
            robot.turn(-90 * turnSide, 45);
            robot.alignAngle();
        }

        // pickCatcher();

        while (!proximity(centerUltra.read(), 38, 3))
        {
            robot.move(50, 50);
        }

        robot.alignAngle();
        catchBall();
        robot.alignAngle();
        int distanceWall = (turnSide < 0) ? rightUltra.read() : leftUltra.read();

        while (centerUltra.read() >= 35)
        {
            robot.move(25, 25);
        }

        robot.turn(-45 * turnSide, 45);
        robot.moveCentimeters(35 - distanceWall, 80);

        robot.turn(45 * turnSide, 45);
        robot.alignAngle();
        robot.turn(45 * turnSide, 45);

        raiseCatcher();
        robot.moveTime(75, 75, 1350);
        releaseVictim();
    }

    robot.die();
}