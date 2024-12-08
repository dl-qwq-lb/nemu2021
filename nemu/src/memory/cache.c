#include "common.h"
#include "memory/cache.h"
#include "burst.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void ddr3_read_replace(hwaddr_t addr, void* data);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
void ddr3_write_replace(hwaddr_t addr, void* data, uint8_t *mask);

void init_cache(){
    int i;
    for(i = 0; i < Cache_L1_Size / Cache_Block_Size; i ++){
        Cache_L1[i].valid = 0;
    }
}

int read_cache_L1(hwaddr_t addr){
    uint32_t group_index = (addr >> Cache_Blockbits) & (Cache_L1_Groupsize - 1);
    uint32_t tag = (addr >> (Cache_L1_Groupbits + Cache_Blockbits));
    uint32_t block_start = (addr >> Cache_Blockbits) << Cache_Blockbits;

    int group = group_index * Cache_L1_Waysize;
    int i;
    for(i = group + 0; i < group + Cache_L1_Waysize; i++){
        if(Cache_L1[i].valid == 1 && Cache_L1[i].tag == tag){
            return i;
        }
    }
    srand(time(0));
    int idx = group + rand() % Cache_L1_Waysize;
    int j;
    for(j = 0; j < Cache_Block_Size / BURST_LEN; j ++){
        ddr3_read_replace(block_start + BURST_LEN * j, Cache_L1[idx].data + BURST_LEN * j);
    }
    Cache_L1[idx].valid = 1;
    Cache_L1[idx].tag = tag;
    return idx;
}

void write_cache_L1(hwaddr_t addr, size_t len, uint32_t data){
    uint32_t group_index = (addr >> Cache_Blockbits) & (Cache_L1_Groupsize - 1);
    uint32_t tag = (addr >> (Cache_L1_Groupbits + Cache_Blockbits));
    uint32_t offset = addr & (Cache_Block_Size - 1);

     
    int i; //group = group_index * Cache_L1_Waysize;
    for (i = group_index * Cache_L1_Waysize + 0;i < group_index * Cache_L1_Waysize + Cache_L1_Waysize;i ++){
        if(Cache_L1[i].valid == 1 && Cache_L1[i].tag == tag){
            if(offset + len > Cache_Block_Size){
                dram_write(addr, Cache_Block_Size - offset, data);
                memcpy(Cache_L1[i].data + offset, &data, Cache_Block_Size - offset);
                write_cache_L1(addr + Cache_Block_Size - offset, len - (Cache_Block_Size - offset),data >> (8 * (Cache_Block_Size - offset)));
            }
            else{
                dram_write(addr, len, data);
                memcpy(Cache_L1[i].data + offset, &data, len);
            }
            return;
        }
    }
    if(offset + len > Cache_Block_Size){
        dram_write(addr, Cache_Block_Size - offset, data);
        write_cache_L1(addr + Cache_Block_Size - offset, len - (Cache_Block_Size - offset),data >> (8 * (Cache_Block_Size - offset)));
    }
    else{
        dram_write(addr, len, data);
    }
    return;
}

