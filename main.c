#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <limits.h>

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


typedef enum {
	NO_MATCHING_HARDWARE = 0,
	BILLING_DONE,
	CANCELED,
	NO_ITEMS_IN_CART,
} signal_t;


typedef struct {
	processor_socket_t psoc;
	ram_socket_t rsoc;
	unsigned int rsoc_count;
	unsigned int rsoc_used;
	unsigned int ram_clock_freq;
	unsigned int storage_dev_count;
	unsigned int storage_dev_used;
	unsigned int pw_total;
	unsigned int psu_pw_output;
} computer_info_t;


static int low_cost_pc(mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb, 
	shopping_cart_t* cart, int min_ram_capacity) {

	query_result_t res_p = { 0 };
	query_result_t res_ram = { 0 };
	query_result_t res = { 0 };

	processor_socket_t psoc;
	ram_socket_t rsoc;

	unsigned int pw_total = 0;
	unsigned int cost = 0;
	unsigned int lowest_cost = INT_MAX;

	void* selection_sd;
	void* selection_psu;
	void* selection_mb;
	void* selection_p;
	void* selection_ram;

	void* lowest_ram = 0;
	void* lowest_mb = 0;
	void* lowest_p = 0;

	int i;
	int j;
	int k;

	for (i = 0; i < mbb->count; ++i) {
		/* Anakart */
		selection_mb = mbb->data[i];
		psoc = mainboard_get_psoc(selection_mb);
		rsoc = mainboard_get_rsoc(selection_mb);

		/* Islemci */
		processor_query(&res_p, pcb, processor_select_socket, psoc, 0, 0);

		for (j = 0; j < res_p.count; ++j) {
			selection_p = res_p.elems[j];

			/* Bellek */

			ram_query(&res_ram, rb, ram_select_capacity_included, rsoc, 0, min_ram_capacity, 0, 0);

			for (k = 0; k < res_ram.count; ++k) {
				selection_ram = res_ram.elems[k];

				cost = mainboard_get_price(selection_mb) + 
					processor_get_price(selection_p) + 
					ram_get_price(selection_ram);

				if (cost < lowest_cost) {
					lowest_cost = cost;

					lowest_mb = selection_mb;
					lowest_p = selection_p;
					lowest_ram = selection_ram;
				}

			}

			res_ram.count = 0;
		}

		res_p.count = 0;
	}

	if (!(lowest_mb && lowest_p && lowest_ram)) {
		return 0;
	}

	pw_total = mainboard_get_power_usage(lowest_mb) +
		processor_get_power_usage(lowest_p) +
		ram_get_power_usage(lowest_ram);
	
	/* Depolama Aygiti */
	selection_sd = sdb->data[0];
	pw_total += storage_dev_get_power_usage(selection_sd);

	/* Guc Kaynagi */
	psu_query(&res, pb, psu_select_pw_min, pw_total, 0);
	selection_psu = res.elems[0];

	shopping_cart_add_item(cart, lowest_mb, SCI_MAINBOARD);
	shopping_cart_add_item(cart, lowest_p, SCI_PROCESSOR);
	shopping_cart_add_item(cart, lowest_ram, SCI_RAM);
	shopping_cart_add_item(cart, selection_sd, SCI_STORAGE_DEV);
	shopping_cart_add_item(cart, selection_psu, SCI_PSU);

	return 1;
}


static void get_computer_parts(mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb,
	shopping_cart_t* cart) {

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

	shopping_cart_add_item(cart, selection, SCI_MAINBOARD);

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

	shopping_cart_add_item(cart, selection, SCI_PROCESSOR);

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

			shopping_cart_add_item(cart, selection, SCI_RAM);

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

			shopping_cart_add_item(cart, selection, SCI_RAM);

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

		shopping_cart_add_item(cart, selection, SCI_STORAGE_DEV);

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

	shopping_cart_add_item(cart, selection, SCI_PSU);

	res.count = 0; /* Islemcilerin devamina yazmasin - SS */

	CLEAR_SCREEN();
}


