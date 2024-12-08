#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);

        update_eflags_pf_zf_sf((DATA_TYPE_S)result);
        cpu.eflags.CF = cpu.eflags.OF = 0;


	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)

make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(and_si2rm_, SUFFIX)) {
	int len = decode_si2rm_l(eip + 1);
	int8_t temp_src = op_src->val;
	if(DATA_BYTE == 4){
		int32_t result = op_dest->val & temp_src;
		OPERAND_W(op_dest, result);
    
		update_eflags_pf_zf_sf((DATA_TYPE_S)result);
		cpu.eflags.CF = cpu.eflags.OF = 0;

		print_asm_template2();

		return len + 1;
	}
	else{
		int16_t result = op_dest->val & temp_src;
		OPERAND_W(op_dest, result);
    
		update_eflags_pf_zf_sf((DATA_TYPE_S)result);
		cpu.eflags.CF = cpu.eflags.OF = 0;

		print_asm_template2();

		return len + 1;
	}
	
}


#include "cpu/exec/template-end.h"
