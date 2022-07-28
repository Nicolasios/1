static inline def_EHelper(jal)
{
  //Log("sqe:0x%x jmp: 0x%x",s->seq_pc,s->jmp_pc);
  rtl_li(s, ddest,s->seq_pc);
  int offset = id_src1->imm;
  // Log("0x%lx   0x%lx",offset,offset>>20);
  rtl_j(s, offset>>20);
  Log("sqe:0x%x jmp: 0x%x  isJmp:%d",s->seq_pc,s->jmp_pc,s->is_jmp);
  print_asm_template2(jal);
}