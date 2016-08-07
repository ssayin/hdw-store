#ifndef __SORT_H__
#define __SORT_H__

#include "query.h"
#include "block_types.h"

typedef int(bubble_sort_cb_t) (void* a, void* b);

void query_bubble_sort(query_result_t* res, bubble_sort_cb_t func);

#endif