/*Лабораторная работа 3. Вариант 3. Задача 3
    Написать программу, которая обрабатывает двоичный файл, содержащий целые числа.
Программа должна уметь
− создавать файл и заполнять его случайными числами;
− выводить числа из файла на экран;
− упорядочивать числа в файле.
*/

/*
- Алгоритм сортировки: Сортировка выбором
- «Направление» упорядочивания по возрастанию
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERR_IO -1
#define ERR_WRONG_CMD -2
#define ERR_NONE -3

#define MAX_VALUE 1000
#define MIN_VALUE -1000

/* Функция напечатывает исключение
*/
void usage(void)
{
    fprintf(stderr, "app.exe <action> <name_file>\n");
    fprintf(stderr, "Action\n");
    fprintf(stderr, "\t w-write_file\n");
    fprintf(stderr, "\t p-print_file\n");
    fprintf(stderr, "\t s-sort_file\n");
}

/* Функция определит количество челых чисел в файле (файл содержит целые числа)
    Функция возвращает код ошибки
* 'fb' - двоичный файл
* 'size' - количество челых чисел, передаваемое по указателю
*/
/*int findsize(FILE *fb, int *size)
{
    int num, ret;
    *size = 0;
    while (fread(&num, sizeof(int), 1, fb) == 1)
        (*size)++;
    if (feof(fb))
        ret = OK;
    else
        ret = ERR_IO;
    rewind(fb);
    return ret;
}*/

int findsize(FILE *fb, int *size)
{
    int pos_end, ret = OK;
    if (fseek(fb, 0, SEEK_END))
        ret = ERR_IO;
    else
    {
        pos_end = ftell(fb);
        if (pos_end % sizeof(int))
            ret = ERR_IO;
        else
            *size = pos_end / sizeof(int);
    }
    rewind(fb);
    return ret;
}

/* Функция заполнять двоичный файл случайными числами
    Функция возвращает код ошибки
* 'fb' - двоичный файл
*/
int write_random_number_to_file(FILE *fb)
{
    int num, n, ret = OK;
    srand(time(0));
    printf("Input number of elements: ");
    if (scanf("%d", &n) != 1)
        ret = ERR_IO;
    else
    {
        for (int i = 0; i < n && ret == OK; i++)
        {   
            num = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
            if (fwrite(&num, sizeof(int), 1, fb) != 1)
                ret = ERR_IO;
        }
    }
    return ret;  
}

/* Функция выводит числа из файла на экран
   Функция возвращает код ошибки
* 'fb' - двоичный файл
*/
int read_and_print(FILE *fb)
{
    int num, ret, k;
    ret = findsize(fb, &k);
    if (ret == OK)
    {
        while (fread(&num, sizeof(int), 1, fb) == 1)
            printf("%d ", num);
        if (k == 0)
            ret = ERR_NONE;
        else
            printf("\n");
    }
    return ret;
}

/* Функция прочитает число в указанной позиции из двоичного файла
   Функция возвращает код ошибки
* 'fb' - двоичный файл
* 'number' - целое числе, передаваемое по указателю
* 'pos' - заданная позиция
*/
int get_number_by_pos(FILE *fb, int *number, int pos)
{
    int ret = OK;
    if (fseek(fb, sizeof(int) * pos, SEEK_SET))
        ret = ERR_IO;
    else if (fread(number, sizeof(int), 1, fb) != 1)
        ret = ERR_IO;
    return ret;
}

/* Функция записывает число в указанную позицию в файл
* Функция возвращает код ошибки
* 'fb' - двоичный файл
* 'number' - целое числе
* 'pos' - заданная позиция
*/
int put_number_by_pos(FILE *fb, int number, int pos)
{
    int ret = OK;
    if (fseek(fb, sizeof(int) * pos, SEEK_SET))
        ret = ERR_IO;
    else if (fwrite(&number, sizeof(int), 1, fb) != 1)
        ret = ERR_IO;
    return ret;
}

/* Функция упорядочивает числа в файле
    Функция возвращает код ошибки
* 'fb' - двоичный файл
*/
int sort_int_num_file(FILE *fb)
{
    int n, ret;
    int number, min_number, index_min_number;

    ret = findsize(fb, &n);
    
    for (int i = 0; i < n - 1 && ret == OK; i++)
    {  
        if ((ret = get_number_by_pos(fb, &number, i)) == OK)
        {
            min_number = number;
            index_min_number = i;
        }
        for (int j = i + 1; j < n && ret == OK; j++)
        {
            if ((ret = get_number_by_pos(fb, &number, j)) == OK && min_number > number)
            {
                min_number = number;
                index_min_number = j;
            }
        }
        if (ret == OK && index_min_number != i)
        {
            if ((ret = get_number_by_pos(fb, &number, i)) != OK);
            else if ((ret = put_number_by_pos(fb, min_number, i)) != OK);
            else if ((ret = put_number_by_pos(fb, number, index_min_number)) != OK);
        }
    }
    if (n == 0)
        ret = ERR_NONE;  // Пустой файл
    return ret;
}

int main(int argc, char *argv[])
{
    int ret;
    FILE *fb;
    setbuf(stdout, NULL);

    if (argc != 3)
    {
        usage();
        ret = ERR_WRONG_CMD;
    }
    else if (strcmp(argv[1], "w") == 0)
    // создавать файл и заполнять его случайными числами
    {
        fb = fopen(argv[2], "wb");
        if (!fb)
        {
            printf("Can't open file\n");
            ret = ERR_IO;
        }
        else 
        {
            if ((ret = write_random_number_to_file(fb)) == OK)
                printf("Write file successfully\n");
            else
                printf("Write file unsuccessfully\n");
            fclose(fb);
        }
    }
    else if (strcmp(argv[1], "p") == 0)
    // выводить числа из файла на экран
    {
        fb = fopen(argv[2], "rb");
        if (!fb)
        {
            printf("Can't open file\n");
            ret = ERR_IO;
        }
        else 
        {
            if ((ret = read_and_print(fb)) == ERR_IO)
                printf("Error IO\n");
            else if (ret == ERR_NONE)
                printf("Error: empty file\n");
            else
                printf("Print file successfully\n");
            fclose(fb);
        }
    }
    else if (strcmp(argv[1], "s") == 0)
    // упорядочивать числа в файле
    {
        fb = fopen(argv[2], "rb+");
        if (!fb)
        {
            printf("Can't open file\n");
            ret = ERR_IO;
        }
        else 
        {
            if ((ret = sort_int_num_file(fb)) == ERR_IO)
                printf("Error IO\n");
            else if (ret == ERR_NONE)
                printf("Error: empty file\n");
            else
                printf("Sort file successfully\n");
            fclose(fb);
        }
    }
    else
    {
        usage();
        ret = ERR_IO;
    }
    return ret;
}
