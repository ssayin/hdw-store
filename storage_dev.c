#include "storage_dev.h"
#include "common_traits.h"

#include <string.h>


struct storage_dev {
    common_traits_t common;
    unsigned int capacity;
};


void storage_dev_set(storage_dev_t* sd, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	unsigned int capacity) {

	sd->common.power_usage = pusage;
	sd->common.price = price;
	strcpy(sd->common.brand, brand);
	strcpy(sd->common.code, code);
	sd->capacity = capacity;
}


unsigned int storage_dev_get_power_usage(storage_dev_t* sd) {
	return sd->common.power_usage;
}


const char* storage_dev_get_brand(storage_dev_t* sd) {
	return sd->common.brand;
}


const char* storage_dev_get_code(storage_dev_t* sd) {
	return sd->common.code;
}


unsigned int storage_dev_get_price(storage_dev_t* sd) {
	return sd->common.price;
}


unsigned int storage_dev_get_capacity(storage_dev_t* sd) {
	return sd->capacity;
}


size_t storage_dev_struct_size() {
	return sizeof(storage_dev_t);
}