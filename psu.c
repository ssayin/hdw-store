#include "psu.h"
#include "common_traits.h"

#include <string.h>


struct psu {
    common_traits_t common;
    unsigned int pw_output;
};


void psu_set(psu_t* psu, unsigned int price, 
	char* brand, char* code,
	unsigned int pw_output) {

	psu->common.price = price;
	strcpy(psu->common.brand, brand);
	strcpy(psu->common.code, code);
	psu->pw_output = pw_output;
}


const char* psu_get_brand(psu_t* psu) {
	return psu->common.brand;
}


const char* psu_get_code(psu_t* psu) {
	return psu->common.code;
}


unsigned int psu_get_price(psu_t* psu) {
	return psu->common.price;
}


unsigned int psu_get_pw_output(psu_t* psu) {
	return psu->pw_output;
}


size_t psu_struct_size() {
	return sizeof(psu_t);
}