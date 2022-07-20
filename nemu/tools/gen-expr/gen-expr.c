#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
    "#include <stdio.h>\n"
    "int main() { "
    "  unsigned result = %s; "
    "  printf(\"%%u\", result); "
    "  return 0; "
    "}";

//生成0-n的随机数
uint32_t choose(uint32_t n)
{
  uint32_t res = rand() % (n + 1);
  return res;
}
void gen_num()
{
  int a = choose(9);
  char *number[10] = {
      "1",
      "2",
      "3",
      "4",
      "5",
      "6",
      "7",
      "8",
      "9",
      "10"};
  strcat(buf, number[a]);
}

void gen_left()
{
  strcat(buf, "(");
}
void gen_right()
{
  strcat(buf, ")");
}
void gen_rand_op()
{
  int a = choose(3);
  char *op[4] = {"+", "-", "*", "/"};
  strcat(buf, op[a]);
}

static inline void gen_rand_expr()
{
  int a = choose(2);
  switch (a)
  {
  case 0:
    gen_num();
    break;
  case 1:
    gen_left();
    gen_rand_expr();
    gen_right();
    break;
  case 2:
    gen_rand_expr();
    gen_rand_op();
    gen_rand_expr();
    break;
  }
}

int main(int argc, char *argv[])
{
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1)
  {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i++)
  {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0)
      continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
    memset(buf, '\0', 65536);
  }
  return 0;
}
