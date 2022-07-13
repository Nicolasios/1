#include <common.h>

void char2int(char *c)
{
    Log("%s转换", c);
    int i = strlen(c);
    int j;
    int res = 0;
    for (j = 0; j < i; j++)
    {
        printf("%d\n", c[j] - '0');
        res = (c[j] - '0') + res * 10;
    }
    printf("%d\n", res);
    Log("%d", res);
}