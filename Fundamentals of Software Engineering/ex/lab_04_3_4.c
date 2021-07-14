#include <stdio.h>
#include <string.h>

#define MAX_STR_LEN 256
#define MAX_WORD_LEN 16
#define OK 0
#define ERROR_IO -1
#define ERROR_RANGE -2

// input 1 string
int input(char *s)
{
    if (!fgets(s, MAX_STR_LEN + 2, stdin))
        return ERROR_IO;
    if (strlen(s) > MAX_STR_LEN && s[strlen(s) - 1] != '\n')
         return ERROR_RANGE;
    return OK;
}

// check if ch is a separator
int is_separator(char ch)
{
    char a[9] = { '.', ',', ':', '!', '?', '-', ';', ' ', '\n' };
    for (int i = 0; i < sizeof(a); i++)
        if (ch == a[i])
            return 1;
    return 0;
}

//return the beginning and ending positions of a word
int get_word_pos(const char *s, int *ibeg, int *iend)
{
    int i = *ibeg;
    while (s[i] && is_separator(s[i]))
        i++;
    if (s[i])
    {
        *ibeg = i;
        while (s[i] && !is_separator(s[i]))
            i++;
        *iend = i - 1;
        return 1;
    }
    return 0;
}

// assign each word of string s to a element of a
int divide(const char *s, char a[][MAX_WORD_LEN + 1], int *n)
{
    int ibeg = 0, iend = 0, rc = OK;
    *n = 0;
    while (get_word_pos(s, &ibeg, &iend) && rc == OK)
    {
        strncpy(a[*n], s + ibeg, iend - ibeg + 1);
        if (iend - ibeg + 1 > MAX_WORD_LEN)
            rc = ERROR_IO;
        a[*n][iend - ibeg + 1] = '\0';
        ibeg = iend + 1;
        (*n)++;
    }
    return rc;
}

// return the number of occurences of s in a
int check(char a[][MAX_WORD_LEN + 1], int n, char *s)
{
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (strcmp(a[i], s) == 0)
            cnt++;
    return cnt;
}

// print out words that are not present in both arrays
int process(char a1[][MAX_WORD_LEN + 1], char a2[][MAX_WORD_LEN + 1], int n1, int n2)
{
    int cnt = 0;
    for (int i = 0; i < n1; i++)
        if (check(a2, n2, a1[i]) == 0 && check(a1, n1, a1[i]) == 1)
        {
            cnt++;
            if (cnt == 1)
                printf("Result:");
            printf("%s ", a1[i]);
        }
    for (int i = 0; i < n2; i++)
        if (check(a1, n1, a2[i]) == 0 && check(a2, n2, a2[i]) == 1)
        {
            cnt++;
            if (cnt == 1)
                printf("Result:");
            printf("%s ", a2[i]);
        }
    if (cnt == 0)
        return ERROR_IO;
    return OK;
}


int main()
{
    char s1[MAX_STR_LEN + 1], s2[MAX_STR_LEN + 1];
    char a1[MAX_STR_LEN][MAX_WORD_LEN + 1], a2[MAX_STR_LEN][MAX_WORD_LEN + 1];
    int n1 = 0, n2 = 0, rc;
    printf("Input the first string : ");
    rc = input(s1);
    if (rc == OK)
    {
        printf("Input the second string : ");
        rc = input(s2);
    }

    if (rc == OK)
        rc = divide(s1, a1, &n1);

    if (rc == OK)
        rc = divide(s2, a2, &n2);

    if (rc == OK)
        rc = process(a1, a2, n1, n2);

    if (rc == ERROR_IO)
        printf("Input error");
    return rc;
}

