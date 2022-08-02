#ifndef __CPU_DECODE_H__
#define __CPU_DECODE_H__

#include <isa.h>

#define OP_STR_SIZE 40
//     reg              mem          imm
enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };

typedef struct {
  uint32_t type;
  int width;
  union {
    uint32_t reg;
    word_t imm;
    sword_t simm;
  };
  rtlreg_t *preg;
  rtlreg_t val;
  char str[OP_STR_SIZE];
} Operand;

//它用于存放在执行一条指令过程中的译码和执行信息, 包括操作数的类型, 宽度, 值等信息
typedef struct {
  uint32_t opcode;
  vaddr_t seq_pc;  // sequential pc
  uint32_t is_jmp;
  vaddr_t jmp_pc;
  Operand src1, dest, src2;
  int width;
  rtlreg_t tmp_reg[4];
  ISADecodeInfo isa;//ISA相关的
} DecodeExecState;

// decode helper 操作对象(比如从哪里移动到哪里)
#define def_DHelper(name) void concat(decode_, name) (DecodeExecState *s)

#ifdef DEBUG
#define print_Dop(...) snprintf(__VA_ARGS__)
#else
#define print_Dop(...)
#endif

#endif
