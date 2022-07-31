#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(special) {
  switch (s->isa.instr.r.func) {
    default: exec_inv(s);
  }
}


static inline void fetch_decode_exec(DecodeExecState *s) {
  // instruction fetch IF
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);

  switch (s->isa.instr.r.opcode ) {
    EX   (000, special)
    IDEX (017, IU, lui)
    IDEXW(043, ld, ld, 4)
    IDEXW(053, st, st, 4)
    EX   (074, nemu_trap)
    default: exec_inv(s);
  }
  printf("d=%d,x=%x\n",s->isa.instr.r.opcode,s->isa.instr.r.opcode);
  // wth: last decode update pc
  // update_pc(s);
}

static inline void reset_zero() {
  reg_l(0) = 0;
}

vaddr_t isa_exec_once() {
  // decode infomation,option,value,etc
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}
