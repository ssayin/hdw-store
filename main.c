#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "mainboard.h"
#include "processor.h"
#include "ram.h"
#include "storage_dev.h"
#include "psu.h"
#include "fileutil.h"
#include "block_allocator.h"
#include "shopping_cart.h"
#include "query.h"
#include "query_select.h"
#include "ui.h"
#include "platform.h"
#include "sort.h"
#include "sort_callbacks.h"


static void greet() {
	printf("Eylul Bilisim Hizmetlerine hosgeldiniz!\n");
	printf("Bilgisayar toplamak icin T tusuna basin.\n");
	printf("Ucuz bilgisayar otomatik toplamak icin U tusuna basin.\n");
	printf("Cikmak istiyorsaniz X tusuna basin.\n");
	printf("Sepeti gormek istiyorsaniz S tusuna basin.\n");
	printf("Faturayi bastirmak istiyorsaniz F tusuna basin.\n");
}


//static void low_cost_with_ram_capacity_specified(mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb,
//	shopping_cart_t* cart, int num, int capacity) {
//	query_result_t res = { 0 };
//	processor_socket_t psoc;
//	ram_socket_t rsoc;
//	unsigned int pw_total = 0;
//	void* selection;
//
//	/* Bellek */
//	selection = rb->data[0];
//	pw_total += ram_get_power_usage(selection);
//	shopping_cart_add_item(cart, selection, SCI_RAM, num);
//}


static void low_cost_pc(mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb,
	shopping_cart_t* cart, int num) {

	query_result_t res = { 0 };
	processor_socket_t psoc;
	ram_socket_t rsoc;
	unsigned int pw_total = 0;
	void* selection;

	/* Anakart */
	selection = mbb->data[0];
	psoc = mainboard_get_psoc(selection);
	rsoc = mainboard_get_rsoc(selection);
	pw_total += mainboard_get_power_usage(selection);
	shopping_cart_add_item(cart, selection, SCI_MAINBOARD, num);

	/* Islemci */
	processor_query(&res, pcb, processor_select_socket, psoc, 0, 0);
	selection = res.elems[0];
	res.count = 0; /* Unutmadan - SS */
	pw_total += processor_get_power_usage(selection);
	shopping_cart_add_item(cart, selection, SCI_PROCESSOR, num);

	/* Bellek */
	ram_query(&res, rb, ram_select_socket, rsoc, 0, 0, 0, 0);
	selection = res.elems[0];
	res.count = 0; 
	pw_total += ram_get_power_usage(selection);
	shopping_cart_add_item(cart, selection, SCI_RAM, num);

	/* Depolama Aygiti */
	selection = sdb->data[0];
	pw_total += storage_dev_get_power_usage(selection);
	shopping_cart_add_item(cart, selection, SCI_STORAGE_DEV, num);

	/* Guc Kaynagi */
	psu_query(&res, pb, psu_select_pw_min, pw_total, 0);
	selection = res.elems[0];
	res.count = 0;
	shopping_cart_add_item(cart, selection, SCI_PSU, num);
}


static void get_computer_parts(mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb,
	shopping_cart_t* cart, int num) {

	query_result_t res = { 0 };
	processor_socket_t psoc;
	ram_socket_t rsoc;
	unsigned int rsoc_count;
	unsigned int ram_clock_freq;
	unsigned int ram_count = 0;
	unsigned int storage_dev_limit;
	unsigned int storage_dev_count = 0;
	unsigned int pw_total = 0;
	void* selection;
	int ram_selected = 0;
	int sel = 0;
	char ch = 'a';

	CLEAR_SCREEN();

	printf("Anakart Secimi\n");
	print_mainboard_array(stdout, mbb);

	do {
		printf("\n Secmek istediginiz anakartin numarasini yazin: ");
		scanf("%d", &sel);
	} while (!(sel >= 0 && sel < mbb->count)); /* Bunun boyle olmasi lazim - SS */

	/* Anakartin adresi bize lazim - SS */
	selection = mbb->data[sel];

	/* Bunlar bize lazim - SS */
	psoc = mainboard_get_psoc(selection);
	rsoc = mainboard_get_rsoc(selection);
	rsoc_count = mainboard_get_rsoc_count(selection);
	storage_dev_limit = mainboard_get_storage_dev_count(selection);

	shopping_cart_add_item(cart, selection, SCI_MAINBOARD, num);

	pw_total += mainboard_get_power_usage(selection);

	CLEAR_SCREEN();

	/* Islemci */
	printf("Islemci Secimi\n");
	processor_query(&res, pcb, processor_select_socket, psoc, 0, 0); /* Ise yarayanlari listeleyelim artik - SS */
	print_processor_array(stdout, &res); 

	do {
		printf("\n Secmek istediginiz islemcinin numarasini yazin: ");
		scanf("%d", &sel);
	} while (!(sel >= 0 && sel < res.count));

	selection = res.elems[sel];

	shopping_cart_add_item(cart, selection, SCI_PROCESSOR, num);

	pw_total += processor_get_power_usage(selection);

	res.count = 0; /* Islemcilerin devamina yazmasin - SS */

	CLEAR_SCREEN();

	/* Bellekler */
	do {
		printf("Bellek Secimi\n");
		
		/* Ilk kez seciyoruz, sadece psoc onemli - SS */
		if (!ram_selected) {
			ram_query(&res, rb, ram_select_socket, rsoc, 0, 0, 0, 0);
			print_ram_array(stdout, &res);

			do {
				printf("\n Secmek istediginiz bellegin numarasini yazin: ");
				scanf("%d", &sel);
			} while (!(sel >= 0 && sel < res.count));

			ram_selected = 1;
			selection = res.elems[sel];
			ram_clock_freq = ram_get_clock_freq(selection);

			shopping_cart_add_item(cart, selection, SCI_RAM, num);

			ram_count++;

		/* Burada saat frekansi da onemli - SS */
		} else {
			res.count = 0;
			ram_query(&res, rb, ram_select_socket_clock_freq, rsoc, 0, 0, ram_clock_freq, 0);
			print_ram_array(stdout, &res);

			do {
				printf("\n Secmek istediginiz bellegin numarasini yazin (Cikmak icin -1): ");
				scanf("%d", &sel);
				if (sel == -1)
					break;
			} while (!(sel >= 0 && sel < res.count));

			if (sel == -1) {
				CLEAR_SCREEN();
				break;
			}

			selection = res.elems[sel];

			shopping_cart_add_item(cart, selection, SCI_RAM, num);

			ram_count++;
		}

		CLEAR_SCREEN();

		pw_total += ram_get_power_usage(selection);

	} while (ram_count < rsoc_count);

	/* Depolama aygitlari */

	printf("Depolama Aygiti Secimi\n");

	do {
		
		print_storage_dev_array(stdout, sdb);

		do {
			printf("\n Secmek istediginiz depolama aygitinin numarasini yazin (Cikmak icin -1): ");
			scanf("%d", &sel);
			if (sel == -1)
				break;
		} while (!(sel >= 0 && sel < sdb->count));

		if (sel == -1) {
			CLEAR_SCREEN();
			break;
		}
			
		selection = sdb->data[sel];

		shopping_cart_add_item(cart, selection, SCI_STORAGE_DEV, num);

		CLEAR_SCREEN();

		pw_total += storage_dev_get_power_usage(selection);

		storage_dev_count++;

	} while (storage_dev_count < storage_dev_limit);

	/* Guc Kaynagi */

	printf("Guc Kaynagi Secimi\n");

	/* Karismasin - SS */
	res.count = 0;

	psu_query(&res, pb, psu_select_pw_min, pw_total, 0);
	print_psu_array(stdout, &res);

	do {
		printf("\n Secmek istediginiz guc kaynaginin numarasini yazin: ");
		scanf("%d", &sel);
	} while (!(sel >= 0 && sel < res.count));

	selection = res.elems[sel];

	shopping_cart_add_item(cart, selection, SCI_PSU, num);

	res.count = 0; /* Islemcilerin devamina yazmasin - SS */

	CLEAR_SCREEN();
}


