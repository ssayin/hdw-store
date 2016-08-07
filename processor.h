#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include "processor_socket.h"

typedef struct processor processor_t;

void processor_set(processor_t* p, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	processor_socket_t psoc_type, unsigned int clock_rate);

unsigned int processor_get_power_usage(processor_t* p);
const char* processor_get_brand(processor_t* p);
const char* processor_get_code(processor_t* p);
unsigned int processor_get_price(processor_t* p);
processor_socket_t processor_get_psoc(processor_t* p);
unsigned int processor_get_clock_rate(processor_t* p);
size_t processor_struct_size();

#endif