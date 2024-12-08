#ifndef __TLB_H__
#define __TLB_H__

#include "common.h"

#define TLB_SIZE 64

struct tlb{
    uint32_t tag;
    bool valid;
    uint32_t page;
}tlb[TLB_SIZE];


hwaddr_t TLB_READ(uint32_t addr);
void TLB_WRITE(uint32_t addr,uint32_t num);
void init_tlb();

#endif