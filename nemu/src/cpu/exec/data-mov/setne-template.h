#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute() {
    if(cpu.eflags.ZF == 0){
        OPERAND_W(op_src, 1);
    }
    else{
        OPERAND_W(op_src, 0);
    }
    
    print_asm(str(instr) " %x", cpu.eip + 1 + DATA_BYTE);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"