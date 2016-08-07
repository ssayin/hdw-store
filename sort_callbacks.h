#ifndef __SORT_CALLBACKS_H__
#define __SORT_CALLBACKS_H__

#include "mainboard.h"
#include "psu.h"
#include "processor.h"
#include "ram.h"
#include "storage_dev.h"

int mainboard_sort_price_cb(mainboard_t* a, mainboard_t* b);
int processor_sort_price_cb(processor_t* a, processor_t* b);
int ram_sort_price_cb(ram_t* a, ram_t* b);
int psu_sort_price_cb(psu_t* a, psu_t* b);
int storage_dev_sort_price_cb(storage_dev_t* a, storage_dev_t* b);

#endif