/*
Módulo responsável pelas funções relacionadas a matemática
*/

// #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/**
 * @brief Retorna se um valor está dentro de um intervalo
 * @param value - valor a ser verificado
 * @param min - valor mínimo do intervalo
 * @param max - valor máximo do intervalo
 * @return boolean - true se o valor está dentro do intervalo, false caso contrário
 *
 * @example
 *      bool a = interval(5, 1, 10);
 *      // a = true
 */
#define interval(value, min, max) ((value) >= (min) && (value) <= (max))

#define proximity(value, target, tolerance) (interval(value, target - tolerance, target + tolerance))

/**
 * @brief Garante que um valor está dentro do intervalo de 360º, retornando o módulo da divisão dele por 360
 * @param value - valor a ser convertido
 * @return int - valor convertido
 *
 * @example
 *      int a = convertDegrees(280+90);
 *     // a = 10
 */

int convertDegrees(int value)
{
    return value % 360;
}
