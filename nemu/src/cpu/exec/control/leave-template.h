#include "cpu/exec/template-start.h"

#define instr leave

make_helper(leave){
    REG(R_ESP) = REG(R_EBP);
    REG(R_EBP) = MEM_R(REG(R_ESP), R_SS);
    REG(R_ESP) += 4;
    
    print_asm(str(instr) " %x", cpu.eip + 1 + DATA_BYTE);
    return 1;
}


#include "cpu/exec/template-end.h"