#include <cpu/exec.h>
#include "rtl.h"

// decode operand helper 译码操作数辅助函数
#define def_DopHelper(name) \
  void concat(decode_op_, name) (DecodeExecState *s, Operand *op, uint32_t val, bool load_val)

//以x86为例, 译码操作数辅助函数会把操作数的信息记录在结构体op中, 
//如果操作数在指令中, 就会通过instr_fetch()将它们从s->seq_pc所指向的内存位置取出. 
//为了使译码操作数辅助函数更易于复用, 函数中将带有一个load_val参数, 
//用于控制是否需要将该操作数读出到译码信息s中供后续使用. 
//例如如果一个内存操作数是源操作数, 
//就需要将这个操作数从内存中读出来供后续执行阶段来使用; 
//如果它仅仅是一个目的操作数, 就不需要从内存读出它的值了, 
//因为执行这条指令并不需要这个值, 而是将新数据写入相应的内存位置.
static inline def_DopHelper(i) {
  op->type = OP_TYPE_IMM;
  op->imm = val;

  print_Dop(op->str, OP_STR_SIZE, "%d", op->imm);
}

static inline def_DopHelper(r) {
  op->type = OP_TYPE_REG;
  op->reg = val;
  op->preg = &reg_l(val);

  print_Dop(op->str, OP_STR_SIZE, "%s", reg_name(op->reg));
}

static inline def_DHelper(IU) {
  decode_op_r(s, id_src1, s->isa.instr.iu.rs, true);
  decode_op_i(s, id_src2, s->isa.instr.iu.imm, true);
  decode_op_r(s, id_dest, s->isa.instr.iu.rt, false);

  print_Dop(id_src2->str, OP_STR_SIZE, "0x%x", s->isa.instr.iu.imm);
}

static inline def_DHelper(ld) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs, true);
  decode_op_i(s, id_src2, s->isa.instr.i.simm, true);
  decode_op_r(s, id_dest, s->isa.instr.i.rt, false);
}

static inline def_DHelper(st) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs, true);
  decode_op_i(s, id_src2, s->isa.instr.i.simm, true);
  decode_op_r(s, id_dest, s->isa.instr.i.rt, true);
}

