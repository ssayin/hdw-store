#include "ram.h"
#include "common_traits.h"
#include "ram_socket.h"

#include <string.h>


struct ram {
    common_traits_t common;
    ram_socket_t rsoc;
    unsigned int clock_freq;
    unsigned int capacity;
};


void ram_set(ram_t* r, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	ram_socket_t rsoc_type, unsigned int clock_freq,
	unsigned int capacity) {

	r->common.power_usage = pusage;
	r->common.price = price;
	strcpy(r->common.brand, brand);
	strcpy(r->common.code, code);
	r->rsoc = rsoc_type;
	r->clock_freq = clock_freq;
	r->capacity = capacity;
}


unsigned int ram_get_power_usage(ram_t* r) {
	return r->common.power_usage;
}


const char* ram_get_brand(ram_t* r) {
	return r->common.brand;
}


const char* ram_get_code(ram_t* r) {
	return r->common.code;
}


unsigned int ram_get_price(ram_t* r) {
	return r->common.price;
}


ram_socket_t ram_get_rsoc(ram_t* r) {
	return r->rsoc;
}


unsigned int ram_get_clock_freq(ram_t* r) {
	return r->clock_freq;
}


unsigned int ram_get_capacity(ram_t* r) {
	return r->capacity;
}


size_t ram_struct_size() {
	return sizeof(ram_t);
}