static void low_cost_ui(signal_t* signal, mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb, 
	int min_ram_capacity) {

	int ch;

	shopping_cart_t* cart;

	FILE* fp;

	cart = shopping_cart_create();

	if (low_cost_pc(mbb, pcb, sdb, rb, pb, cart, min_ram_capacity)) {

		print_shopping_cart_billing(stdout, cart);

		printf("\nFaturalandirmak icin F'ye cikmak icin C'ye basin.\n");

		do {
			ch = getchar();

			switch (ch) {
			case 'f':
				fp = fopen("fatura.txt", "a");
				print_shopping_cart_billing(fp, cart);
				fclose(fp);

				*signal = BILLING_DONE;

				return;
			case 'c':
				*signal = CANCELED;

				return;
			}
		} while (ch);

	}

	else {
		*signal = NO_MATCHING_HARDWARE;
	}

	shopping_cart_destroy(cart);
}


static void get_computer_parts_ui(signal_t* signal, mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb) {

	char ch;

	shopping_cart_t* cart;
	
	FILE* fp;

	cart = shopping_cart_create();

	get_computer_parts(mbb, pcb, sdb, rb, pb, cart);

	print_shopping_cart_billing(stdout, cart);

	printf("\nFaturalandirmak icin F'ye cikmak icin C'ye basin.\n");

	do {
		ch = getchar();

		switch (ch) {
		case 'f':
			fp = fopen("fatura.txt", "a");
			print_shopping_cart_billing(fp, cart);
			fclose(fp);

			*signal = BILLING_DONE;

			return;
		case 'c':
			*signal = CANCELED;

			return;
		}
	} while (ch);

	shopping_cart_destroy(cart);
}


