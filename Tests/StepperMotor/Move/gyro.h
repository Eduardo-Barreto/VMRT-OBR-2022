// Biblioteca criada para gerenciar as leituras de ângulos do MPU6050
// Por Equipe VMRT

// Inclui as bibliotecas utilizadas
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>

// Cria as variáveis globais de ângulos
float AxisX, AxisY, AxisZ;

// Cria a instância do sensor
MPU6050 mpu;

// Variáveis de controle do MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
// uint8_t teapotPacket[14] = {'$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n'};

/*
    @brief      Inicializa o MPU com as configurações necessárias
*/
void mpuSetup()
{
    Wire.begin();
    Wire.setClock(400000);

    mpu.initialize();

    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (devStatus == 0)
    {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();

        mpu.setDMPEnabled(true);

        packetSize = mpu.dmpGetFIFOPacketSize();
    }
}

/*
    @brief      Lê os dados do MPU e atualiza as variáveis globais de ângulo:
    AxisX, AxisY, AxisZ
*/
void ReadGyro()
{
    // Lê os dados do MPU
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    }

    // Normaliza os ângulos e atualiza as variáveis globais
    AxisX = ypr[0] * 180 / M_PI + 179.99;
    AxisY = ypr[1] * 180 / M_PI + 179.99;
    AxisZ = ypr[2] * 180 / M_PI + 179.99;
}
