#include "memset.h"

/*

===============================================================================

	
	Things about memset:

		- compile time loop unrolling is not reliable

		- unrolling doesn't make to much difference on my PC at the moment (maxing out memory bandwidth already?)

===============================================================================

template <int N>
struct Unroll {
	template <typename F>
	static void ThisFunction(const F& f) {
		f(N);
		Unroll<N - 1>::ThisFunction(f);
	}
};

template <>
struct Unroll<1> {
	template <typename F>
	static void ThisFunction(const F& f) {
		f(1);
	}
};
*/

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
    
	while (length >= 64) {
		_mm_storeu_si128((__m128i*)(byte_buffer), block);
		_mm_storeu_si128((__m128i*)(byte_buffer + 16), block);
		_mm_storeu_si128((__m128i*)(byte_buffer + 32), block);
		_mm_storeu_si128((__m128i*)(byte_buffer + 48), block);
		/*_mm_storeu_si128((__m128i*)(byte_buffer + 64), block);
		_mm_storeu_si128((__m128i*)(byte_buffer + 80), block);
		_mm_storeu_si128((__m128i*)(byte_buffer + 96), block);
		_mm_storeu_si128((__m128i*)(byte_buffer + 112), block);*/

		byte_buffer += 64;
		length -= 64;
	}

	while (length > 0) {
		*byte_buffer = (unsigned char)c;
		byte_buffer++;
		length--;
	}
}

constexpr int Unroll_Factor = 1;

void Memset_SIMD_MOVNTPS(const void *start_address, int value, uint64_t length) {
    unsigned char c = (unsigned char)value;
	float *float_buffer = (float *)start_address;
	alignas(16) unsigned char block[16] = {c, c, c, c, 
										   c, c, c, c, 
										   c, c, c, c, 
										   c, c, c, c};
	__m128 vector = _mm_set_ps1(*(float*)block);

    
	while (length >= 64) {
		_mm_stream_ps(float_buffer +  0,  vector);
		_mm_stream_ps(float_buffer +  4,  vector);
		_mm_stream_ps(float_buffer +  8,  vector);
		_mm_stream_ps(float_buffer + 12, vector);		
		/*_mm_stream_ps(float_buffer + 16, vector);
		_mm_stream_ps(float_buffer + 20, vector);
		_mm_stream_ps(float_buffer + 24, vector);
		_mm_stream_ps(float_buffer + 28, vector);*/

		float_buffer += 16;
		length -= 64;
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
