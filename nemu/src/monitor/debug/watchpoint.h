#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>
#include <debug.h>

typedef struct watchpoint
{
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  res_t res;
  char expr[256];

} WP;

WP *new_wp();

void free_wp(int NO);

WP *gethead();

WP *getfree();

void is_wp_display();

#endif
