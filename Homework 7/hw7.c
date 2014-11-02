/*
Homework #7
Name: Andrew Rosiclair
SBU ID: 109235970
*/

#include <stdlib.h>
#include "hw7.h"

/* Debugging macros */
#ifdef DEBUG
	#define debug(fmt, ...) printf("DEBUG: %s:%s:%d " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define debug(fmt, ...)
#endif

/* Macros required for grading */
#ifdef CSE220
/* Define assignment macros here */
#else
/* Define assignment macros here */
#endif