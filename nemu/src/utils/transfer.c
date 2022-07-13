#include <common.h>

uint64_t char2int(char *c)
{
    int i = strlen(c);
    int j;
    uint64_t res = 0;
    for (j = 0; j < i; j++)
        res = (c[j] - '0') + res * 10;
    return res;
}

uint64_t char0X2int(char *c)
{
    int i = strlen(c);
    int j;
    uint64_t res = 0;
    for (j = 2; j < i; j++)
        res = (c[j] - '0') + res * 16;
    return res;
}