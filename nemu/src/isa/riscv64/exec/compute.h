//U
static inline def_EHelper(lui)
{
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}
//U
static inline def_EHelper(auipc)
{
  Log("0x%lx",id_src1->imm);
  rtl_li(s, ddest, id_src1->imm + s->seq_pc - 4 );
  print_asm_template2(auipc);
}
//I
static inline def_EHelper(addi)
{
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  print_asm_template3(addi);
}
