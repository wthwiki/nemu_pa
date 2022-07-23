
#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "at", "v0", "v1", "a0", "a1", "a2", "a3",
  "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
  "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra"
};

// by wth
word_t getValueByReg(char * str){
  for(int i=0;i<32;i++){
    if(strcmp(str,reg_name(i))==0){
      return reg_l(i);
    } 
  }
  printf("reg name error");
  return 0x80100000;
}

void isa_reg_display() {
  printf("show the reg info\n");
  int num=sizeof(cpu.gpr)/sizeof(cpu.gpr[0]);
  for(int i=0;i<num;i++){
    printf("%s, valuex = %x, valued=%d\n", reg_name(i), reg_l(i),reg_l(i));
  }
}

word_t isa_reg_str2val(const char *s, bool *success) {
  for(int i=0;i<32;i++){
    if(strcmp(s,reg_name(i))==0){
      return reg_l(i);
    } 
  }
  printf("reg name error");
  return -1;
}

