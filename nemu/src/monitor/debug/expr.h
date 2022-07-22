#ifndef __EXPR_H__
#define __EXPR_H__

#include <common.h>
enum
{
    ERROR_NO_ERROR = 0,
    ERROR_DIV_ZERO,
    ERROR_ILLEAGEL_EXPRESS,
    ERROR_REG_FAIL,
    ERROR_UNSUPPORT_OP,
};

word_t expr(char *, bool *);

#endif
