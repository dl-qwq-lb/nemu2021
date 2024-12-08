#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), R_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, R_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4
make_helper(mov_cr2r){
	uint8_t op = instr_fetch(eip + 1, 1);
	if(op == 0xc0){
		cpu.eax = cpu.cr0.val;
		print_asm("mov cr0 to eax");
	}
	else{
		cpu.eax = cpu.cr3.val;
		print_asm("mov cr3 to eax");
	}
	return 2;
}
make_helper(mov_r2cr){
	uint8_t op = instr_fetch(eip + 1, 1);
	if(op == 0xc0){
		cpu.cr0.val = cpu.eax;
		print_asm("mov eax to cr0");
	}
	else{
		cpu.cr3.val = cpu.eax;
		init_tlb();
		print_asm("mov eax to cr3");
	}
	return 2;
}
#endif
#if DATA_BYTE == 2
make_helper(mov_sreg){
	uint8_t op = instr_fetch(eip+1,1);
	uint8_t csreg = (op >> 3) & 0x7;
	//0xc0 1100_0000 0xd0 1101_0000 0xd8 1101_1000
	cpu.sreg[csreg].sel = reg_w(R_EAX);
	sreg_load(csreg);
	print_asm("mov_sreg");
	return 2;
}
#endif

#include "cpu/exec/template-end.h"
