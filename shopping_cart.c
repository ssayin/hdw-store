#include "shopping_cart.h"

#include <assert.h>
#include <string.h>


struct shopping_cart {
	shopping_cart_item_t items[100];
	size_t count;
};


void shopping_cart_add_item(shopping_cart_t* cart, void* ptr, shopping_cart_item_type_t type) {
	int i;

	for (i = 0; i < cart->count; ++i) {
		if (cart->items[i].ptr == ptr) {
			cart->items[i].count++;
			return;
		}
	}

	if (cart->count > 99) { /* Maks. eleman sayisina ulasildi - SS */
		return;
	}

	cart->items[cart->count].ptr = ptr;
	cart->items[cart->count].type = type;
	cart->items[cart->count].count = 1;
	cart->count++;
}


shopping_cart_t* shopping_cart_create() {
	shopping_cart_t* cart;
	cart = malloc(sizeof(struct shopping_cart));
	cart->count = 0;
	return cart;
}


void shopping_cart_destroy(shopping_cart_t* cart) {
	free(cart);
}


size_t shopping_cart_get_item_count(shopping_cart_t* cart) {
	return cart->count;
}


void* shopping_cart_get_elem_ptr(shopping_cart_t* cart, int index) {
	assert(index >= 0 && index < cart->count);
	return cart->items[index].ptr;
}


shopping_cart_item_type_t shopping_cart_get_elem_type(shopping_cart_t* cart, int index) {
	assert(index >= 0 && index < cart->count);
	return cart->items[index].type;
}


size_t shopping_cart_get_elem_count(shopping_cart_t* cart, int index) {
	assert(index >= 0 && index < cart->count);
	return cart->items[index].count;
}


void shopping_cart_set_elem_count(shopping_cart_t* cart, int index, size_t count) {
	assert(index >= 0 && index < cart->count);
	cart->items[index].count = count;
}


void shopping_cart_remove_item(shopping_cart_t* cart, int index) {
	int i;

	/* Kaydiriyorum - SS */
	for (i = index; i < cart->count - 1; ++i) {
		cart->items[i] = cart->items[i + 1];
	}

	cart->count--;
}


int shopping_cart_find_elem_by_code(shopping_cart_t* cart, const char* code) {
	
	int i;
	char loc_code[20];
	void* ptr;

	for (i = 0; i < cart->count; ++i) {

		ptr = cart->items[i].ptr;

		switch (cart->items[i].type) {
		case SCI_MAINBOARD:
			strcpy(loc_code, mainboard_get_code(ptr));
			break;
			
		case SCI_PROCESSOR:
			strcpy(loc_code, processor_get_code(ptr));
			break;

		case SCI_PSU:
			strcpy(loc_code, psu_get_code(ptr));
			break;

		case SCI_RAM:
			strcpy(loc_code, ram_get_code(ptr));
			break;

		case SCI_STORAGE_DEV:
			strcpy(loc_code, storage_dev_get_code(ptr));
			break;
		}

		if (strcmp(loc_code, code) == 0) {
			return i;
		}
	}

	return -1;
}


int shopping_cart_find_elem_by_type(shopping_cart_t* cart, shopping_cart_item_type_t type) {

	int i;

	for (i = 0; i < cart->count; ++i) {
		if (cart->items[i].type == type) {
			return i;
		}
	}

	return -1;
}