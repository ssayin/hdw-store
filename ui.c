#include "ui.h"


void print_mainboard(FILE* fp, mainboard_t* mb) {
	fprintf(fp, "%-20s%-23s%-15u%-8u%-17s%-13s%-6u%-5u\n", mainboard_get_brand(mb), 
		mainboard_get_code(mb), mainboard_get_power_usage(mb),
		mainboard_get_price(mb), psoc_to_string(mainboard_get_psoc(mb)), 
		rsoc_to_string(mainboard_get_rsoc(mb)), 
		mainboard_get_rsoc_count(mb), 
		mainboard_get_storage_dev_count(mb));
}


void print_processor(FILE* fp, processor_t* p) {
	fprintf(fp, "%-20s%-23s%-15u%-8u%-17s%-16u\n", processor_get_brand(p), 
		processor_get_code(p), processor_get_power_usage(p), processor_get_price(p),
		psoc_to_string(processor_get_psoc(p)), processor_get_clock_rate(p));
}


void print_psu(FILE* fp, psu_t* psu) {
	fprintf(stdout, "%-20s%-23s%-8u%-14u%\n", psu_get_brand(psu),
		psu_get_code(psu), psu_get_price(psu), psu_get_pw_output(psu));
}


void print_ram(FILE* fp, ram_t* ram) {
	fprintf(stdout, "%-20s%-23s%-15u%-8u%-13s%-16u%-11u\n", ram_get_brand(ram),
		ram_get_code(ram), ram_get_power_usage(ram), ram_get_price(ram),
		rsoc_to_string(ram_get_rsoc(ram)), ram_get_clock_freq(ram), ram_get_capacity(ram));
}


void print_storage_dev(FILE* fp, storage_dev_t* sd) {
	fprintf(stdout, "%-20s%-23s%-15u%-8u%-11u\n", storage_dev_get_brand(sd), storage_dev_get_code(sd),
		storage_dev_get_power_usage(sd), storage_dev_get_price(sd), storage_dev_get_capacity(sd));
}


void print_mainboard_array(FILE* fp, query_result_t* data) {
	int i;

	fprintf(stdout, "%-4s%-20s%-23s%-15s%-8s%-17s%-13s%-6s%-6s\n", "#", "Marka",
		"Kod", "Guc Tuketimi", "Fiyat",
		"Islemci Soketi", "Ram Soketi",
		"RSS", "DAS");

	for (i = 0; i < data->count; ++i) {
		fprintf(fp, "%-4u", i);
		print_mainboard(fp, data->elems[i]);
	}
}


void print_processor_array(FILE* fp, query_result_t* data) {
	int i;

	fprintf(stdout, "%-4s%-20s%-23s%-15s%-8s%-17s%-16s\n", "#", "Marka",
		"Kod", "Guc Tuketimi", "Fiyat",
		"Islemci Soketi", "Saat Frekansi");

	for (i = 0; i < data->count; ++i) {
		fprintf(fp, "%-4u", i);
		print_processor(fp, data->elems[i]);
	}
}


void print_psu_array(FILE* fp, query_result_t* data) {
	int i;

	fprintf(stdout, "%-4s%-20s%-23s%-8s%-14s%\n", "#", "Marka",
		"Kod", "Fiyat", "Guc Cikisi");

	for (i = 0; i < data->count; ++i) {
		fprintf(fp, "%-4u", i);
		print_psu(fp, data->elems[i]);
	}
}


void print_ram_array(FILE* fp, query_result_t* data) {
	int i;

	fprintf(stdout, "%-4s%-20s%-23s%-15s%-8s%-13s%-16s%-11s\n", "#", "Marka",
		"Kod", "Guc Tuketimi", "Fiyat",
		"Ram Soketi", "Saat Frekansi", "Kapasite");

	for (i = 0; i < data->count; ++i) {
		fprintf(fp, "%-4u", i);
		print_ram(fp, data->elems[i]);
	}
}


void print_storage_dev_array(FILE* fp, query_result_t* data) {
	int i;

	fprintf(stdout, "%-4s%-20s%-23s%-15s%-8s%-11s\n", "#", "Marka",
		"Kod", "Guc Tuketimi", "Fiyat", "Kapasite");

	for (i = 0; i < data->count; ++i) {
		fprintf(fp, "%-4u", i);
		print_storage_dev(fp, data->elems[i]);
	}
}


void print_shopping_cart_billing(FILE* fp, shopping_cart_t* cart) {
	/* 0 elemanliysa gerek yok - SS */
	if (shopping_cart_get_item_count(cart) == 0)
		return;

	int i;
	void* ptr;
	shopping_cart_item_type_t type;
	//int computer;

	char brand[20];
	char code[20];
	char szType[20];
	unsigned int price;
	unsigned int price_total = 0;

	fprintf(fp, "%-20s%-23s%-14s%-8s\n", "Marka",
		"Kod", "Parca Turu", "Fiyat");

	for (i = 0; i < shopping_cart_get_item_count(cart); ++i) {

		type = shopping_cart_get_elem_type(cart, i);
		ptr = shopping_cart_get_elem_ptr(cart, i);

		switch (type) {
		case SCI_MAINBOARD:
			strcpy(szType, "Anakart");
			strcpy(brand, mainboard_get_brand(ptr));
			strcpy(code, mainboard_get_code(ptr));
			price = mainboard_get_price(ptr);
			break;
		case SCI_PROCESSOR:
			strcpy(szType, "Islemci");
			strcpy(brand, processor_get_brand(ptr));
			strcpy(code, processor_get_code(ptr));
			price = processor_get_price(ptr);
			break;
		case SCI_PSU:
			strcpy(szType, "Guc Kaynagi");
			strcpy(brand, psu_get_brand(ptr));
			strcpy(code, psu_get_code(ptr));
			price = psu_get_price(ptr);
			break;
		case SCI_RAM:
			strcpy(szType, "Bellek");
			strcpy(brand, ram_get_brand(ptr));
			strcpy(code, ram_get_code(ptr));
			price = ram_get_price(ptr);
			break;
		case SCI_STORAGE_DEV:
			strcpy(szType, "Depolama");
			strcpy(brand, storage_dev_get_brand(ptr));
			strcpy(code, storage_dev_get_code(ptr));
			price = storage_dev_get_price(ptr);
			break;
		}

		price_total += price;

		fprintf(fp, "%-20s%-23s%-14s%-8u\n", brand,
			code, szType, price);
	}

	fprintf(fp, "\nToplam Fiyat: %u\n", price_total);
}