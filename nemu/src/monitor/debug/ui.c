#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include<stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/vaddr.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}
// finsh
static int cmd_c(char *args) {
  // 
  cpu_exec(-1);
  return 0;
}

// finsh
static int cmd_q(char *args) {
  return -1;
}
// finsh
static int cmd_help(char *args);

// by wth
static int cmd_si(char *args){
  char *arg = strtok(NULL," ");
  int num =1 ;
  if (arg == NULL) {
    printf("you should give a num  default 1\n");
  }
  else {
    num= atoi(args);
    printf("PC excute %d instruction\n",num);
  }
  cpu_exec(num);
  return 0;
}

// by wth
static int cmd_info(char *args){
  char *arg = strtok(NULL," ");
  if (arg == NULL) {
    printf("you should give a char like r or w\n");
  }
  else {
    char c=args[0];
    printf("%c\n",c);
    switch (c){
    case 'r':
      isa_reg_display();
      break;
    case 'w':
      show_wp();
      break;
    default:
      printf("you should give the args like r or w\n");
      break;
    } 
  }
  return 0;
}

static int cmd_x(char *args){
  
  char *arg = strtok(NULL," ");
  if (arg == NULL) {
    printf("you should give a num  default 1\n");
  }else{
    char *address=strtok(NULL," ");
    int n=atoi(arg);
    paddr_t addr =(paddr_t)getValueByReg(address);
    int len =4;
    for(int i=0;i<n/4;i++){
      printf("0x%x\t",addr+i*16);
      for(int j=0;j<4;j++){
        word_t add=vaddr_read(addr+i*len +j, len);
        printf("0x%x\t",add);
      }
      printf("\n");
    }
  }
  return 0;
}

static int cmd_p(char *args){
  printf ("args=%s\n",args);
  // char *arg = strtok(NULL," ");
    bool f=false;
    int res= expr(args,&f);
  return res;
}
static int cmd_w(char *args){
  WP* new_w=new_wp();
  char str[10];
  strcpy(str,args);
  printf("args;;;;=%s\n",args);
  new_w->expr=malloc(strlen(args));
  new_w->expr=str;
  printf("new_w->expr;;;; = %s\n",new_w->expr);
  bool f=false;
  int his=expr(args,&f);

  new_w->histroy=his;
  show_wp();
return 0;
}
 

static int cmd_d(char *args){
  del_wp(args);
  // printf("don't have watchpoint you want to delete\n");
  return 0;
}



static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si","debug n ",cmd_si},
  {"info","show the infomation of reg or w",cmd_info},
  {"x","x N Expr: disapper the memary info",cmd_x},
  {"p","p expr:print the expression value",cmd_p},
  {"w","w expr:set the moniter on expr ,when expr value change stop the code",cmd_w},
  {"d","delete the moniter",cmd_d}
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);
    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    // printf("%ld",strlen(cmd));
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }
#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif
    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
