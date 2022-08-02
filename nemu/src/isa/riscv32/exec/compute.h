static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

// wth
// static inline def_rtl(sm, const rtlreg_t* addr, word_t offset, const rtlreg_t* src1, int len) {
//    vaddr_write(*addr + offset, *src1, len);
// }
// M[x[rs1] + sext(offset) = x[rs2][31: 0]
static inline def_EHelper(sw) {
  rtl_sm(s, dsrc1,id_src2->imm,ddest,4);
  // print_asm_template2(lui);
}