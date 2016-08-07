#ifndef __QUERY_H__
#define __QUERY_H__

#include "mainboard.h"
#include "processor.h"
#include "ram.h"
#include "psu.h"
#include "storage_dev.h"
#include "processor_socket.h"
#include "ram_socket.h"
#include "block_types.h"

typedef int(*mainboard_query_cb_t) (mainboard_t* elem, processor_socket_t, ram_socket_t, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price);
typedef int(*processor_query_cb_t) (processor_t* elem, processor_socket_t, unsigned int pw, unsigned int price);
typedef int(*ram_query_cb_t) (ram_t* elem, ram_socket_t, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price);
typedef int(*psu_query_cb_t) (psu_t* elem, unsigned int pw, unsigned int price);
typedef int(*storage_dev_query_cb_t) (storage_dev_t* elem, unsigned int capacity, unsigned int pw, unsigned int price);

typedef struct {
	void* elems[100];
	int count;
} query_result_t;

void mainboard_query(query_result_t* res, mainboard_block_t* block, mainboard_query_cb_t query, 
	processor_socket_t psoc, ram_socket_t rsoc, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price);

void processor_query(query_result_t* res, processor_block_t* block, processor_query_cb_t query, 
	processor_socket_t psoc, unsigned int pw, unsigned int price);

void ram_query(query_result_t* res, ram_block_t* block, ram_query_cb_t query, 
	ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price);

void psu_query(query_result_t* res, psu_block_t* block, psu_query_cb_t query, 
	unsigned int pw, unsigned int price);

void storage_dev_query(query_result_t* res, storage_dev_block_t* block, storage_dev_query_cb_t query, 
	unsigned int capacity, unsigned int pw, unsigned int price);

#endif