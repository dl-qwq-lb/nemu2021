#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat3(instr,_rm_,SUFFIX)){
    int len = concat(decode_rm_,SUFFIX)(eip + 1);
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP), 4,cpu.eip + len + 1, R_SS);
    cpu.eip = op_src->val - len - 1;

    print_asm(str(instr) " %x", cpu.eip + 2);
    return len + 1;
}

make_helper(concat3(instr,_i_,SUFFIX)){
    int len = concat(decode_i_,SUFFIX)(eip+1);
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP),4,cpu.eip + 1 + DATA_BYTE, R_SS);
    cpu.eip += op_src->val;
    
    print_asm(str(instr) " %x", cpu.eip + 1 + DATA_BYTE);
    return len + 1;
}



#include "cpu/exec/template-end.h"