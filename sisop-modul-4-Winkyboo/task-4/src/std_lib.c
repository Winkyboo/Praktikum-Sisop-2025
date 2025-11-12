#include "std_lib.h"

int div(int a, int b)
{
    int count = 0;
    while (a >= b)
    {
        a -= b;
        count++;
    }
    return count;
}

int mod(int a, int b)
{
    while (a >= b)
    {
        a -= b;
    }
    return a;
}

void memcpy(byte* src, byte* dst, unsigned int size)
{
    unsigned int i;
    for (i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

unsigned int strlen(char* str)
{
    unsigned int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

bool strcmp(char* str1, char* str2)
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}

void strcpy(char* src, char* dst)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';}

void clear(byte* buf, unsigned int size)
{
    unsigned int i;
    for (i = 0; i < size; i++)
    {
        buf[i] = 0;
    }
}

void itoa(int num, char* str)
{
    int i;
    int isNegative;
    int rem;
    int s;
    int e;
    char temp;

    i = 0;
    isNegative = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    while (num != 0)
    {
        rem = mod(num, 10);
        str[i++] = rem + '0';
        num = div(num, 10);
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    s = 0;
    e = i - 1;
    while (s < e)
    {
        temp = str[s];
        str[s] = str[e];
        str[e] = temp;
        s++;
        e--;
    }
}
