#include <common.h>

void char2int(char *c)
{
    Log("%s转换", c);
    int i = strlen(c);
    int count;
    int res = 0;
    for (count = 0; count < i; count++)
    {
        res += (c[i] - '0') * 10;
    }
    Log("%d\n", res);
}