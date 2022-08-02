static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}
// wth
static inline def_EHelper(addi) {
  //rtl_addi(s, dest, rz, imm);
  rtl_addi(s, ddest, rz,id_src1->imm);
  print_asm_template2(lui);
}
