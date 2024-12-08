#include "cpu/exec/template-start.h"

#define instr cld

static void do_execute() {
    cpu.eflags.DF = 0;
	print_asm("cld");
}

make_instr_helper(p)

#include "cpu/exec/template-end.h"