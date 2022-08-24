char turnSide = 0;
byte triangle = 3;
unsigned long ySteps = 0;
unsigned long xSteps = 0;
int distanceMid = 0;

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

    catchBall();
    unsigned long timeout = millis() + 3000;
    while (millis() < timeout)
    {
        robot.move(-60, -60);
    }
    gyro.init();
    robot.alignAngle();

    for (int i = 1; i <= 3; i++)
    {
        if (i == 2)
            ySteps = robot.getRightSteps();

        if (i == 3)
            xSteps = robot.getRightSteps();

        robot.alignAngle();
        while (!proximity(centerUltra.read(), 38, 3))
        {
            robot.move(50, 50);
        }
        int distanceWall = (turnSide < 0) ? rightUltra.read() : leftUltra.read();
        if (i == 2)
            ySteps = robot.getRightSteps() - ySteps;
        if (i == 3)
            xSteps = robot.getRightSteps() - xSteps;
        robot.alignAngle();
        greenLED.on();

        while (centerUltra.read() >= 35)
        {
            robot.move(25, 25);
        }
        greenLED.off();

        robot.alignAngle();
        robot.alignUltra(25, 35, 3);

        robot.turn(45 * turnSide, 45);
        catchBall();
        robot.moveCentimeters(35 - distanceWall, 80);

        robot.turn(45 * turnSide, 45);
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
                robot.moveTime(32, 32, 1250);
                openBlocker();
                delay(500);
                closeBlocker();
                delay(250);
                openBlocker();
                delay(500);
                closeBlocker();
                delay(250);
                robot.moveTime(-80, -80, 300);
                robot.turn(45 * turnSide, 60);
                robot.alignAngle();
                robot.turn(45 * turnSide, 60);
                return;
            }
        }
        if (i == 1)
            robot.moveToAngle(convertDegrees(360 + (315 * turnSide)));
        if (i == 2)
            robot.moveToAngle(convertDegrees(360 + (45 * turnSide)));
        if (i == 3)
            robot.moveToAngle(convertDegrees(360 + (135 * turnSide)));

        robot.stop();
        layCatcher();
        if (i == 3)
            break;
        robot.moveTime(30, 30, 500);
        robot.turn(-45 * turnSide, 45);
        robot.alignAngle();
        catchBall();
        robot.moveTime(70, 70, 400);
    }

    if (!proximity((xSteps + 35), (ySteps + 35), robot.centimetersToSteps(20)))
    {
        distanceMid = (sqrt(pow((xSteps + 40), 2) + pow((ySteps + 40), 2))) / 2;
        // 3x3
        for (int j = 1; j <= 3; j++)
        {
            lowerCatcher();
            if (j == 1)
            {
                robot.moveTime(75, 75, 3250);
            }
            else
            {
                robot.moveTime(75, 75, 1625);
            }

            robot.moveTime(-50, -50, 300);

            if (j == 1)
                robot.moveToAngle(convertDegrees(360 + (135 * turnSide)));
            if (j == 2)
                robot.moveToAngle(convertDegrees(360 + (225 * turnSide)));
            if (j == 3)
                robot.moveToAngle(convertDegrees(360 + (315 * turnSide)));

            robot.moveTime(-50, -50, 300);

            layCatcher();
            delay(350);

            if (lowerUltra.read() < 10)
            {
                triangle = j;
                robot.moveTime(70, 70, 1500);
                openBlocker();
                delay(500);
                closeBlocker();
                delay(250);
                openBlocker();
                delay(500);
                closeBlocker();
                delay(250);
                robot.moveTime(-80, -80, 300);
                robot.turn(45 * turnSide, 60);
                robot.alignAngle();
                robot.turn(45 * turnSide, 60);
                return;
            }

            robot.turn(-45 * turnSide, 60);
            robot.alignAngle();
            robot.turn(-90 * turnSide, 60);
            robot.alignAngle();
            robot.turn(-45 * turnSide, 60);
            robot.moveCentimeters(distanceMid, 80);

            robot.turn(-45 * turnSide, 60);
            robot.alignAngle();
            robot.turn(-45 * turnSide, 60);
        }
        // aaa
        delay(999999);
    }
    else
    {
        // 4x3
        delay(999999);
    }
}

void rescue()
{
    findTriangle();
    delay(999999);
    // o contrario da direcao do triangulo
    lowerCatcher();
    robot.moveTime(70, 70, 3000);
    robot.stop(25);
    robot.moveTime(-32, -32, 250);
    raiseCatcher();
    robot.turn(180 * turnSide, 45);
    robot.alignAngle();
    robot.alignUltra(70, 40, 3);
    robot.alignUltra(30, 40);
    robot.moveTime(30, 30, 350);
    robot.alignAngle();
    turnSide = -turnSide;
    robot.turn(45 * turnSide, 45);
    lowerCatcher();
    robot.moveCentimeters(35, 80);
    robot.turn(15 * turnSide, 45);
    robot.moveTime(-32, -32, 650);

    for (int i = 0; i < 3; i++)
    {
        lowerCatcher();
        robot.moveTime(70, 70, 4250);
        robot.stop(25);
        robot.moveTime(-32, -32, 250);
        raiseCatcher();
        robot.alignAngle();
        robot.turn(50 * turnSide, 45);
        robot.moveTime(60, 60, 500);
        robot.turn(40 * turnSide, 45);
        robot.alignAngle();
        robot.moveTime(-32, -32, 350);
        robot.turn(-15 * turnSide, 60);
    }

    robot.alignAngle();
    robot.alignUltra(60, 35, 3);
    robot.turn(45 * turnSide, 45);
    robot.moveCentimeters(20, 45);
    robot.turn(-90 * turnSide, 60);
    robot.moveTime(80, 80, 1500);
    openBlocker();

    robot.turnOffMotors();
    delay(999999);
}