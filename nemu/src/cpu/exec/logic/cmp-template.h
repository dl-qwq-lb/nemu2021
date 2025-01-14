#include "cpu/exec/template-start.h"

#define instr cmp
static void do_execute(){
	//Log("%d %d\n",op_dest->val , op_src->val);
	DATA_TYPE result = op_dest->val - op_src->val;

    
    update_eflags_pf_zf_sf((DATA_TYPE_S)result);
	cpu.eflags.CF = result > op_dest->val;
	cpu.eflags.OF = MSB((op_dest->val ^ op_src->val) & (op_dest->val ^ result));

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(cmp_si2rm_, SUFFIX)) {
	int len = decode_si2rm_l(eip + 1);
	int8_t temp_src = op_src->val;
	if(DATA_BYTE == 4){
		int32_t result = op_dest->val - temp_src;

		update_eflags_pf_zf_sf((DATA_TYPE_S)result);
		cpu.eflags.CF = result > op_dest->val;
		cpu.eflags.OF = MSB((op_dest->val ^ temp_src) & (op_dest->val ^ result));

		print_asm_template2();
		return len + 1;
	}
	else{
		int16_t result = op_dest->val - temp_src;
    
		update_eflags_pf_zf_sf((DATA_TYPE_S)result);
		cpu.eflags.CF = result > op_dest->val;
		cpu.eflags.OF = MSB((op_dest->val ^ temp_src) & (op_dest->val ^ result));

		print_asm_template2();

		return len + 1;
	}
	
}


#include "cpu/exec/template-end.h"