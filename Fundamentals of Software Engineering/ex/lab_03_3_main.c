/*program work with binary file containing integer numbers
 * program has:
 * creat binary file and fill random integer numbers in file
 * selection sort increasing
 * print sequence numbers on screen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

//program sucessful complete
#define OK 0
// Error command
#define ERR_WRONG_CMD -1
// Error can't open file
#define ERR_FILE -2
// Error range
#define ERR_RANGE -3
// Error I/O
#define ERR_IO -4
// Error empty file
#define ERR_EMPTY -5

// print exception
void usage(void)
{
    fprintf(stderr, "app.exe <action> <file name>\n");
    fprintf(stderr, "action\n");
    fprintf(stderr, "\t c-creat_file\n");
    fprintf(stderr, "\t p-print_file\n");
    fprintf(stderr, "\t s-sort_file\n");
}
// fill n random numbers in binary file f
int fill(FILE *f)
{
    int n, ran_num, rc = OK;
    if (fseek(f, 0, SEEK_SET))
        rc = ERR_IO;
    else
    {
        printf("input n: ");
        if (scanf("%d", &n) != 1)
            rc = ERR_IO;
        if (n > 0)
        {
            srand(time(NULL));
            for (int i = 0; i < n && rc == OK; i++)
            {
                ran_num = rand();
                if (fwrite(&ran_num, sizeof(int), 1, f) != 1)
                    rc = ERR_IO;
            }
        }
        else
            rc = ERR_RANGE;
    }

    return rc;
}
// get number num from binary file f by position pos
int get_number_by_pos(FILE *f, int pos, int *num)
{
    int rc = OK;
    if (fseek(f, pos * sizeof(int), SEEK_SET))
        rc = ERR_IO;
    else if (fread(num, sizeof(int), 1, f) != 1)
        rc = ERR_IO;

    return rc;
}
// put number num in file f by position pos
int put_number_by_pos(FILE *f, int pos, int num)
{
    int rc = OK;
    if (fseek(f, pos * sizeof(int), SEEK_SET))
        rc = ERR_IO;
    else if (fwrite(&num, sizeof(int), 1, f) != 1)
        rc = ERR_IO;

    return rc;
}
// selection sortting sequence n numbers in file f
int selection_sort(FILE *f)
{
    int rc = OK, min_j, num_i, num_j, num_min, i = 0, j = 0;
    while ((rc = get_number_by_pos(f, i, &num_i) == OK))
    {
        min_j = i;
        j = i + 1;
        while ((rc = get_number_by_pos(f, j, &num_j) == OK))
        {
            rc = get_number_by_pos(f, min_j, &num_min);
            if (rc == OK && num_j < num_min)
            {
                min_j = j;
            }
            j++;
        }
        if ((rc = get_number_by_pos(f, i, &num_i) == OK) && (rc = get_number_by_pos(f, min_j, &num_j) == OK))
        {
            rc = put_number_by_pos(f, i, num_j);
            if (rc == OK)
                rc = put_number_by_pos(f, min_j, num_i);
        }
        i++;
    }
    if (i == 0)
        rc = ERR_EMPTY;

    return rc;
}
// print sequence n numbers from file f on screen
int print_on_screen(FILE *f)
{
    int num, rc = OK;
    if (fseek(f, 0, SEEK_SET))
        rc = ERR_IO;
    else
    {
        if (fread(&num, sizeof(int), 1, f) == 1)
        {
            printf("sequence in file:\n");
            printf("%d ", num);
            while (fread(&num, sizeof(int), 1, f) == 1)
            {
                printf("%d ", num);
            }
        }
        else
            rc = ERR_EMPTY;
    }

    return rc;
}
int main(int argc, char *argv[])
{
    int rc;
    FILE *f;
    setbuf(stdout, NULL);
    if (argc != 3)
    {
        rc = ERR_WRONG_CMD;
        printf("Wrong CMD");
        usage();
    }
    else
    {

        if (strcmp(argv[1], "c") == 0)
        {
            f = fopen(argv[2], "wb");
            if (f)
            {
                rc = fill(f);
                if (rc == OK)
                    printf("sequence was created\n");
                fclose(f);
            }
            else
                rc = ERR_FILE;
        }
        else if (strcmp(argv[1], "p") == 0)
        {
            f = fopen(argv[2], "rb");
            if (f)
            {
                rc = print_on_screen(f);
                fclose(f);
            }
            else
                rc = ERR_FILE;
        }
        else if (strcmp(argv[1], "s") == 0)
        {
            f = fopen(argv[2], "rb+");
            if (f)
            {
                rc = selection_sort(f);
                if (rc == OK)
                    printf("sequence was sorted\n");
                fclose(f);
            }
            else
                rc = ERR_FILE;
        }
        else
        {
            rc = ERR_WRONG_CMD;
            printf("Wrong Command\n");
            usage();
        }
        if (rc == ERR_FILE)
            printf("Can't open file\n");
        else if (rc == ERR_IO)
            printf("I/O Error\n");
        else if (rc == ERR_RANGE)
            printf("n out of range\n");
        else if (rc == ERR_EMPTY)
            printf("Error empty file\n");
    }

    return rc;
}
