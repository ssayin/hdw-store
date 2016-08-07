#include "sort_callbacks.h"


int mainboard_sort_price_cb(mainboard_t* a, mainboard_t* b) {
	if (mainboard_get_price(a) < mainboard_get_price(b)) return -1;
	if (mainboard_get_price(a) > mainboard_get_price(b)) return 1;
	return 0;
}


int processor_sort_price_cb(processor_t* a, processor_t* b) {
	if (processor_get_price(a) < processor_get_price(b)) return -1;
	if (processor_get_price(a) > processor_get_price(b)) return 1;
	return 0;
}


int ram_sort_price_cb(ram_t* a, ram_t* b) {
	if (ram_get_price(a) < ram_get_price(b)) return -1;
	if (ram_get_price(a) > ram_get_price(b)) return 1;
	return 0;
}


int psu_sort_price_cb(psu_t* a, psu_t* b) {
	if (psu_get_price(a) < psu_get_price(b)) return -1;
	if (psu_get_price(a) > psu_get_price(b)) return 1;
	return 0;
}


int storage_dev_sort_price_cb(storage_dev_t* a, storage_dev_t* b) {
	if (storage_dev_get_price(a) < storage_dev_get_price(b)) return -1;
	if (storage_dev_get_price(a) > storage_dev_get_price(b)) return 1;
	return 0;
}