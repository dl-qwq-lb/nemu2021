#include "cpu/exec/template-start.h"

#define instr lgdt

void do_execute() {
    if(DATA_BYTE == 2){
        cpu.gdtr.limit = MEM_R(op_src->addr,R_DS);
        cpu.gdtr.base = swaddr_read(op_src->addr + 2, 3,R_DS);
    }
    else if(DATA_BYTE == 4){
        cpu.gdtr.limit = swaddr_read(op_src->addr , 2,R_DS);
        cpu.gdtr.base = swaddr_read(op_src->addr + 2, 4,R_DS);
    }
	print_asm_template1();
    
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"