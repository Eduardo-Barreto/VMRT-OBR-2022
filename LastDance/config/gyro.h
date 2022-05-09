/*
    Módulo de configuração do sensor giroscópio e acelerômetro MPU 6050
*/

// Inclui as bibliotecas utilizadas
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>

class gyroSensor
{
private:
    MPU6050 mpu; // Instância do sensor

    // Variáveis de controle do MPU
    uint8_t devStatus;      // Status de cada operação (0 = sucesso, 1 = erro)
    uint16_t packetSize;    // Tamanho esperado do pacote DMT (default is 42 bytes)
    uint16_t fifoCount;     // Quantidade de bytes atualmente na FIFO
    uint8_t fifoBuffer[64]; // buffer da FIFO

    // Variáveis de movimentação e orientação
    Quaternion q;        // [w, x, y, z]         Container Quaternion
    VectorFloat gravity; // [x, y, z]            Vetor de gravidade
    float ypr[3];        // [yaw, pitch, roll]   Vetor de Yaw, Pitch e Roll

public:
    float Yaw, Pitch, Roll;

    /**
     * @brief: Construtor da classe
     */
    gyroSensor() {}

    /**
     * @brief: Inicializa o MPU com as configurações necessárias
     *
     * TODO: Verificar Offsets
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

    /**
     * @brief: Lê os dados do MPU e atualiza as variáveis de ângulo:
     *         Yaw, Pitch, Roll
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
