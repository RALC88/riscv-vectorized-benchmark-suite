static unsigned long get_inst_count()
{
    unsigned long instr;
    asm volatile ("rdinstret %[instr]"
                : [instr]"=r"(instr));
    return instr;
}

static unsigned long get_cycles_count()
{
    unsigned long cycles;
    asm volatile ("rdcycle %[cycles]"
                : [cycles]"=r"(cycles));
    return cycles;
}