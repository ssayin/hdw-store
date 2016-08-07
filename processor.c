#include "processor.h"
#include "common_traits.h"
#include "processor_socket.h"

#include <string.h> 


struct processor {
    common_traits_t common;
    processor_socket_t psoc;
    unsigned int clock_rate;
};


void processor_set(processor_t* p, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	processor_socket_t psoc_type, unsigned int clock_rate) {

	p->common.power_usage = pusage;
	p->common.price = price;
	strcpy(p->common.brand, brand);
	strcpy(p->common.code, code);
	p->psoc = psoc_type;
	p->clock_rate = clock_rate;
}


unsigned int processor_get_power_usage(processor_t* p) {
	return p->common.power_usage;
}


const char* processor_get_brand(processor_t* p) {
	return p->common.brand;
}


const char* processor_get_code(processor_t* p) {
	return p->common.code;
}


unsigned int processor_get_price(processor_t* p) {
	return p->common.price;
}


processor_socket_t processor_get_psoc(processor_t* p) {
	return p->psoc;
}


unsigned int processor_get_clock_rate(processor_t* p) {
	return p->clock_rate;
}


size_t processor_struct_size() {
	return sizeof(processor_t);
}