static int check_items(shopping_cart_t* cart) {
	void* mb;
	void* p;
	void* ram[8];
	void* sd[8];
	void* psu;

	int i;
	int j;

	int mb_c = 0;
	int p_c = 0;
	int sd_c = 0;
	int psu_c = 0;
	int ram_c = 0;

	processor_socket_t psoc;
	ram_socket_t rsoc;
	unsigned int rsoc_count;
	unsigned int max_storage_dev_count;
	unsigned int ram_ref_clock_freq;

	unsigned int total_power_usage = 0;

	for (i = 0; i < shopping_cart_get_item_count(cart); ++i) {

		switch (shopping_cart_get_elem_type(cart, i)) {
		case SCI_MAINBOARD:
			mb = shopping_cart_get_elem_ptr(cart, i);
			mb_c += shopping_cart_get_elem_count(cart, i);
			break;

		case SCI_PROCESSOR:
			p = shopping_cart_get_elem_ptr(cart, i);
			p_c += shopping_cart_get_elem_count(cart, i);
			break;

		case SCI_PSU:
			psu = shopping_cart_get_elem_ptr(cart, i);
			psu_c += shopping_cart_get_elem_count(cart, i);
			break;

		case SCI_RAM:
			for (j = 0; j < shopping_cart_get_elem_count(cart, i); ++j) {
				if (ram_c >= 8) {
					printf("Hata: Bu kadar bellek olmaz.\n");
					return 0;
				}

				ram[ram_c++] = shopping_cart_get_elem_ptr(cart, i);
			}

			break;

		case SCI_STORAGE_DEV:
			for (j = 0; j < shopping_cart_get_elem_count(cart, i); ++j) {
				if (sd_c >= 8) {
					printf("Hata: Bu kadar depolama aygiti olmaz.\n");
					return 0;
				}

				sd[sd_c++] = shopping_cart_get_elem_ptr(cart, i);
			}

			break;
		}
	}

	if (mb_c > 1) {
		printf("Hata: Birden fazla Anakart var.\n");
		return 0;
	}

	if (mb_c < 1) {
		printf("Hata: Anakart yok.\n");
		return 0;
	}

	if (p_c > 1) {
		printf("Hata: Birden fazla Islemci var.\n");
		return 0;
	}

	if (p_c < 1) {
		printf("Hata: Islemci yok.\n");
		return 0;
	}

	if (psu_c > 1) {
		printf("Hata: Birden fazla Guc Kaynagi var.\n");
		return 0;
	}

	if (psu_c < 1) {
		printf("Hata: Guc Kaynagi yok.\n");
		return 0;
	}

	if (ram_c < 1) {
		printf("Hata: Bellek yok.\n");
		return 0;
	}

	psoc = mainboard_get_psoc(mb);
	rsoc = mainboard_get_rsoc(mb);
	rsoc_count = mainboard_get_rsoc_count(mb);
	max_storage_dev_count = mainboard_get_storage_dev_count(mb);

	if (processor_get_psoc(p) != psoc) {
		printf("Hata: Islemci soketi uymuyor.\n");
		return 0;
	}

	ram_ref_clock_freq = ram_get_clock_freq(ram[0]);

	for (i = 0; i < ram_c; ++i) {
		if (ram_get_rsoc(ram[i]) != rsoc) {
			printf("Hata: Bellek soketi uymuyor.\n");
			return 0;
		}

		if (ram_get_clock_freq(ram[i]) != ram_ref_clock_freq) {
			printf("Hata: Belleklerin saat frekanslari uymuyor.\n");
			return 0;
		}
	}

	if (sd_c > max_storage_dev_count) {
		printf("Hata: Depolama aygiti sayisi soket sayisindan fazla.\n");
		return 0;
	}

	if (ram_c > rsoc_count) {
		printf("Hata: Bellek sayisi soket sayisindan fazla.\n");
		return 0;
	}

	total_power_usage += mainboard_get_power_usage(mb);
	total_power_usage += processor_get_power_usage(p);

	for (i = 0; i < ram_c; ++i) {
		total_power_usage += ram_get_power_usage(ram[i]);
	}

	for (i = 0; i < sd_c; ++i) {
		total_power_usage += storage_dev_get_power_usage(sd[i]);
	}

	if (psu_get_pw_output(psu) < total_power_usage) {
		printf("Hata: Guc Kaynaginin cikis gucu yetersiz.\n");
		return 0;
	}

	printf("Bilgisayar uyumludur.\n");
	return 1;
}


