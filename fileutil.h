#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include <stdio.h>

#include "mainboard.h"
#include "processor.h"
#include "psu.h"
#include "ram.h"
#include "storage_dev.h"

#define FN_MAINBOARD ("anakart.txt")
#define FN_PROCESSOR ("islemci.txt")
#define FN_PSU ("guc.txt")
#define FN_RAM ("bellek.txt")
#define FN_STORAGE_DEV ("depolama.txt")

typedef void(*read_hardware_cb_t)(FILE* fp, void* addr);

void read_mainboard(FILE* fp, mainboard_t* mb);
void read_processor(FILE* fp, processor_t* p);
void read_psu(FILE* fp, psu_t* psu);
void read_ram(FILE* fp, ram_t* r);
void read_storage_dev(FILE* fp, storage_dev_t* sd);
void read_from_file(const char* fn, void** addr, int* count, read_hardware_cb_t func);

#endif