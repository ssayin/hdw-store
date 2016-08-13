#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdlib.h>

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
	// Windows (x64 and x86)
	#define CLEAR_SCREEN()	(system("cls"))
#elif __unix__ // all unices, not all compilers
	// Unix
	#define CLEAR_SCREEN()	(system("clear"))
#elif __linux__
	// linux
	#define CLEAR_SCREEN()	(system("clear"))
#elif __APPLE__
	// Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
	#define CLEAR_SCREEN()	(system("clear"))
#endif

#if defined __GNUC__ && !defined __GNUC_STDC_INLINE__ && !defined __GNUC_GNU_INLINE__
	# define __GNUC_GNU_INLINE__ 1
#endif

#endif