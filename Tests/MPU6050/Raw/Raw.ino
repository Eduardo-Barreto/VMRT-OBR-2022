/*
Teste 0 de acesso ao MPU6050
Objetivo: Obter os valores crus da leitura do sensor (aceleração e giroscópio)
Status: Finalizado com sucesso em 22/04/2022
*/

// Biblioteca responsável pela comunicação com o MPU6050
#include <Wire.h>

// Define o endereço do MPU
const int MPU = 0x68;

// Cria as variáveis que armazenarão os valores lidos (aceleração, temperatura e giroscópio)
int AccelX, AccelY, AccelZ, Temp, GyroX, GyroY, GyroZ;

void setup()
{
    Serial.begin(9600);

    // Inicializa o MPU
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void loop()
{
    // Inicia a leitura dos valores
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 14, true);

    // Lê e trata os valores
    AccelX = Wire.read() << 8 | Wire.read();
    AccelY = Wire.read() << 8 | Wire.read();
    AccelZ = Wire.read() << 8 | Wire.read();
    Temp = Wire.read() << 8 | Wire.read();
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();

    // Imprime os valores lidos
    Serial.print(AccelX);
    Serial.print("\t");
    Serial.print(AccelY);
    Serial.print("\t");
    Serial.print(AccelZ);
    Serial.print("\t");
    Serial.print(Temp);
    Serial.print("\t");
    Serial.print(GyroX);
    Serial.print("\t");
    Serial.print(GyroY);
    Serial.print("\t");
    Serial.println(GyroZ);
    delay(100);
}
