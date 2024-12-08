#include "nemu.h"
#include <stdlib.h>
void init_tlb(){
    int i;
    for(i = 0;i < TLB_SIZE;i++){
        tlb[i].valid = 0;
        tlb[i].tag = 0;
        tlb[i].page = 0;
    }
}
hwaddr_t TLB_READ(uint32_t tag){
    int i;
    for(i = 0;i < TLB_SIZE;i++){
        if(tlb[i].valid && (tlb[i].tag == tag) ){
            return tlb[i].page;
        }
    }
    return -1;
}
void TLB_WRITE(uint32_t tag,uint32_t page){
    int i;
    for(i = 0;i < TLB_SIZE;i++){
        if(!tlb[i].valid){
            tlb[i].valid = 1;
            tlb[i].tag = tag;
            tlb[i].page = page;
            return;
        }
    }
    srand(0);
    i = rand()%TLB_SIZE;
    tlb[i].valid = 1;
    tlb[i].tag = tag;
    tlb[i].page = page;
}