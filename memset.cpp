#include "memset.h"

void* Mem_Alloc(uint64_t size) {
	return _aligned_malloc(size, 16);
}

void Mem_Free(void *ptr) {
	return _aligned_free(ptr);
}
