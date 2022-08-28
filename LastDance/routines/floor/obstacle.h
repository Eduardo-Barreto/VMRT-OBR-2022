
void checkObstacle()
{
    if (centerUltra.read() < 20)
    {
        unsigned long timeout = millis() + 1500;
        while (centerUltra.read() > 7)
        {
            targetPower = 60;
            readColors();
            checkTurn();
            runLineFollower(true);
            if (millis() > timeout)
            {
                return;
            }
        }

        alignLine();
        robot.alignUltra(20, 7, 5);
        alignLine();
        robot.alignUltra(20, 7, 5);
        robot.stop();

        gyro.read();
        for (int angle : ortogonals)
        {
            if (proximity(gyro.Yaw, angle, 5))
            {
                robot.alignAngle();
            }
        }
        robot.stop(10);
        robot.turn(90, 60);
        robot.stop(100);
        unsigned long maxSteps = motorRight.motorSteps + robot.centimetersToSteps(5);
        while (leftUltra.read() > 30)
        {
            robot.move(-20, -20);
            if (motorRight.motorSteps > maxSteps)
                break;
        }
        robot.stop(100);

        maxSteps = motorRight.motorSteps + robot.centimetersToSteps(20);
        while (leftUltra.read() <= 30)
        {
            robot.move(20, 20);
            if (motorRight.motorSteps > maxSteps)
                break;
        }
        robot.moveCentimeters(10, 60);
        robot.stop(10);
        robot.turnOneWheel(-90, 70);
        robot.stop(100);

        maxSteps = motorRight.motorSteps + robot.centimetersToSteps(25);
        while (leftUltra.read() > 30)
        {
            robot.move(30, 30);
            if (motorRight.motorSteps > maxSteps)
                break;
        }
        while (leftUltra.read() < 30)
        {
            robot.move(30, 30);
            if (motorRight.motorSteps > maxSteps)
                break;
        }

        robot.moveCentimeters(12, 60);
        robot.stop(10);
        robot.turnOneWheel(-90, 70);
        robot.stop(10);

        maxSteps = motorRight.motorSteps + robot.centimetersToSteps(10);
        while (lineSensors[3].getLight() > blackThreshold + 10)
        {
            robot.move(20, 20);
            if (motorRight.motorSteps > maxSteps)
            {
                break;
            }
        }
        robot.moveCentimeters(8, 60);
        robot.stop(10);
        robot.turn(90, 60);
        robot.stop(10);
        alignLine();

        unsigned long stepOut = motorRight.motorSteps + robot.centimetersToSteps(20);
        while (bumper.isReleased())
        {
            robot.move(-15, -15);
            if (motorRight.motorSteps > stepOut)
                break;
        }
        alignLine();
    }
}
