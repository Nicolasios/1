#include <common.h>

/* 十进制字符串转int*/
word_t char2int(char *c)
{
    int i = strlen(c);
    int j;
    uint64_t res = 0;
    for (j = 0; j < i; j++)
        res = (c[j] - '0') + res * 10;
    return res;
}

/*16进制制度串转addr*/
word_t char0X2int(char *c)
{
    int i = strlen(c);
    int j;
    uint32_t res = 0;
    for (j = 2; j < i; j++)
    {
        switch (c[j])
        {
        case 'A':
            res = 10 + res * 16;
            break;
        case 'B':
            res = 11 + res * 16;
            break;
        case 'C':
            res = 12 + res * 16;
            break;
        case 'D':
            res = 13 + res * 16;
            break;
        case 'E':
            res = 14 + res * 16;
            break;
        case 'F':
            res = 15 + res * 16;
            break;
        case 'a':
            res = 10 + res * 16;
            break;
        case 'b':
            res = 11 + res * 16;
            break;
        case 'c':
            res = 12 + res * 16;
            break;
        case 'd':
            res = 13 + res * 16;
            break;
        case 'e':
            res = 14 + res * 16;
            break;
        case 'f':
            res = 15 + res * 16;
            break;
        default:
            res = (c[j] - '0') + res * 16;
            break;
        }
    }
    return res;
}