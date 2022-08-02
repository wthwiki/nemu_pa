//I wth  t =pc+4; pc=(x[rs1]+sext(offset))&~1; x[rd]=t
static inline def_EHelper(jalr) {
  rtl_li(s, ddest, s->seq_pc+4);
  s->seq_pc=(*dsrc1+id_src2->simm)&(~1);
}
// I wth 
static inline def_EHelper(ret) {
  def_EHelper(jalr);
}
