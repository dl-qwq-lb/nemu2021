#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_,SUFFIX)){
    cpu.eip = MEM_R(REG(R_ESP), R_SS) - 1;
    REG(R_ESP) += 4;
    
    print_asm(str(instr) " %x", cpu.eip + 1);
    return 1;
}

make_helper(concat(ret_i_,SUFFIX)){
    int len = decode_i_w(eip + 1);
    cpu.eip = MEM_R(REG(R_ESP), R_SS);
    REG(R_ESP) += op_src->val;
    
    print_asm(str(instr) " %x", cpu.eip);
    return len + 1;
}



#include "cpu/exec/template-end.h"