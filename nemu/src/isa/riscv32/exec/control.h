// wth
static inline def_EHelper(jal) {
  Log("id_src1->imm=%x\n",id_src1->simm);
  sword_t jump =s->seq_pc+id_src1->simm;
  rtl_j(s, jump);
//   print_asm_template2(li);
}