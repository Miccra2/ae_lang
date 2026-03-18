#ifndef __UTILS_H__
#define __UTILS_H__

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifndef DA_MIN_CAP
#define DA_MIN_CAP (512)
#endif // DA_MIN_CAP

#define DA_EMPTY {.capacity=0, .length=0, .buffer=NULL}
#define DA_GET(da, idx) do { assert(idx < (da).length && "Trying to access dynamic array outside of bounds!"); (da).buffer[(idx)] } while (0)

#define DA_RESERVE(da, len) do {													\
	size_t __cap = (da).capacity;													\
	if ((da).capacity == 0) {														\
		(da).capacity = DA_MIN_CAP;													\
	}																				\
	while ((da).capacity < len) {													\
		(da).capacity *= 2;															\
	}																				\
	if (__cap != (da).capacity) {													\
		(da).buffer = realloc((da).buffer, (da).capacity * sizeof((da).buffer[0]));	\
		assert((da).buffer && "Could not allocate dynamic array!");					\
	}																				\
} while (0)

#define DA_PUSH(da, item) do { DA_RESERVE((da), (da).length + 1); (da).buffer[(da).length++] = item; } while (0)

#endif // __UTILS_H__
