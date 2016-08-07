#include "shopping_cart.h"

#include <assert.h>


struct shopping_cart {
	shopping_cart_item_t items[100];
	size_t count;
};


void shopping_cart_add_item(shopping_cart_t* cart, void* ptr, shopping_cart_item_type_t type, int computer) {
	cart->items[cart->count].ptr = ptr;
	cart->items[cart->count].type = type;
	cart->items[cart->count].computer = computer;
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


int shopping_cart_get_computer_num(shopping_cart_t* cart, int index) {
	assert(index >= 0 && index < cart->count);
	return cart->items[index].computer;
}