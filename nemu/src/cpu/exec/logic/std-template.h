#include "cpu/exec/template-start.h"

#define instr std

static void do_execute() {
    cpu.eflags.DF = 1;
	print_asm("std");
}

make_instr_helper(p)

#include "cpu/exec/template-end.h"