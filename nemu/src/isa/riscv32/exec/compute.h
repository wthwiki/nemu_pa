static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}
static inline def_EHelper(li) {
  rtl_li(s, ddest, 0);
  print_asm_template2(li);
}