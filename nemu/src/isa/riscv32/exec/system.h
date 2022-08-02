//I wth  t =pc+4; pc=(x[rs1]+sext(offset))&~1; x[rd]=t
static inline def_EHelper(jalr) {
  s->seq_pc=(*dsrc1+id_src2->imm)&(~1);
  rtl_li(s, ddest, s->seq_pc+4);
  
  print_asm_template2(jalr);
}
// I wth 
static inline def_EHelper(ret) {
  def_EHelper(jalr);
}
