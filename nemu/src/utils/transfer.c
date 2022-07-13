#include <common.h>

void char2int(char *c)
{
    int i = strlen(c);
    Log("%s转换%d", c, i);
    int count;
    int res = 0;
    for (count = 0; count < i; count++)
    {
        Log("%d\n", c[i] - '0');
    }
    Log("%d\n", res);
}