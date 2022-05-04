#include "stepperMotor.h"

stepperMotor motorLeft(
    2,    // Step pin
    5,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
);

stepperMotor motorRight(
    3,    // Step pin
    6,    // Direction pin
    8,    // Enable pin
    400,  // Resolution
    7000, // Minimum velocity
    400   // Maximum velocity
);

void setup()
{
}
long time = 0;
int c = 0;
void loop()
{
  if (millis() > time)
  {
    c = c >= 100 ? c : c + 1;
    time = millis() + 10;
  }
  motorLeft.run(c);
  motorRight.run(c);
}