static void shopping_cart_loop(signal_t* signal, mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, ram_block_t* rb, psu_block_t* pb) {

	int op = 0;
	int ok = 0;
	int sel = -1;
	int c = 0;
	char code[20];
	int found = -1;

	char ch = 'e';

	FILE* fp;

	shopping_cart_t* cart;

	cart = shopping_cart_create();

	do {
		CLEAR_SCREEN();

		printf("Sepet Durumu\n\n");

		print_shopping_cart_billing(stdout, cart);

		putchar('\n');

		printf("1) Yeni parca ekle\n");
		printf("2) Parca miktari guncelle\n");
		printf("3) Parca sil\n");
		printf("4) Uyumluluk kontrolu yap\n");
		printf("5) Faturalandir\n");
		printf("6) Iptal et\n");

		printf("Secim: ");
		scanf("%d", &op);

		switch (op) {
		case 1:
			CLEAR_SCREEN();
			printf("1) Anakart ekle\n");
			printf("2) Islemci ekle\n");
			printf("3) Bellek ekle\n");
			printf("4) Depolama aygiti ekle\n");
			printf("5) Guc kaynagi ekle\n");

			printf("Secim: ");
			scanf("%d", &op);

			switch (op) {
			case 1:
				CLEAR_SCREEN();
				print_mainboard_array(stdout, mbb);
				do {
					printf("\n Secmek istediginiz anakartin numarasini yazin: ");
					scanf("%d", &sel);
				} while (!(sel >= 0 && sel <mbb->count));

				shopping_cart_add_item(cart, mbb->data[sel], SCI_MAINBOARD);

				break;
				
			case 2:
				CLEAR_SCREEN();
				print_processor_array(stdout, pcb);
				do {
					printf("\n Secmek istediginiz islemcinin numarasini yazin: ");
					scanf("%d", &sel);
				} while (!(sel >= 0 && sel <pcb->count));

				shopping_cart_add_item(cart, pcb->data[sel], SCI_PROCESSOR);

				break;

			case 3:
				CLEAR_SCREEN();
				print_ram_array(stdout, rb);
				do {
					printf("\n Secmek istediginiz bellegin numarasini yazin: ");
					scanf("%d", &sel);
				} while (!(sel >= 0 && sel <rb->count));

				shopping_cart_add_item(cart, rb->data[sel], SCI_RAM);

				break;

			case 4:
				CLEAR_SCREEN();
				print_storage_dev_array(stdout, sdb);
				do {
					printf("\n Secmek istediginiz depolama aygitinin numarasini yazin: ");
					scanf("%d", &sel);
				} while (!(sel >= 0 && sel <sdb->count));

				shopping_cart_add_item(cart, sdb->data[sel], SCI_STORAGE_DEV);

				break;

			case 5:
				CLEAR_SCREEN();
				print_psu_array(stdout, pb);
				do {
					printf("\n Secmek istediginiz guc kaynaginin numarasini yazin: ");
					scanf("%d", &sel);
				} while (!(sel >= 0 && sel <pb->count));

				shopping_cart_add_item(cart, pb->data[sel], SCI_PSU);

				break;
			}

			break;

		case 2:
			
			printf("Guncellemek istediginiz parcanin kodunu girin: ");
			scanf("%s", code);

			found = shopping_cart_find_elem_by_code(cart, code);

			if (found < 0) {
				printf("Sepette %s kodlu bir parca bulunamadi.\n", code);
				getchar();
				getchar();
				break;
			}

			do {
				printf("Parca adedini girin: ");
				scanf("%d", &c);
			} while (!(c >= 0));

			shopping_cart_set_elem_count(cart, found, c);

			break;

		case 3:

			printf("Silmek istediginiz parcanin kodunu girin: ");
			scanf("%s", code);

			found = shopping_cart_find_elem_by_code(cart, code);

			if (found < 0) {
				printf("Sepette %s kodlu bir parca bulunamadi.\n", code);
				getchar();
				getchar();
				break;
			}

			shopping_cart_remove_item(cart, found);

			break;

		case 4:
			check_items(cart);
			getchar();
			getchar();
			break;

		case 5:
			ch = 'e';

			/* Kontrol edelim - SS */
			if (check_items(cart) == 0) {

				printf("\n Yine de faturalandirmak istiyor musunuz? (e/h): ");

				do {
					/* TODO: Burayi duzelt - SS */
					ch = getchar();

					if (ch == 'h' || ch == 'e') {
						break;
					}

				} while (ch);
			}

			if (ch == 'e') {
				fp = fopen("fatura.txt", "a");
				print_shopping_cart_billing(fp, cart);
				fclose(fp);

				if (shopping_cart_get_item_count(cart) == 0) {
					*signal = NO_ITEMS_IN_CART;
				}
				else {
					*signal = BILLING_DONE;
				}

				ok = 1;
			}

			break;

		case 6:
			*signal = CANCELED;
			ok = 1;
			break;
		}

	} while (!ok);

	shopping_cart_destroy(cart);
}


static int read_in_range(const char* message, int min, int max) {
	int ret;

	do {
		printf(message);
		scanf("%d", &ret);
	} while (!(ret >= min && ret <= max));

	return ret;
}


