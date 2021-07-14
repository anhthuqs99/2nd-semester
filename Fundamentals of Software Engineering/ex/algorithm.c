#include "algorithm.h"

/**
 * @brief Поменять местами самое длтнное и самое короткое слово
 * @param [in, out] a - массив слов
 * @param [in] i_min - индекс мин
 * @param [in] i_max - индекс мак
 */
void Replace_min_max(char a[][MAX_LEN_WORD + 1], int i_min, int i_max)
{
    char temp[MAX_LEN_WORD + 1];
    strcpy(temp, a[i_min]);
    strcpy(a[i_min], a[i_max]);
    strcpy(a[i_max], a[i_min]);
}

/**
 * @brief Находит начальную и конечную позиции словы в строке
 * @param [in] str - строке
 * @param [in, out] ibeg - начальная позиция
 * @param [out] iend - конечная позиция
 * @return Код ошибки
 */
int get_word_by_pos(const char *str, int *ibeg, int *iend)
{
    int rc = OK;

    if (str == NULL)
        rc = ERR_IO;
    else if (ibeg == NULL)
        rc = ERR_IO;
    else if (iend == NULL)
        rc = ERR_IO;
    else if (*ibeg < 0 || *ibeg > strlen(str))
        rc = ERR_RANGE;
    else
    {
        while (str[*ibeg] && str[*ibeg] == ' ')
            (*ibeg)++;
        int i = *ibeg;
        while (str[i] && str[i] != ' ')
            i++;
        *iend = i - 1;
        if (*iend < *ibeg)
            rc = ERR_IO;
    }
    return rc;
}

/**
 * @brief Сохранить строки в массив слов
 * @param [in]  str - Cтрока
 * @param [out] a - массив слов
 * @param [out] n - количество слов
 * @return
 */
int SaveStrArr(char *str, char a[][MAX_LEN_WORD + 1], int *n)
{
    int ibeg, iend, i = 0;
    int rc = OK;
    ibeg = iend = i;

    if (str == NULL)
        rc = ERR_IO;
    else if (strlen(str) > MAX_LEN_SENTENCE)
        rc = ERR_IO;
    else
    {
        while (rc == OK)
        {
            rc = get_word_by_pos(str, &ibeg, &iend);
            if (rc == OK && iend - ibeg + 1 < MAX_LEN_WORD)
            { 
                strncpy(a[i], str + ibeg, iend - ibeg + 1);
                a[i][iend - ibeg + 1] = '\0';
            }
            else
                rc = ERR_RANGE;
        }
    }
    return rc;
}
