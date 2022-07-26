#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args)
{
  return -1;
}

static int cmd_help(char *args);

static int cmd_si_n(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct
{
  char *name;
  char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "显示所有支持的指令的概要信息", cmd_help},
    {"c", "继续运行程序", cmd_c},
    {"q", "退出 NEMU", cmd_q},

    /* TODO: Add more commands */
    {"si", "si [N]:让程序单步执行N条指令后暂停执行,当N没有给出时, 缺省为1", cmd_si_n},
    {"info", "info r:打印寄存器状态;info w:打印监视点信息(目前只实现了info r)", cmd_info},
    {"x", "x N EXPR(x 10 0x100000):将EXPR作为起始内存地址, 以十六进制形式输出连续的N个4字节", cmd_x},
    {"p", "p Expr 求出表达式的值", cmd_p},
    {"w", "w EXPR 当表达式EXPR的值发生变化时, 暂停程序执行", cmd_w},
    {"d", "d N 删除序号为N的监视点", cmd_d},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL)
  {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++)
    {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else
  {
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(arg, cmd_table[i].name) == 0)
      {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si_n(char *args)
{
  char *arg = strtok(NULL, " ");
  uint64_t n;
  if (arg == NULL)
  {
    n = 1;
  }
  else
  {
    n = char2int(arg);
  }
  Log("单步执行%ld条指令后停止", n);
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL)
  {
    Log("请输入参数:w(监视点信息)或者r(寄存器状态)");
  }
  else if (strcmp(arg, "r") == 0)
  {
    Log("打印寄存器信息");
    isa_reg_display();
  }
  else if (strcmp(arg, "w") == 0)
  {
    Log("打印监控点信息");
    is_wp_display();
  }
  else
  {
    Log("info指令不支持参数%s,仅支持 info w 与info r", arg);
  }
  return 0;
}

static int cmd_x(char *args)
{
#ifdef ISA64
#define READ_BATCH 4
#else
#define READ_BATCH 2
#endif
  char *arg = strtok(args, " ");
  if (arg == NULL)
  {
    Log("参数 N EXPR 缺失。指令格式为:x N EXPR(x 10 0x100000)");
    return 1;
  }
  int n = atoi(arg);
  char *EXPR = strtok(NULL, " ");
  if (EXPR == NULL)
  {
    Log("参数 EXPR 缺失。指令格式为:x N EXPR(x 10 0x100000)");
    return 1;
  }
  if (strtok(NULL, " ") != NULL)
  {
    Log("参数过多。指令格式为:x N EXPR(x 10 0x100000)");
    return 1;
  }

  //获取地址
  char *ptr;
  paddr_t begin_addr = strtol(EXPR, &ptr, 16);
  printf("   Addr   |   data   \n");
  int i;
  for (i = 0; i < n; i++)
  {
    printf("0x%08x:0x%08lx\n", begin_addr + i * READ_BATCH, paddr_read(begin_addr + i * READ_BATCH, READ_BATCH));
  }
  return 1;
}

static int cmd_p(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL)
  {
    Log("表达式缺失");
  }
  else
  {
    exprcal(arg);
  }
  return 0;
}

static int cmd_w(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL)
  {
    Log("表达式缺失");
  }
  else
  {
    WP *new = new_wp();
    res_t res = exprcal(arg);
    new->res = res;
    strcpy(new->expr, arg);
    Log("新增监测点%d : %s = %ld", new->NO, new->expr, new->res);
  }
  return 0;
}

static int cmd_d(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL)
  {
    Log("监视点序号缺失");
  }
  else
  {
    int number = atoi(arg);
    free_wp(number);
  }
  return 0;
}

void ui_mainloop()
{
  if (is_batch_mode())
  {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL;)
  {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL)
    {
      continue;
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end)
    {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0)
      {
        if (cmd_table[i].handler(args) < 0)
        {
          return;
        }
        break;
      }
    }

    if (i == NR_CMD)
    {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}
