#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	MEM_W(cpu.edi, MEM_R(cpu.esi, R_DS), R_ES);
	cpu.esi += (cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE);
	cpu.edi += (cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE);
	//Log("%x",cpu.eip);
	//Log("%x",cpu.ecx);
	print_asm("movs" str(SUFFIX) " %%ds:(%%esi),%%es:(%%edi)");
	return 1;
}

#include "cpu/exec/template-end.h"
