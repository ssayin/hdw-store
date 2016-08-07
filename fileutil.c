#include "fileutil.h"


/* Buraya gelen ptrler null olmasin artik, buna dikkat et - SS */
void read_mainboard(FILE* fp, mainboard_t* mb) {
	char brand[20];
	char code[20];
	unsigned int pusage;
	unsigned int price;

	char p_socket[5];
	char r_socket[5];
	unsigned int r_soc_count;
	unsigned int storage_dev_count;

	/* string degiskenlerine ayirici prefixler verilebilir sz gibi, 
	karismasin diye - SS */
	fscanf(fp, "%s %s %u %u %s %s %u %u", brand, code, &pusage,
		&price, p_socket, r_socket, &r_soc_count, &storage_dev_count);

	mainboard_set(mb, pusage, price, brand, code, to_processor_socket(p_socket), 
		to_ram_socket(r_socket), r_soc_count, storage_dev_count);
}


void read_processor(FILE* fp, processor_t* p) {
	char brand[20];
	char code[20];
	unsigned int pusage;
	unsigned int price;

	char p_socket[5];
	unsigned int clock_rate;

	fscanf(fp, "%s %s %u %u %s %u", brand, code, &pusage,
		&price, p_socket, &clock_rate);

	processor_set(p, pusage, price, brand, code, to_processor_socket(p_socket), 
		clock_rate);
}


void read_psu(FILE* fp, psu_t* psu) {
	char brand[20];
	char code[20];
	unsigned int price;

	unsigned int pw_output;

	fscanf(fp, "%s %s %u %u", brand, code, &price, 
		&pw_output);

	psu_set(psu, price, brand, code, pw_output);
}


void read_ram(FILE* fp, ram_t* r) {
	char brand[20];
	char code[20];
	unsigned int pusage;
	unsigned int price;

	char r_socket[5];
	unsigned int clock_freq;
	unsigned int capacity;

	fscanf(fp, "%s %s %u %u %s %u %u", brand, code, &pusage,
		&price, r_socket, &clock_freq, &capacity);

	ram_set(r, pusage, price, brand, code, to_ram_socket(r_socket), 
		clock_freq, capacity);
}


void read_storage_dev(FILE* fp, storage_dev_t* sd) {
	char brand[20];
	char code[20];
	unsigned int pusage;
	unsigned int price;

	unsigned int capacity;

	fscanf(fp, "%s %s %u %u %u", brand, code, &pusage,
		&price, &capacity);

	storage_dev_set(sd, pusage, price, brand, code, capacity);
}


void read_from_file(const char* fn, void** addr, int* count, read_hardware_cb_t func) {
	FILE* fp;
	fp = fopen(fn, "r");

	while (!feof(fp)) {
		func(fp, addr[(*count)++]);
	}

	fclose(fp);
}