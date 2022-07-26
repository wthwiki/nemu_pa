#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  char* expr;
  int  histroy;
} WP;
void show_wp();
WP* new_wp();
void free_wp(WP *wp);
void del_wp(char * args);
//
bool check_watchpoints();
#endif
