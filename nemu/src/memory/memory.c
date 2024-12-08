#include "common.h"
#include "memory/cache.h"
#include "burst.h"
#include "cpu/reg.h"
#include "nemu.h"


uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */
 
uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int L1_line1 = read_cache_L1(addr);
	uint32_t offset = addr & (Cache_Block_Size - 1);
	uint8_t ret[BURST_LEN << 1];
	if(offset + len > Cache_Block_Size){
		int L1_line2 = read_cache_L1(addr + Cache_Block_Size - offset);
		memcpy(ret, Cache_L1[L1_line1].data + offset, Cache_Block_Size - offset);
		memcpy(ret + Cache_Block_Size - offset, Cache_L1[L1_line2].data, len - (Cache_Block_Size - offset));
	}
	else{
		memcpy(ret, Cache_L1[L1_line1].data + offset, len);
	}

	int temp = 0;
	uint32_t result = unalign_rw(ret + temp, 4) & (~0u >> ((4 - len) << 3));
	return result;
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	write_cache_L1(addr, len, data);
	//dram_write(addr, len, data);
}

hwaddr_t page_translate(lnaddr_t addr) {
	if(cpu.cr0.protect_enable && cpu.cr0.paging) {
		hwaddr_t tmpaddr;
		//printf("!");
		if ((tmpaddr = TLB_READ(addr & 0xfffff000)) != -1){
			return (tmpaddr << 12) + (addr & 0xfff);
		}
		
		PageEntry dir, page;
		uint32_t dir_offset = addr>>22;
		uint32_t page_offset = ((addr>>12) & 0x3ff);
		uint32_t offset = addr & 0xfff;
		dir.val = hwaddr_read((cpu.cr3.page_directory_base << 12) + (dir_offset << 2), 4);
		Assert(dir.p, "Invalid Page!");
		page.val = hwaddr_read((dir.base << 12) + (page_offset << 2), 4);
		Assert(page.p, "Invalid Page!");
		TLB_WRITE(addr & 0xfffff000, page.base);
		return (page.base << 12)+offset;
	} else {
	//	printf("?");
		return addr;
	}
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	size_t max_len = ((~addr) & 0xfff) + 1;
    if (len > max_len){
		uint32_t low = lnaddr_read(addr, max_len);
        uint32_t high = lnaddr_read(addr + max_len, len - max_len);
        return (high << (max_len << 3)) | low;
	}
	hwaddr_t hwaddr = page_translate(addr);
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	size_t max_len = ((~addr) & 0xfff) + 1;
    if (len > max_len){
        lnaddr_write(addr, max_len, data & ((1 << (max_len << 3)) - 1));
        lnaddr_write(addr + max_len, len - max_len, data >> (max_len << 3));
        return;
	}
	hwaddr_t hwaddr = page_translate(addr);
	hwaddr_write(hwaddr, len, data);
}

lnaddr_t seg_translate(swaddr_t addr, size_t len,uint8_t sreg){
	if(cpu.cr0.protect_enable == 0) return addr;
	//printf("SEGING!");
	return cpu.sreg[sreg].base + addr;
}

uint32_t swaddr_read(swaddr_t addr, size_t len,uint8_t sreg) {
//#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
//#endif
	lnaddr_t lnaddr = seg_translate(addr,len,sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
//#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
//#endif
	lnaddr_t lnaddr = seg_translate(addr,len,sreg);
	lnaddr_write(lnaddr, len, data);
}