static void get_computer_parts_selection(shopping_cart_item_type_t type, mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, 
	ram_block_t* rb, psu_block_t* pb, shopping_cart_t* cart, computer_info_t* info) {

	query_result_t res = { 0 };
	query_result_t res2 = { 0 };

	void* selection;
	int sel;
	int found = -1;
	void* found_ptr = 0;

	switch (type) {
	case SCI_MAINBOARD:
		/* Sepette islemci yok - SS */
		if (shopping_cart_find_elem_by_type(cart, SCI_PROCESSOR) == -1) {
			info->psoc = PSC_ANY;
		}

		/* Sepette ram yok - SS */
		if (shopping_cart_find_elem_by_type(cart, SCI_RAM) == -1) {
			info->rsoc = RSC_ANY;
		}

		mainboard_query(&res, mbb, mainboard_select_socket, info->psoc, info->rsoc, info->rsoc_used, info->storage_dev_used, 0, 0);
		
		if (info->psu_pw_output > 0) { /* PSU var, ona gore secmek icin query etmeliyiz - SS */
			mainboard_query(&res2, &res, mainboard_select_pw_max, 0, 0, 0, 0, info->psu_pw_output - info->pw_total, 0);
			memcpy(&res, &res2, sizeof(query_result_t));
		}

		print_mainboard_array(stdout, &res);

		sel = read_in_range("\n Secmek istediginiz Anakartin numarasini yazin (Cikmak icin -1): ", -1, res.count - 1);
		if (sel == -1) {
			return;
		}

		/* Secim mevcut, yenisini sec - SS */
		if ((found = shopping_cart_find_elem_by_type(cart, SCI_MAINBOARD)) > -1) {
			found_ptr = shopping_cart_get_elem_ptr(cart, found);
			info->pw_total -= mainboard_get_power_usage(found_ptr);
			shopping_cart_remove_item(cart, found);
		}

		selection = res.elems[sel];
		info->psoc = mainboard_get_psoc(selection);
		info->rsoc = mainboard_get_rsoc(selection);
		info->rsoc_count = mainboard_get_rsoc_count(selection);
		info->storage_dev_count = mainboard_get_storage_dev_count(selection);

		shopping_cart_add_item(cart, selection, SCI_MAINBOARD);

		info->pw_total += mainboard_get_power_usage(selection);

		break;

	case SCI_PROCESSOR:
		/* Sepette anakart yok - SS */
		if (shopping_cart_find_elem_by_type(cart, SCI_MAINBOARD) == -1) {
			info->psoc = PSC_ANY;
		}

		processor_query(&res, pcb, processor_select_socket, info->psoc, 0, 0);

		if (info->psu_pw_output > 0) { /* PSU var, ona gore secmek icin query etmeliyiz - SS */
			processor_query(&res2, &res, processor_select_pw_max, 0, info->psu_pw_output - info->pw_total, 0);
			memcpy(&res, &res2, sizeof(query_result_t));
		}

		print_processor_array(stdout, &res);

		sel = read_in_range("\n Secmek istediginiz Islemcinin numarasini yazin (Cikmak icin -1): ", -1, res.count - 1);
		if (sel == -1) {
			return;
		}

		/* Secim mevcut, yenisini sec - SS */
		if ((found = shopping_cart_find_elem_by_type(cart, SCI_PROCESSOR)) > -1) {
			found_ptr = shopping_cart_get_elem_ptr(cart, found);
			info->pw_total -= processor_get_power_usage(found_ptr);
			shopping_cart_remove_item(cart, found);
		}

		selection = res.elems[sel];
		info->psoc = processor_get_psoc(selection);

		shopping_cart_add_item(cart, selection, SCI_PROCESSOR);

		info->pw_total += processor_get_power_usage(selection);

		break;

	case SCI_PSU:
		psu_query(&res, pb, psu_select_pw_min, info->pw_total, 0);

		print_psu_array(stdout, &res);

		sel = read_in_range("\n Secmek istediginiz Guc Kaynaginin numarasini yazin (Cikmak icin -1): ", -1, res.count - 1);
		if (sel == -1) {
			return;
		}

		/* Secim mevcut, yenisini sec - SS */
		if ((found = shopping_cart_find_elem_by_type(cart, SCI_PSU)) > -1) {
			shopping_cart_remove_item(cart, found);
		}

		selection = res.elems[sel];
		info->psu_pw_output = psu_get_pw_output(selection);

		shopping_cart_add_item(cart, selection, SCI_PSU);

		break;

	case SCI_RAM:
		if (info->rsoc_used > 0) { /* Daha once ram konmus - SS */
			if (info->rsoc_count > 0) { /* Anakart eklendiginden emin olalim */
				/* Hepsi doldu */
				if (info->rsoc_used >= info->rsoc_count) {
					printf("Hata: Butun bellek soketlerini doldurdunuz, daha fazla ekleyemezsiniz.\n");
					getchar();
					getchar();
					break;
				}
			}

			ram_query(&res, rb, ram_select_socket_clock_freq, info->rsoc, 0, 0, info->ram_clock_freq, 0);
		}

		else {
			ram_query(&res, rb, ram_select_socket, info->rsoc, 0, 0, 0, 0);
		}

		if (info->psu_pw_output > 0) { /* PSU var, ona gore secmek icin query etmeliyiz - SS */
			ram_query(&res2, &res, ram_select_pw_max, 0, info->psu_pw_output - info->pw_total, 0, 0, 0);
			memcpy(&res, &res2, sizeof(query_result_t));
		}

		print_ram_array(stdout, &res);

		sel = read_in_range("\n Secmek istediginiz Bellegin numarasini yazin (Cikmak icin -1): ", -1, res.count - 1);
		if (sel == -1) {
			return;
		}

		selection = res.elems[sel];
		info->rsoc = ram_get_rsoc(selection);
		info->rsoc_used++;
		info->ram_clock_freq = ram_get_clock_freq(selection);

		shopping_cart_add_item(cart, selection, SCI_RAM);

		info->pw_total += ram_get_power_usage(selection);

		break;

	case SCI_STORAGE_DEV:
		memcpy(&res, sdb, sizeof(storage_dev_block_t));

		if (info->storage_dev_count > 0) {
			/* Depolama aygiti limitini astik - SS */
			if (info->storage_dev_used >= info->storage_dev_count) {
				printf("Hata: Butun depolama aygiti soketlerini doldurdunuz, daha fazla ekleyemezsiniz.\n");
				getchar();
				getchar();
				break;
			}
		}

		if (info->psu_pw_output > 0) { /* PSU var, ona gore secmek icin query etmeliyiz - SS */
			storage_dev_query(&res2, &res, storage_dev_select_pw_max, 0, info->psu_pw_output - info->pw_total, 0);
			memcpy(&res, &res2, sizeof(query_result_t));
		}

		print_storage_dev_array(stdout, &res);

		sel = read_in_range("\n Secmek istediginiz Depolama Aygitinin numarasini yazin (Cikmak icin -1): ", -1, res.count - 1);
		if (sel == -1) {
			return;
		}

		selection = res.elems[sel];
		info->storage_dev_used++;

		shopping_cart_add_item(cart, selection, SCI_STORAGE_DEV);

		info->pw_total += storage_dev_get_power_usage(selection);

		break;
	}
}


