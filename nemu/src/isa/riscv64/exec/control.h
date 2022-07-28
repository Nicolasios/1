static inline def_EHelper(jal)
{
  rtl_li(s, ddest,s->seq_pc);
  rtl_j(s,c_sar(id_src1->imm,12));
  print_asm_template2(jal);
}