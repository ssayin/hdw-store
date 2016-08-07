#ifndef __BLOCK_TYPES_H__
#define __BLOCK_TYPES_H__

#include "mainboard.h"
#include "processor.h"
#include "psu.h"
#include "ram.h"
#include "storage_dev.h"

#define MAX_DATA_COUNT 100

typedef struct { mainboard_t* data[MAX_DATA_COUNT]; size_t count; } mainboard_block_t;
typedef struct { processor_t* data[MAX_DATA_COUNT]; size_t count; } processor_block_t;
typedef struct { psu_t* data[MAX_DATA_COUNT]; size_t count; } psu_block_t;
typedef struct { ram_t* data[MAX_DATA_COUNT]; size_t count; } ram_block_t;
typedef struct { storage_dev_t* data[MAX_DATA_COUNT]; size_t count; } storage_dev_block_t;

#endif