char turnSide = 0;
byte triangle = 3;
int initAngle = 0;

void findTriangle()
{
    raiseCatcher();
    robot.moveTime(70, 70, 250);
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

    unsigned long timeout = millis() + 3000;
    while (millis() < timeout)
    {
        robot.move(-60, -60);
        if (bumper.isPressed())
            break;
    }
    gyro.init();
    layCatcher();
    robot.moveCentimeters(15, 45);
    robot.turn(-10 * turnSide, 60);

    for (int i = 1; i <= 3; i++)
    {
        while (centerUltra.read() > 45)
        {
            robot.move(50, 50);
        }

        while (centerUltra.read() >= 35)
        {
            robot.move(25, 25);
        }

        robot.alignAngle();
        robot.alignUltra(25, 35, 3);
        robot.turn(45 * turnSide, 45);
        robot.moveCentimeters(20, 80);
        robot.turn(90 * turnSide, 45);
        unsigned long timeout = millis() + 1750;
        while (millis() < timeout)
        {
            robot.move(-60, -60);
            if (bumper.isPressed())
            {
                robot.stop();
                triangle = i;
                robot.moveTime(80, 80, 500);
                robot.turn(-90 * turnSide, 60);
                robot.moveTime(-32, -32, 250);
                return;
            }
        }
        robot.stop();
        robot.turn(-45 * turnSide, 45);
        robot.alignAngle();
    }
}

void rescue()
{
    findTriangle();
    // o contrario da direcao do triangulo
    lowerCatcher();
    robot.moveTime(70, 70, 3000);
    robot.stop(25);
    robot.moveTime(-32, -32, 250);
    raiseCatcher();
    robot.turn(180 * turnSide, 45);
    robot.alignAngle();
    robot.alignUltra(70, 40, 3);
    turnSide = -turnSide;
    robot.turn(45 * turnSide, 45);
    robot.moveCentimeters(35, 80);
    robot.turn(15 * turnSide, 45);
    robot.moveTime(-32, -32, 650);

    for (int i = 0; i < 3; i++)
    {
        lowerCatcher();
        robot.moveTime(70, 70, 4000);
        robot.stop(25);
        robot.moveTime(-32, -32, 250);
        raiseCatcher();
        robot.alignAngle();
        robot.turn(65 * turnSide, 45);
        robot.moveTime(60, 60, 500);
        robot.turn(20 * turnSide, 45);
        robot.moveTime(-32, -32, 250);
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