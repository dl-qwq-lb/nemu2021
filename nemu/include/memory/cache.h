#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

#define Cache_L1_Size 64 * 1024
#define Cache_Block_Size 64
#define Cache_Blockbits 6

#define Cache_L1_Waysize 8
#define Cache_L1_Waybits 3

#define Cache_L1_Groupsize 128
#define Cache_L1_Groupbits 7

typedef struct{
    uint8_t data[Cache_Block_Size];
    uint32_t tag;
    bool valid;

}Cache_block;



Cache_block Cache_L1[Cache_L1_Size / Cache_Block_Size];

void init_cache();

int read_cache_L1(hwaddr_t addr);

void write_cache_L1(hwaddr_t addr, size_t len, uint32_t data);

#endif