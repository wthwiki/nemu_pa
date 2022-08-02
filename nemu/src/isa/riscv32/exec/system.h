//I wth  t =pc+4; pc=(x[rs1]+sext(offset))&~1; x[rd]=t
static inline def_EHelper(jalr) {
  rtl_li(s, ddest, s->seq_pc+4);
  printf("jalr\tid_src2->simm=%x\t\t%x\n",id_src2->simm,id_src2->simm&(~1));
  s->seq_pc=(*dsrc1+id_src2->simm)&(~1);
}
// I wth 
static inline def_EHelper(ret) {
    printf("ret\n");
  def_EHelper(jalr);
}
