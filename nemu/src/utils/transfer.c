#include <common.h>

/* 十进制字符串转int*/
uint64_t char2int(char *c)
{
    int i = strlen(c);
    int j;
    uint64_t res = 0;
    for (j = 0; j < i; j++)
        res = (c[j] - '0') + res * 10;
    return res;
}

/*16进制制度串转addr*/
uint32_t char0X2addr(char *c)
{
    int i = strlen(c);
    int j;
    uint32_t res = 0;
    for (j = 2; j < i; j++)
        res = (c[j] - '0') + res * 16;
    return res;
}