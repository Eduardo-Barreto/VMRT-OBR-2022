// Inclui a biblioteca criada para gerenciar as leituras do MPU6050
#include "gyro.h"

void setup()
{
    // Inicializa a comunicação serial
    Serial.begin(115200);
    while (!Serial)
    {
    }
    // Inicializa o MPU
    mpuSetup();
}

void loop()
{
    // Atualiza as leituras do MPU e envia via Serial
    ReadGyro();
    Serial.print("  ");
    Serial.print(AxisX);
    Serial.print("  ");
    Serial.print(AxisY);
    Serial.print("  ");
    Serial.println(AxisZ);
}
