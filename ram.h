#ifndef __RAM_H__
#define __RAM_H__

#include "ram_socket.h"

#include <stdlib.h>

typedef struct ram ram_t;

void ram_set(ram_t* r, unsigned int pusage,
	unsigned int price, char* brand, char* code,
	ram_socket_t rsoc_type, unsigned int clock_freq,
	unsigned int capacity);

unsigned int ram_get_power_usage(ram_t* r);
const char* ram_get_brand(ram_t* r);
const char* ram_get_code(ram_t* r);
unsigned int ram_get_price(ram_t* r);
ram_socket_t ram_get_rsoc(ram_t* r);
unsigned int ram_get_clock_freq(ram_t* r);
unsigned int ram_get_capacity(ram_t* r);
size_t ram_struct_size();

#endif