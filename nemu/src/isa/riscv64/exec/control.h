static inline def_EHelper(jal)
{
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(jal);
}