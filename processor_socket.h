#ifndef __PROCESSOR_SOCKET_H__
#define __PROCESSOR_SOCKET_H__

#include <string.h>

#include "platform.h"

typedef enum {
	PSC_ANY = 0,
    PSC_FM2p,
    PSC_AM3p,
    PSC_1150,
    PSC_1151
} processor_socket_t;

static inline processor_socket_t to_processor_socket(char* arr) {
	if (strcmp(arr, "FM2+") == 0)
		return PSC_FM2p;
	if (strcmp(arr, "AM3+") == 0)
		return PSC_AM3p;
	if (strcmp(arr, "1150") == 0)
		return PSC_1150;
	if (strcmp(arr, "1151") == 0)
		return PSC_1151;
}

static inline const char* psoc_to_string(processor_socket_t psc) {
	switch (psc) {
	case PSC_FM2p:
		return "FM2+";
	case PSC_AM3p:
		return "AM3+";
	case PSC_1150:
		return "1150";
	case PSC_1151:
		return "1151";
	}
}

#endif