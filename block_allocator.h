#ifndef __BLOCK_ALLOCATOR_H__
#define __BLOCK_ALLOCATOR_H__

#include "block_types.h"

/* Memory fragmentasyonunu onlemek icin, bir blok alloc edip, 
	ptrleri atamak istiyorum - SS */

void* set_mainboard_block_ptrs(void* chunk, mainboard_block_t* b);
void* set_processor_block_ptrs(void* chunk, processor_block_t* b);
void* set_psu_block_ptrs(void* chunk, psu_block_t* b);
void* set_ram_block_ptrs(void* chunk, ram_block_t* b);
void* set_storage_dev_ptrs(void* chunk, storage_dev_block_t* b);

#endif