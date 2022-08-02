// wth x[rd] = pc+4; pc += sext(offset)
static inline def_EHelper(jal) {
  Log("s->seq_pc=%x , id_src1->imm=%x\n",s->seq_pc,id_src1->simm);
  rtl_li(s,ddest,s->seq_pc+4);
  sword_t jump =s->seq_pc+id_src1->simm-4;
  rtl_j(s, jump);
//   print_asm_template2(li);
}

//I wth  t =pc+4; pc=(x[rs1]+sext(offset))&~1; x[rd]=t
static inline def_EHelper(jalr) {
  rtl_li(s, ddest, s->seq_pc+4);
  printf("s->seq_pc=%x,jalr\tid_src2->simm=%x\t\t%x\n",s->seq_pc,id_src2->simm,(*dsrc1+id_src2->simm)&(~1));
  s->seq_pc=((*dsrc1+id_src2->simm)&(-1));
  printf("pc=%x\n",s->seq_pc);
}
// I wth 
static inline def_EHelper(ret) {
    printf("ret\n");
  exec_jalr(s);
}
