#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jge-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jge-template.h"
#undef DATA_BYTE