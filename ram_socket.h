#ifndef __RAM_SOCKET_H__
#define __RAM_SOCKET_H__

#include "platform.h"

typedef enum {
    RSC_DDR3 = 0,
    RSC_DDR4
} ram_socket_t;

static inline ram_socket_t to_ram_socket(char* arr) {
	if (strcmp(arr, "DDR3") == 0)
		return RSC_DDR3;
	if (strcmp(arr, "DDR4") == 0)
		return RSC_DDR4;
}

static inline const char* rsoc_to_string(ram_socket_t rsc) {
	switch (rsc) {
	case RSC_DDR3:
		return "DDR3";
	case RSC_DDR4:
		return "DDR4";
	}
}

#endif