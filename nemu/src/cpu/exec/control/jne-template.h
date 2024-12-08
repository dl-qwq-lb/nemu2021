#include "cpu/exec/template-start.h"

#define instr jne

static void do_execute() {
    if(cpu.eflags.ZF == 0){
        if(MSB(op_src->val) == 1){
            if(DATA_BYTE == 1){
                int8_t temp_signed = op_src->val;
                cpu.eip += temp_signed;
            }
            else if(DATA_BYTE == 2){
                int16_t temp_signed = op_src->val;
                cpu.eip += temp_signed;
            }
            else{
                int32_t temp_signed = op_src->val;
                cpu.eip += temp_signed;
            }
        }
        else{
            cpu.eip += op_src->val;
        }    
    }
    
    print_asm(str(instr) " %x", cpu.eip + 1 + DATA_BYTE);
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"