static void get_computer_parts_selection_ui(signal_t* signal, mainboard_block_t* mbb, processor_block_t* pcb, storage_dev_block_t* sdb, 
	ram_block_t* rb, psu_block_t* pb) {

	shopping_cart_t* cart;
	computer_info_t info = { 0 };

	int op = 0;
	int ok = 0;

	char ch = 'e';

	FILE *fp;

	cart = shopping_cart_create();

	do {
		CLEAR_SCREEN();

		printf("Sepet Durumu\n\n");

		print_shopping_cart_billing(stdout, cart);

		putchar('\n');

		printf("1) Anakart sec\n");
		printf("2) Islemci sec\n");
		printf("3) Guc kaynagi sec\n");
		printf("4) Bellek ekle\n");
		printf("5) Depolama aygiti ekle\n");
		printf("6) Faturalandir\n");
		printf("7) Iptal et\n");

		printf("Secim: ");
		scanf("%d", &op);

		switch (op) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			CLEAR_SCREEN();
			get_computer_parts_selection(op - 1, mbb, pcb, sdb, rb, pb, cart, &info);
			break;

		case 6:
			ch = 'e';

			/* Kontrol edelim - SS */
			if (check_items(cart) == 0) {

				printf("\n Yine de faturalandirmak istiyor musunuz? (e/h): ");

				do {
					/* TODO: Burayi duzelt - SS */
					ch = getchar();
					
					if (ch == 'h' || ch == 'e') {
						break;
					}

				} while (ch);
			}

			if (ch == 'e') {
				fp = fopen("fatura.txt", "a");
				print_shopping_cart_billing(fp, cart);
				fclose(fp);

				if (shopping_cart_get_item_count(cart) == 0) {
					*signal = NO_ITEMS_IN_CART;
				}
				else {
					*signal = BILLING_DONE;
				}

				ok = 1;
			}

			break;

		case 7:
			*signal = CANCELED;

			ok = 1;
			break;
		}


	} while (!ok);

	shopping_cart_destroy(cart);
}


