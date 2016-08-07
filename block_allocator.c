#include "block_allocator.h"


void* set_mainboard_block_ptrs(void* chunk, mainboard_block_t* b) {
	int i;
	for (i = 0; i < MAX_DATA_COUNT; ++i) {
		b->data[i] = (void*)((unsigned char*)chunk + (i * mainboard_struct_size()));
	}

	return (void*)((unsigned char*)chunk + (i * mainboard_struct_size()));
}


void* set_processor_block_ptrs(void* chunk, processor_block_t* b) {
	int i;
	for (i = 0; i < MAX_DATA_COUNT; ++i) {
		b->data[i] = (void*)((unsigned char*)chunk + (i * processor_struct_size()));
	}

	return (void*)((unsigned char*)chunk + (i * processor_struct_size()));
}


void* set_psu_block_ptrs(void* chunk, psu_block_t* b) {
	int i;
	for (i = 0; i < MAX_DATA_COUNT; ++i) {
		b->data[i] = (void*)((unsigned char*)chunk + (i * psu_struct_size()));
	}

	return (void*)((unsigned char*)chunk + (i * psu_struct_size()));
}


void* set_ram_block_ptrs(void* chunk, ram_block_t* b) {
	int i;
	for (i = 0; i < MAX_DATA_COUNT; ++i) {
		b->data[i] = (void*)((unsigned char*)chunk + (i * ram_struct_size()));
	}

	return (void*)((unsigned char*)chunk + (i * ram_struct_size()));
}


void* set_storage_dev_ptrs(void* chunk, storage_dev_block_t* b) {
	int i;
	for (i = 0; i < MAX_DATA_COUNT; ++i) {
		b->data[i] = (void*)((unsigned char*)chunk + (i * storage_dev_struct_size()));
	}

	return (void*)((unsigned char*)chunk + (i * storage_dev_struct_size()));
}