// wth
static inline def_EHelper(jal) {
  Log("s->seq_pc=%x , id_src1->imm=%x\n",s->seq_pc,id_src1->simm);
  
  sword_t jump =s->seq_pc+id_src1->simm-4;
  rtl_j(s, jump);
//   print_asm_template2(li);
}