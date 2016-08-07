#include "mainboard.h"
#include "common_traits.h"

#include <string.h>


struct mainboard {
    common_traits_t common;
    processor_socket_t psoc;
    ram_socket_t rsoc;
    unsigned int rsoc_count;
    unsigned int storage_dev_count;
};


void mainboard_set(mainboard_t* mb, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	processor_socket_t psoc_type, ram_socket_t rsoc_type,
	unsigned int rsoc_c, unsigned int storage_dev_c) {

	mb->common.power_usage = pusage;
	strcpy(mb->common.brand, brand);
	strcpy(mb->common.code, code);
	mb->common.price = price;
	mb->psoc = psoc_type;
	mb->rsoc = rsoc_type;
	mb->rsoc_count = rsoc_c;
	mb->storage_dev_count = storage_dev_c;
}


unsigned int mainboard_get_power_usage(mainboard_t* mb) {
	return mb->common.power_usage;
}


const char* mainboard_get_brand(mainboard_t* mb) {
	return mb->common.brand;
}


const char* mainboard_get_code(mainboard_t* mb) {
	return mb->common.code;
}


unsigned int mainboard_get_price(mainboard_t* mb) {
	return mb->common.price;
}


processor_socket_t mainboard_get_psoc(mainboard_t* mb) {
	return mb->psoc;
}


ram_socket_t mainboard_get_rsoc(mainboard_t* mb) {
	return mb->rsoc;
}


unsigned int mainboard_get_rsoc_count(mainboard_t* mb) {
	return mb->rsoc_count;
}


unsigned int mainboard_get_storage_dev_count(mainboard_t* mb) {
	return mb->storage_dev_count;
}


size_t mainboard_struct_size() {
	return sizeof(mainboard_t);
}


//int16_t mainboard_get_int16(mainboard_t* t, mainboard_offset_t loc) {
//	return *((int*)(((char*)t) + loc));
//}
//
//
//void mainboard_set_int16(mainboard_t* t, mainboard_offset_t loc, int16_t data) {
//	*((int*)(((char*)t) + loc)) = data;
//}