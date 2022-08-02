// wth I
static inline def_EHelper(addi) {
  printf("addi_______dsrc1=%x\n",*ddest);
  rtl_addi(s, ddest, dsrc1,id_src2->imm);
  print_asm_template2(addi);
}