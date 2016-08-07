#ifndef __MAINBOARD_H__
#define __MAINBOARD_H__

#include "processor_socket.h"
#include "ram_socket.h"

#include <stdlib.h>

/*
typedef enum {
    MB_POWER_USAGE = 0x0,
    MB_PRICE = 0x2,
    MB_BRAND = 0x4,
    MB_CODE = 0x18,
    MB_PSOCKET = 0x38,
    MB_RSOCKET = 0x3A,
    MB_RSOCKET_COUNT = 0x3C,
    MB_STORAGE_DEV_COUNT = 0x3E
} mainboard_offset_t;
*/

typedef struct mainboard mainboard_t;

void mainboard_set(mainboard_t* mb, unsigned int pusage, 
	unsigned int price, char* brand, char* code, 
	processor_socket_t psoc_type, ram_socket_t rsoc_type, 
	unsigned int rsoc_c, unsigned int storage_dev_c);

unsigned int mainboard_get_power_usage(mainboard_t* mb);
const char* mainboard_get_brand(mainboard_t* mb);
const char* mainboard_get_code(mainboard_t* mb);
unsigned int mainboard_get_price(mainboard_t* mb);
processor_socket_t mainboard_get_psoc(mainboard_t* mb);
ram_socket_t mainboard_get_rsoc(mainboard_t* mb);
unsigned int mainboard_get_rsoc_count(mainboard_t* mb);
unsigned int mainboard_get_storage_dev_count(mainboard_t* mb);
size_t mainboard_struct_size();

#endif