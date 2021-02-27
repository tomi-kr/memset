#ifndef MEMSET_H_
#define MEMSET_H_

#include <malloc.h>
#include <stdint.h>
#include <immintrin.h>

void* Mem_Alloc(uint64_t size);
void Mem_Free(void *ptr);

void Memset_SIMD(const void *start_address, int value, uint64_t length);
void Memset_Manual(const void *start_address, int value, uint64_t length);

#endif
