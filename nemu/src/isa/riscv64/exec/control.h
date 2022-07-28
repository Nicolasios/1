static inline def_EHelper(jal)
{
  rtl_j(s,c_sar(id_src1->imm,12));
  Log("sqe:0x%x jmp: 0x%x",s->seq_pc,s->jmp_pc);
  rtl_li(s, ddest,s->seq_pc);
  print_asm_template2(jal);
}