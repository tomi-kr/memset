#include "memset.h"


void* Mem_Alloc(uint64_t size) {
	return _aligned_malloc(size, 16);
}


void Mem_Free(void *ptr) {
	return _aligned_free(ptr);
}


void Memset_SIMD_MOVDQA(const void *start_address, int value, uint64_t length) {
    char c = (char)value;
	unsigned char *byte_buffer = (unsigned char *)start_address;
    __m128i block = _mm_set_epi8(c, c, c, c, 
								 c, c, c, c,
								 c, c, c, c, 
								 c, c, c, c);
    
	while (length >= 128) {
		_mm_storeu_si128((__m128i*)&byte_buffer[0], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[16], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[32], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[48], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[64], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[80], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[96], block);
		_mm_storeu_si128((__m128i*)&byte_buffer[112], block);

		byte_buffer += 128;
		length -= 128;
	}

	while (length > 0) {
		*byte_buffer = (unsigned char)c;
		byte_buffer++;
		length--;
	}
}


void Memset_SIMD_MOVNTPS(const void *start_address, int value, uint64_t length) {
    unsigned char c = (unsigned char)value;
	float *float_buffer = (float *)start_address;
	alignas(16) unsigned char block[16] = {c, c, c, c, 
										   c, c, c, c, 
										   c, c, c, c, 
										   c, c, c, c};
    
	while (length >= 128) {
		_mm_stream_ps((float*)&float_buffer[0],  *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[4],  *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[8],  *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[12], *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[16], *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[20], *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[24], *(__m128*)block);
		_mm_stream_ps((float*)&float_buffer[28], *(__m128*)block);

		float_buffer += 32;
		length -= 128;
	}

	volatile unsigned char * byte_buffer = (unsigned char*)float_buffer;
	while (length > 0) {
		*byte_buffer = (unsigned char)c;
		byte_buffer++;
		length--;
	}
}


void Memset_8bytes(const void *start_address, int value, uint64_t length) {
	uint64_t* long_pointer = (uint64_t *)start_address;
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

	while (length >= 8) {
		*long_pointer = storage;		
		long_pointer++;
        length -= 8;
	}

	volatile unsigned char* byte_pointer = (unsigned char *)(long_pointer);
	while (length > 0) {
		*byte_pointer = c;
		byte_pointer++;
		length--;
	}
}
