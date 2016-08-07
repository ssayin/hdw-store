#ifndef __STORAGE_DEV_H__
#define __STORAGE_DEV_H__

#include <stdlib.h>

typedef struct storage_dev storage_dev_t;

void storage_dev_set(storage_dev_t* sd, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	unsigned int capacity);

unsigned int storage_dev_get_power_usage(storage_dev_t* sd);
const char* storage_dev_get_brand(storage_dev_t* sd);
const char* storage_dev_get_code(storage_dev_t* sd);
unsigned int storage_dev_get_price(storage_dev_t* sd);
unsigned int storage_dev_get_capacity(storage_dev_t* sd);
size_t storage_dev_struct_size();

#endif