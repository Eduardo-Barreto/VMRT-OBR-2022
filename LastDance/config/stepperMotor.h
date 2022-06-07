class stepperMotor
{
private:
    byte stepPin;               // Pino para pulsos de step
    byte directionPin;          // Pino para controle de direção
    byte enablePin;             // Pino de habilitação
    int minimumVelocity;        // Velocidade mínima (tempo entre steps)
    int maximumVelocity;        // Velocidade máxima (tempo entre steps)
    unsigned long lastStep = 0; // Tempo do último step
    bool _pulse = 0;            // Flag para indicar se o motor deve mover

public:
    bool lock = 0;                // Trava o motor quando necessário
    unsigned long motorSteps = 0; // Quantidade de steps realizados
    int resolution;               // Resolução do motor (steps por revolução)

    /**
     * @brief Construtor da classe de motor de passo

     * @param stepPin: pino para pulsos de step
     * @param directionPin: pino para controle de direção
     * @param enablePin: pino de habilitação
     * @param resolution: resolução do motor (steps por revolução)
     * @param minimumVelocity: velocidade mínima (tempo entre steps)
     * @param maximumVelocity: velocidade máxima (tempo entre steps)
     *
     * @example
     *      stepperMotor motor(
     *          2,    // Step pin
     *          5,    // Direction pin
     *          8,    // Enable pin
     *          400,  // Resolution
     *          7000, // Minimum velocity
     *          400   // Maximum velocity
     *      );
     */
    stepperMotor(byte stepPin, byte directionPin, byte enablePin, int resolution, int minimumVelocity, int maximumVelocity)
    {
        // Atribui as variáveis de acordo com os parâmetros
        this->stepPin = stepPin;
        this->directionPin = directionPin;
        this->enablePin = enablePin;
        this->resolution = resolution;
        this->minimumVelocity = minimumVelocity;
        this->maximumVelocity = maximumVelocity;

        // Inicializa a configuração do motor
        init();
    }

    /**
     * @brief Inicializa o motor de passo com as configurações
     */
    void init()
    {
        // Configura os pinos do motor como OUTPUT
        pinMode(stepPin, OUTPUT);
        pinMode(directionPin, OUTPUT);
        pinMode(enablePin, OUTPUT);

        // Desabilita o motor
        off();
    }

    /**
     * @brief Configura o motor de passo para mover-se
     * @param velocity: velocidade desejada (0% a 100%)
     *
     * @example
     *      motor.set(100); // Configura o motor para mover-se a 100% de velocidade
     *      motor.run();  // Move o motor
     */
    void set(char velocity)
    {
        if (lock)
        {
            return;
        }
        // Configura a direção do movimento (frente ou trás)
        digitalWrite(directionPin, ((velocity < 0)) ? 1 : 0);

        // Normaliza a velocidade do movimento para um valor entre 0 e 100
        velocity = abs(velocity);
        velocity = constrain(velocity, 0, 100);

        // Calcula o tempo entre steps de acordo com a velocidade desejada
        unsigned long stepTime = map(velocity, 0, 100, minimumVelocity, maximumVelocity);

        // Verifica se o motor pode mover
        if (micros() > (lastStep + stepTime))
        {
            // Marca a variável de pulso para que o motor se mova
            _pulse = 1;
            // Salva o tempo do último step
            lastStep = micros();
            // Incrementa o número de steps do motor
            motorSteps++;
        }
    };

    /**
     * @brief Move o motor de passo em um pulso
     *
     * @example
     *      motor.set(100); // Configura o motor para mover-se a 100% de velocidade
     *      motor.run();  // Move o motor
     */
    void run()
    {
        // Verifica se o motor deve mover
        if (!_pulse)
            return;

        // Move o motor dando um pulso no pino de step
        digitalWrite(stepPin, 1);
        digitalWrite(stepPin, 0);

        // Desabilita a variável de pulso
        _pulse = 0;
    }

    /**
     * @brief Habilita o motor de passo
     */
    void on()
    {
        digitalWrite(enablePin, 0);
    }

    /**
     * @brief Desabilita o motor de passo
     */
    void off()
    {
        digitalWrite(enablePin, 1);
    }
};
