#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (2, ld, 4)
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (2, st, 4)
    default: exec_inv(s);
  }
}

//取指, 译码, 执行
static inline void fetch_decode_exec(DecodeExecState *s) {
  // instruction fetch IF  取指(instruction fetch, IF) 
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");

  printf("pc. x=%x\n",s->isa.instr.val);
  printf("d=%x,x=%x\n",s->isa.instr.i.opcode6_2,s->isa.instr.i.opcode1_0);
  /*
  case 0b00000:
    decode_I(s);
    exec_load(s);
  case 0b01000:
    decode_S(s);
    exec_store(s);
  case 0b01101:
    decode_U(s);
    exec_lui(s);
  case 0b11010:
    exec_nemu_trap(s);
  }
  */
 /*decode rules like I S U; 目的是得到指令的操作和操作对象
  */
  switch (s->isa.instr.i.opcode6_2) {  
    IDEX (0b00000, I, load)
    IDEX (0b00100, I, lui) // wth
    IDEX (0b01000, S, store)
    IDEX (0b00101, S, store)
    IDEX (0b01101, U, lui)
    EX   (0b11010, nemu_trap)
    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_l(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}
