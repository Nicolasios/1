#include "watchpoint.h"
#include "expr.h"
#include <common.h>
#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP *new_wp();

void free_wp(int NO);

WP *gethead();

WP *getfree();

void is_wp_display();

void init_wp_pool()
{
  int i;
  for (i = 0; i < NR_WP; i++)
  {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;     //使用中的监视点的开始
  free_ = wp_pool; //空闲监视点的开始
}

/* TODO: Implement the functionality of watchpoint */
//新增观测点
WP *new_wp()
{
  WP *newWp = NULL;
  if (free_ == NULL)
  {
    Log("监测池没有更多空闲的监测点");
    return NULL;
  }
  else
  {
    // fress
    newWp = free_;
    free_ = free_->next;
    newWp->next = NULL;
  }
  // head 为空
  if (head == NULL)
  {
    head = newWp;
    newWp->NO = 1;
  }
  // head不为空
  else
  {
    //将newWp放到head的末尾
    WP *tmp = head;
    while (tmp->next != NULL)
    {
      tmp = tmp->next;
    }
    tmp->next = newWp;
    newWp->NO = tmp->NO + 1;
  }
  return newWp;
}

//获取使用中head指针
WP *gethead()
{
  return head;
}
//获取空闲块free指针
WP *getfree()
{
  return free_;
}

//使用双指针来判定 释放观测点
void free_wp(int NO)
{
  if (head == NULL)
  {
    Log("监测池没有使用中的监测点");
    return;
  }
  else
  {
    WP *tmp = head;
    if (tmp->next == NULL && tmp->NO == NO)
    {
      Log("dsdasd");
      head = NULL;
      tmp->next = free_;
      free_ = tmp;
      return;
    }
    if (tmp->next == NULL && tmp->NO != NO)
    {
      Log("监测池没有对应的监测点1");
      return;
    }
    if (tmp->next != NULL && tmp->NO == NO)
    {
      head = tmp->next;
      tmp->next = free_;
      free_ = tmp;
      return;
    }
    if (tmp->next != NULL && tmp->NO != NO)
    {
      WP *nn = tmp->next;
      //定位
      while (nn->next != NULL && nn->NO != NO)
      {
        tmp = tmp->next;
        nn = nn->next;
      }
      if (nn->next == NULL && nn->NO != NO)
      {
        Log("监测池没有对应的监测点2");
        return;
      }
      // Log("定位到的表达式为:%s", nn->expr);
      //修改head
      tmp->next = nn->next;
      // Log("tmp:%s--tmp->next:%s", tmp->expr, tmp->next->expr);
      //修改free
      nn->next = free_;
      free_ = nn;
      //修改head的wp的序号tmp之后
      tmp = tmp->next;
      // Log("tmp:%s", tmp->expr);
      while (tmp != NULL)
      {
        // Log("3");
        tmp->NO = tmp->NO - 1;
        tmp = tmp->next;
        // Log("4");
      }
    }
  }
}

void is_wp_display()
{
  WP *tmp = head;
  while (tmp != NULL)
  {
    Log("观测点%d: expr:%s res:%ld", tmp->NO, tmp->expr, tmp->res);
    tmp = tmp->next;
  }
}