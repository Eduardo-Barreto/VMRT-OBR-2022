// Inclui a biblioteca criada para gerenciar as leituras do MPU6050
#include "gyro.h"
gyroSensor gyro;

void setup()
{ 
  Serial.begin(2000000);
    gyro.init();
}

void loop(){
  gyro.read();
  Serial.println(gyro.Yaw);
}