int main(int argc, char* argv[]) {
	void* block;
	void* block_end;
	mainboard_block_t mbb = {0};
	processor_block_t pcb = {0};
	storage_dev_block_t sdb = {0};
	ram_block_t rb = {0};
	psu_block_t pb = {0};
	query_result_t res = {0};
	shopping_cart_t* cart;
	FILE* fp;
	int ok = 0;
	char ch = 'a';
	int num = 0;

	block = malloc(processor_struct_size() * MAX_DATA_COUNT + 
		mainboard_struct_size() * MAX_DATA_COUNT + 
		psu_struct_size() * MAX_DATA_COUNT + 
		ram_struct_size() * MAX_DATA_COUNT + 
		storage_dev_struct_size() * MAX_DATA_COUNT);

	block_end = set_mainboard_block_ptrs(block, &mbb);
	block_end = set_processor_block_ptrs(block_end, &pcb);
	block_end = set_storage_dev_ptrs(block_end, &sdb);
	block_end = set_ram_block_ptrs(block_end, &rb);
	block_end = set_psu_block_ptrs(block_end, &pb);

	read_from_file(FN_MAINBOARD, mbb.data, &mbb.count, read_mainboard);
	read_from_file(FN_PROCESSOR, pcb.data, &pcb.count, read_processor);
	read_from_file(FN_STORAGE_DEV, sdb.data, &sdb.count, read_storage_dev);
	read_from_file(FN_PSU, pb.data, &pb.count, read_psu);
	read_from_file(FN_RAM, rb.data, &rb.count, read_ram);

	/* Bir kere fiyata gore dizdikten sonra bir daha dizmeye gerek kalmayacak - SS */
	query_bubble_sort(&mbb, mainboard_sort_price_cb);
	query_bubble_sort(&pcb, processor_sort_price_cb);
	query_bubble_sort(&sdb, storage_dev_sort_price_cb);
	query_bubble_sort(&pb, psu_sort_price_cb);
	query_bubble_sort(&rb, ram_sort_price_cb);
    
	cart = shopping_cart_create();

	while (!ok) {
		CLEAR_SCREEN();
		greet();
		ch = getchar();
		switch (ch) {
		case 't':
			get_computer_parts(&mbb, &pcb, &sdb, &rb, &pb, cart, num);
			num++;
			break;
		case 'x':
			ok = 1;
			break;
		case 's':
			CLEAR_SCREEN();
			print_shopping_cart_billing(stdout, cart);
			printf("Devam etmek icin Enter'a basin...\n");
			getchar();
			getchar();
			break;
		case 'f':
			CLEAR_SCREEN();
			fp = fopen("fatura.txt", "w");
			print_shopping_cart_billing(fp, cart);
			fclose(fp);
			printf("Fatura yazildi.\nDevam etmek icin Enter'a basin...\n");
			getchar();
			getchar();
			break;
		case 'u':
			CLEAR_SCREEN();
			low_cost_pc(&mbb, &pcb, &sdb, &rb, &pb, cart, num);
			num++;
			printf("Bilgisayariniz toplandi.\nDevam etmek icin Enter'a basin...");
			getchar();
			getchar();
			break;
		default:
			break;
		}
	}

	shopping_cart_destroy(cart);

	free(block); /* Bununla isimiz bitti - SS */

    return 0;
}