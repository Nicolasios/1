#include <common.h>

void char2int(char *c)
{
    Log("%s转换", c);
    int i = strlen(c);
    int j;
    for (j = 0; j < i; j++)
    {
        printf("%d\n", c[j] - '0');
    }
}