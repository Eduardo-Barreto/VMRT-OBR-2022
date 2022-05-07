/*
Módulo responsável pelas funções relacionadas a matemática
*/

/*
    @brief: Retorna se um valor está dentro de um intervalo
    @param: value - valor a ser verificado
    @param: min - valor mínimo do intervalo
    @param: max - valor máximo do intervalo
    @return: boolean - true se o valor está dentro do intervalo, false caso contrário

    Exemplo:
    bool a = interval(5, 1, 10);
    // a = true
*/
bool interval(float value, float min, float max)
{
    return (value >= min && value <= max);
}

/*
    @brief: Garante que um valor está dentro do intervalo de 360º, usando o módulo da divisão
    @param: value - valor a ser convertido
    @return: int - valor convertido

    Exemplo:
    convertDegrees(280+90) = 10
*/
int convertDegrees(float degrees)
{
    return int(degrees) % 360;
}