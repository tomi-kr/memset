#include "memset.h"

void* Mem_Alloc(uint64_t size) {
	return _aligned_malloc(size, 16);
}

void Mem_Free(void *ptr) {
	return _aligned_free(ptr);
}

void Memset_SIMD(const void *start_address, int value, uint64_t length) {
    char c = (char)value;
	unsigned char *byte_buffer = (unsigned char *)start_address;
    __m128i block = _mm_set_epi8(c, c, c, c, 
								 c, c, c, c,
								 c, c, c, c, 
								 c, c, c, c);
    
	while (length >= 32) {
		_mm_storeu_si128((__m128i*)&byte_buffer[0], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[16], block);
		byte_buffer += 32;
		length -= 32;
	}

	while (length > 0) {
		*byte_buffer = (unsigned char)c;
		byte_buffer++;
		length--;
	}
}


void Memset_Manual(const void *start_address, int value, uint64_t length) {
	uint64_t* long_pointer = (uint64_t *)start_address;
	//unsigned char* debug_pointer = (unsigned char *)(long_pointer);
	unsigned char c		   = (unsigned char)value;
	uint64_t storage;

	((unsigned char *)&storage)[0] = c;
	((unsigned char *)&storage)[1] = c;
	((unsigned char *)&storage)[2] = c;
	((unsigned char *)&storage)[3] = c;
	((unsigned char *)&storage)[4] = c;
	((unsigned char *)&storage)[5] = c;
	((unsigned char *)&storage)[6] = c;
	((unsigned char *)&storage)[7] = c;


#if 0
	while (length >= 8) {
		for (int i = 0; i < 8; i++) {
			*long_pointer = storage;		
			long_pointer++;
			length -= 8;
		}
	}
#else
	while (length >= 8) {
		*long_pointer = storage;		
		long_pointer++;
        length -= 8;
	}
#endif

	volatile unsigned char* byte_pointer = (unsigned char *)(long_pointer);
	while (length > 0) {
		*byte_pointer = c;
		byte_pointer++;
		length--;
	}
}
