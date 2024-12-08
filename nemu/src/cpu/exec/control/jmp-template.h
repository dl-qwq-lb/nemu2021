#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
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
	print_asm(str(instr) " %x", cpu.eip + 1 + DATA_BYTE);
}

make_instr_helper(si)
#if DATA_BYTE == 4
make_helper(jmp_rm_l) {
	int len = decode_rm_l(eip + 1);
	cpu.eip = op_src->val - (len + 1);
	print_asm(str(instr) " *%s", op_src->str);
	return len + 1;
}

make_helper(ljmp){
	uint32_t op1 = instr_fetch(eip+1,4) - 7;
	uint16_t op2 = instr_fetch(eip+5,2);
	cpu.eip = op1;
	cpu.cs.sel = op2;
	uint16_t now_off = (op2 & ~0x7);
	Assert(  now_off <= cpu.gdtr.limit, "WRONG IN Ljmp 0x%x,0x%x",now_off, cpu.gdtr.limit);
	seg_des->first_part = instr_fetch( cpu.gdtr.base + now_off,4);
	seg_des->second_part = instr_fetch( cpu.gdtr.base + now_off + 4,4);
	Assert(seg_des->p,"SEGMENT FAULT!");
	cpu.cs.base = (seg_des->base1 << 16) | (seg_des->base2 << 8) | seg_des->base3;
	cpu.cs.limit = (seg_des->limit1 << 16) | (seg_des->limit2 << 12) | 0xfff;
	print_asm("ljmp 0x%x 0x%x",op2,op1+7);
	return 7;
}

#endif
#include "cpu/exec/template-end.h"
