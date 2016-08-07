#include "query_select.h"


int mainboard_select_socket(mainboard_t* elem, processor_socket_t psoc, ram_socket_t rsoc, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price) {
	if (mainboard_get_psoc(elem) == psoc && mainboard_get_rsoc(elem) == rsoc) {
		return 1;
	}
	else return 0;
}


int mainboard_select_only_ram_socket(mainboard_t* elem, processor_socket_t psoc, ram_socket_t rsoc, unsigned int rsoc_count, unsigned int sd_count, unsigned int pw, unsigned int price) {
	if (mainboard_get_rsoc(elem) == rsoc) {
		return 1;
	}
	else return 0;
}


int processor_select_socket(processor_t* elem, processor_socket_t psoc, unsigned int pw, unsigned int price) {
	if (processor_get_psoc(elem) == psoc) {
		return 1;
	}
	else return 0;
}


int ram_select_socket(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price) {
	if (ram_get_rsoc(elem) == rsoc) {
		return 1;
	}
	else return 0;
}


int ram_select_socket_clock_freq(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price) {
	if (ram_select_socket(elem, rsoc, pw, capacity, clock_freq, price) && ram_get_clock_freq(elem) == clock_freq) {
		return 1;
	}
	else return 0;
}


int ram_select_capacity_included(ram_t* elem, ram_socket_t rsoc, unsigned int pw, unsigned int capacity, unsigned int clock_freq, unsigned int price) {
	if (ram_select_socket(elem, rsoc, pw, capacity, clock_freq, price) && ram_get_capacity(elem) == capacity) {
		return 1;
	}
	else return 0;
}


int psu_select_pw_min(psu_t* elem, unsigned int pw, unsigned int price) {
	if (psu_get_pw_output(elem) >= pw) {
		return 1;
	}
	else return 0;
}


int storage_dev_select_pw_max(storage_dev_t* elem, unsigned int capacity, unsigned int pw, unsigned int price) {
	if (storage_dev_get_power_usage(elem) <= pw) {
		return 1;
	}
	else return 0;
}