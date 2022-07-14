#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum
{
  TK_NOTYPE = 256,
  TK_EQ,

  /* TODO: Add more token types */
  TK_PLUS,
  TK_MINUS,
  TK_MUL,
  TK_DIV,
  TK_LEFT,
  TK_RIGHT,
  TK_NUMBER10,
  TK_NUMBER16,
  TK_REG,
};

static struct rule
{
  char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE},                  // 空格
    {"\\+", TK_PLUS},                   // +
    {"==", TK_EQ},                      // ==
    {"-", TK_MINUS},                    // -
    {"\\*", TK_MUL},                    // *
    {"/", TK_DIV},                      // /
    {"\\(", TK_LEFT},                   // (
    {"\\)", TK_RIGHT},                  // )
    {"0[xX][0-9a-fA-F]+", TK_NUMBER16}, //十六进制整数,在这里做了一个小设计 是的十六进制数先被识别
    {"[0-9]+", TK_NUMBER10},            //十进制整数
    {"[\\$astrg][ap0-9]", TK_REG},      //寄存器编号
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++)
  {
    //编译正则匹配表达式到一个形式re
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0)
    {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token
{
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(char *e)
{
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0')
  {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++)
    {
      //执行匹配
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
      {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        nr_token++;
        /*
         * TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type)
        {
        case TK_DIV:
          // tokens[nr_token].str = substr_start;
          break;
        default:
          TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX)
    {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

word_t expr(char *e, bool *success)
{
  if (!make_token(e))
  {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();
  printf("1+1+2+(0X1223abc31+222)");

  return 0;
}
