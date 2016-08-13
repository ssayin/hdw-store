#ifndef __QUERY_SELECT_H__
#define __QUERY_SELECT_H__

#include "query.h"

int mainboard_select_socket(mainboard_t* elem, processor_socket_t, ram_socket_t, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price);
int mainboard_select_pw_max(mainboard_t* elem, processor_socket_t, ram_socket_t, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price);

int processor_select_socket(processor_t* elem, processor_socket_t, unsigned int pw, unsigned int price);
int processor_select_pw_max(processor_t* elem, processor_socket_t, unsigned int pw, unsigned int price);

int ram_select_socket(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price);
int ram_select_socket_clock_freq(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price);
int ram_select_capacity_included(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price);
int ram_select_pw_max(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price);

int psu_select_pw_min(psu_t* elem, unsigned int pw, unsigned int price);

int storage_dev_select_pw_max(storage_dev_t* elem, unsigned int capacity, unsigned int pw, unsigned int price);

#endif
