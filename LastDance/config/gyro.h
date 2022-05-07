// Biblioteca criada para gerenciar as leituras de ângulos do MPU6050
// Por Equipe VMRT

// Inclui as bibliotecas utilizadas
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>

class gyroSensor
{
private:
    // Instância do sensor
    MPU6050 mpu;

    // Variáveis de controle do MPU
    uint8_t devStatus;      // Status de cada operação (0 = sucesso, 1 = erro)
    uint16_t packetSize;    // Tamanho esperado do pacote DMT (default is 42 bytes)
    uint16_t fifoCount;     // Quantidade de bytes atualmente na FIFO
    uint8_t fifoBuffer[64]; // buffer da FIFO

    // Variáveis de movimentação e orientação
    Quaternion q;        // [w, x, y, z]         quaternion container
    VectorFloat gravity; // [x, y, z]            gravity vector
    float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

public:
    float Yaw, Pitch, Roll;

    gyroSensor(){}

    /*
        @brief      Inicializa o MPU com as configurações necessárias
    */
    void init()
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
        @brief      Lê os dados do MPU e atualiza as variáveis de ângulo:
                    Yaw, Pitch, Roll
    */
    void read()
    {
        // Lê os dados do MPU
        if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
        {
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        }

        // Normaliza os ângulos e atualiza as variáveis
        Yaw = ypr[0] * 180 / M_PI + 179.99;
        Pitch = ypr[1] * 180 / M_PI + 179.99;
        Roll = ypr[2] * 180 / M_PI + 179.99;
    }
};
