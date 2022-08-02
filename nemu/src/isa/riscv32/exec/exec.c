#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

//#define def_EHelper(name) void concat(exec_, name) (DecodeExecState *s)
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

  Log("s->isa.instr.val=%x\n",s->isa.instr.val);
  Log("s->isa.instr.i.opcode6_2=%x\n",s->isa.instr.i.opcode6_2);
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
    IDEX (0b00100, I, li) // wth li
    IDEX (0b01000, S, store)
    IDEX (0b00101, U, lui)// wth auipc
    IDEX (0b01101, I, addi)// wth add i
    IDEX (0b11011, J, jal)// wth jal
    IDEX (0b11001, U, lui)// wth li
    EX   (0b11010, nemu_trap)
    // IDEX (0b01000, S, sw) // wth sw
    default: exec_inv(s);
  }
  // printf("s->isa.jmp_pc=%x\n",s->isa.jmp_pc);
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