int main(int argc, char* argv[]) {
	void* block;
	void* block_end;

	mainboard_block_t mbb = {0};
	processor_block_t pcb = {0};
	storage_dev_block_t sdb = {0};
	ram_block_t rb = {0};
	psu_block_t pb = {0};

	FILE* fp;

	int ok = 0;
	int op = 0;
	int num = 0;
	int capacity = 0;

	signal_t signal = -1;

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

	while (!ok) {
		CLEAR_SCREEN();

		switch (signal) {
		case NO_MATCHING_HARDWARE:
			printf("\nIstediginiz ozelliklerde bilgisayar bulunamadi.\n\n");
			break;

		case BILLING_DONE:
			printf("\nFaturalandirildi.\n\n");
			break;

		case CANCELED:
			printf("\nIptal edildi.\n\n");
			break;

		case NO_ITEMS_IN_CART:
			printf("\nSepet bos.\n\n");
			break;
		}

		signal = -1;

		printf("1) Yeni sepet olustur\n");
		printf("2) Sirali sihirbaz baslat\n");
		printf("3) Sirasiz sihirbaz baslat\n");
		printf("4) En ucuz bilgisayari goster\n");
		printf("5) Minimum bellek kapasitesine gore en ucuz bilgisayari goster\n");
		printf("6) Cikis\n");

		printf("Secim: ");
		scanf("%d", &op);

		switch (op) {
		case 1:
			shopping_cart_loop(&signal, &mbb, &pcb, &sdb, &rb, &pb);
			break;

		case 2:
			get_computer_parts_ui(&signal, &mbb, &pcb, &sdb, &rb, &pb);
			break;
		
		case 3:
			get_computer_parts_selection_ui(&signal, &mbb, &pcb, &sdb, &rb, &pb);
			break;

		case 4:
			CLEAR_SCREEN();
			low_cost_ui(&signal, &mbb, &pcb, &sdb, &rb, &pb, 0);
			break;

		case 5:
			CLEAR_SCREEN();
			printf("Minimum bellek kapasitesi: ");
			scanf("%d", &capacity);
			low_cost_ui(&signal, &mbb, &pcb, &sdb, &rb, &pb, capacity);
			break;

		case 6:
			ok = 1;
			break;

		default:
			break;
		}
	}

	free(block); /* Bununla isimiz bitti - SS */

    return 0;
}