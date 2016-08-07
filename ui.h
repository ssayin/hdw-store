#ifndef __UI_H__
#define __UI_H__

#include <stdio.h>

#include "mainboard.h"
#include "processor.h"
#include "psu.h"
#include "ram.h"
#include "storage_dev.h"
#include "shopping_cart.h"
#include "query.h"

void print_mainboard(FILE* fp, mainboard_t* mb);
void print_processor(FILE* fp, processor_t* p);
void print_psu(FILE* fp, psu_t* psu);
void print_ram(FILE* fp, ram_t* ram);
void print_storage_dev(FILE* fp, storage_dev_t* sd);

/* bunlar block objesi de alabiliyor, query_result_t objesi de alabiliyorlar - SS */

void print_mainboard_array(FILE* fp, query_result_t* data);
void print_processor_array(FILE* fp, query_result_t* data);
void print_psu_array(FILE* fp, query_result_t* data);
void print_ram_array(FILE* fp, query_result_t* data);
void print_storage_dev_array(FILE* fp, query_result_t* data);

void print_shopping_cart_billing(FILE* fp, shopping_cart_t* cart);

#endif