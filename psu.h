#ifndef __PSU_H__
#define __PSU_H__

#include <stdlib.h>

typedef struct psu psu_t;

void psu_set(psu_t* psu, unsigned int price, 
	char* brand, char* code,
	unsigned int pw_output);

const char* psu_get_brand(psu_t* psu);
const char* psu_get_code(psu_t* psu);
unsigned int psu_get_price(psu_t* psu);
unsigned int psu_get_pw_output(psu_t* psu);
size_t psu_struct_size();

#endif