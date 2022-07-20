#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <debug.h>
#include <stdlib.h>

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
//递归求值
u_int32_t eval(int p, int q);
//生成tokens
static bool make_token(char *e);
//判断表达式的正确性
word_t expr(char *e, bool *success, int res);
//获取主操作符的位置
void getmainop(int p, int q, int *op_type, int *op);
//检查前后括号是否配对
bool check_parenthese(int p, int q);
//检查表达式是否合法
bool check_correct(int p, int q);

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

static Token tokens[256] __attribute__((used)) = {};
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
        case TK_REG:
          tokens[nr_token].type = TK_REG;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_EQ:
          tokens[nr_token].type = TK_EQ;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_PLUS:
          tokens[nr_token].type = TK_PLUS;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_MINUS:
          tokens[nr_token].type = TK_MINUS;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_DIV:
          tokens[nr_token].type = TK_DIV;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_LEFT:
          tokens[nr_token].type = TK_LEFT;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_RIGHT:
          tokens[nr_token].type = TK_RIGHT;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_NUMBER10:
          tokens[nr_token].type = TK_NUMBER10;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        case TK_NUMBER16:
          tokens[nr_token].type = TK_NUMBER16;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          break;
        default:
          //如果匹配到空白符就丢弃
          nr_token--;
          break;
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

word_t expr(char *e, bool *success, int res)
{
  if (!make_token(e))
  {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // Log("tokens:%d", nr_token);
  if (eval(1, nr_token) != res)
  {
    *success = false;
    return 0;
  }
  *success = true;
  return 0;
}

bool check_parenthese(int p, int q)
{
  if (tokens[p].type != TK_LEFT || tokens[q].type != TK_RIGHT)
  {
    return false;
  }
  else
  {
    int flag = 0;
    for (int i = p; i <= q; i++)
    {
      if (tokens[i].type == TK_LEFT)
      {
        flag++;
      }
      if (tokens[i].type == TK_RIGHT)
      {
        flag--;
      }
      if (flag == 0 && i < q)
      {
        return false;
      }
    }
    return true;
  }
}

bool check_correct(int p, int q)
{
  int flag = 0;
  for (int i = p; i <= q; i++)
  {
    if (tokens[i].type == TK_LEFT)
    {
      flag++;
    }
    if (tokens[i].type == TK_RIGHT)
    {
      flag--;
    }
  }
  if (flag != 0)
  {
    return false;
  }
  return true;
}

void getmainop(int p, int q, int *op_type, int *op)
{
  int flag = 0;
  int prior = 0;
  int position = 0;
  for (int i = p; i <= q; i++)
  {
    // printf("type:%d\n", tokens[i].type);
    if (tokens[i].type == TK_LEFT)
    {
      flag++;
    }
    if (tokens[i].type == TK_RIGHT)
    {
      flag--;
    }
    switch (tokens[i].type)
    {
    case TK_DIV:
      if (prior == 0 && flag == 0)
      {
        prior = 1;
        position = i;
      }
      break;
    case TK_MUL:
      if (prior == 0 && flag == 0)
      {
        prior = 1;
        position = i;
      }
      break;
    case TK_PLUS:
      if (prior != 2 && flag == 0)
      {
        prior = 2;
        position = i;
      }
      break;
    case TK_MINUS:
      if (prior != 2 && flag == 0)
      {
        prior = 2;
        position = i;
      }
      break;
    default:
      break;
    }
  }
  *op = position;
  *op_type = tokens[position].type;
}

u_int32_t eval(int p, int q)
{
  if (p > q)
  {
    Assert(0, "bad expression");
  }
  else if (p == q)
  {
    //单一的数字
    return atoi(tokens[p].str);
  }
  else if (check_parenthese(p, q) == true)
  {
    return eval(p + 1, q - 1);
  }
  else if (check_correct(p, q) == true)
  {
    int op_type;
    int op = 0;
    getmainop(p, q, &op_type, &op);
    printf("main op p :%d,main op q :%d ,main op position:%d\n", p, q, op);
    uint32_t val1 = eval(p, op - 1);
    uint32_t val2 = eval(op + 1, q);
    printf("op_type:%d\n", op_type);
    switch (op_type)
    {
    case TK_PLUS:
      return val1 + val2;
    case TK_MINUS:
      return val1 - val2;
    case TK_MUL:
      return val1 * val2;
    case TK_DIV:
      return val1 / val2;
    default:
      assert(0);
    }
  }
  else
  {
    Assert(0, "unfair expression");
  }
}
