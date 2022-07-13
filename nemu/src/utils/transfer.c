#include <common.h>

int char2int(char *c)
{
    int i = strlen(c);
    int j;
    int res = 0;
    for (j = 0; j < i; j++)
        res = (c[j] - '0') + res * 10;
    return res;
}