#ifndef __SHOPPING_CART_H__
#define __SHOPPING_CART_H__

#include <stdlib.h>

typedef enum {
	SCI_MAINBOARD = 0,
	SCI_PROCESSOR,
	SCI_PSU,
	SCI_RAM,
	SCI_STORAGE_DEV
} shopping_cart_item_type_t;

typedef struct {
	void* ptr;
	shopping_cart_item_type_t type;
	size_t count;
} shopping_cart_item_t;

typedef struct shopping_cart shopping_cart_t;

shopping_cart_t* shopping_cart_create();
void shopping_cart_add_item(shopping_cart_t* cart, void* ptr, shopping_cart_item_type_t type);
void shopping_cart_remove_item(shopping_cart_t* cart, int index);
void shopping_cart_destroy(shopping_cart_t* cart);

void shopping_cart_set_elem_count(shopping_cart_t* cart, int index, size_t count);

size_t shopping_cart_get_item_count(shopping_cart_t* cart);
void* shopping_cart_get_elem_ptr(shopping_cart_t* cart, int index);
shopping_cart_item_type_t shopping_cart_get_elem_type(shopping_cart_t* cart, int index);
size_t shopping_cart_get_elem_count(shopping_cart_t* cart, int index);

int shopping_cart_find_elem_by_code(shopping_cart_t* cart, const char* code);
/* Belirtilen tipteki ilk elemani dondurur, bulamazsa -1 dondurur. */
int shopping_cart_find_elem_by_type(shopping_cart_t* cart, shopping_cart_item_type_t type);

#endif