#include "sort.h"


static void __Swap(void** a, void** b);


void query_bubble_sort(query_result_t* res, bubble_sort_cb_t func) { /* TODO: Optimize etmemiz gerek - SS */
	size_t i, j;
	void* a;
	void* b;
	for (i = 0; i < res->count; i++) {
		for (j = 0; j < res->count; j++) {
			//a = arr[i];
			//b = arr[j];
			if (func(res->elems[i], res->elems[j]) < 0)
				__Swap(&res->elems[i], &res->elems[j]);
		}
	}
}


static void __Swap(void** a, void** b) {
	void* temp;
	temp = *a;
	*a = *b;
	*b = temp;
}