#include "query.h"


/* query_result_t pass edilen init edilmis bir obje olmali, ve mumkunse bos yolla ki, farkli turler karismasin - SS */
void mainboard_query(query_result_t* res, mainboard_block_t* block, mainboard_query_cb_t query,
	processor_socket_t psoc, ram_socket_t rsoc, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price) {

	int i;
	mainboard_t* mb;

	for (i = 0; i < block->count; ++i) {
		mb = block->data[i];
		if (query(mb, psoc, rsoc, rsoc_count, sd_count, pw, price)) {
			res->elems[res->count++] = mb;
		}
	}
}


void processor_query(query_result_t* res, processor_block_t* block, processor_query_cb_t query,
	processor_socket_t psoc, unsigned int pw, unsigned int price) {

	int i;
	processor_t* p;

	for (i = 0; i < block->count; ++i) {
		p = block->data[i];
		if (query(p, psoc, pw, price)) {
			res->elems[res->count++] = p;
		}
	}
}


void ram_query(query_result_t* res, ram_block_t* block, ram_query_cb_t query,
	ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price) {

	int i;
	ram_t* ram;

	for (i = 0; i < block->count; ++i) {
		ram = block->data[i];
		if (query(ram, rsoc, pw, capacity, clock_freq, price)) {
			res->elems[res->count++] = ram;
		}
	}
}


void psu_query(query_result_t* res, psu_block_t* block, psu_query_cb_t query,
	unsigned int pw, unsigned int price) {

	int i;
	psu_t* psu;

	for (i = 0; i < block->count; ++i) {
		psu = block->data[i];
		if (query(psu, pw, price)) {
			res->elems[res->count++] = psu;
		}
	}
}


void storage_dev_query(query_result_t* res, storage_dev_block_t* block, storage_dev_query_cb_t query,
	unsigned int capacity, unsigned int pw, unsigned int price) {

	int i;
	storage_dev_t* sd;

	for (i = 0; i < block->count; ++i) {
		sd = block->data[i];
		if (query(sd, capacity, pw, price)) {
			res->elems[res->count++] = sd;
		}
	}
}