#ifndef MEMSET_H_
#define MEMSET_H_

#include <malloc.h>
#include <stdint.h>
#include <immintrin.h>

void* Mem_Alloc(uint64_t size);
void Mem_Free(void *ptr);

void Memset_SIMD_MOVDQA(const void *start_address, int value, uint64_t length);
void Memset_SIMD_MOVNTPS(const void *start_address, int value, uint64_t length);
void Memset_8bytes(const void *start_address, int value, uint64_t length);

#endif
