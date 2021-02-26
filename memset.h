#ifndef MEMSET_H_
#define MEMSET_H_

#include <malloc.h>
#include <stdint.h>
#include <Windows.h>
#include <immintrin.h>

struct StopWatch {
    LARGE_INTEGER StartingTime;
    double        Frequency;

    StopWatch() {
		Reset();
    }

    void Reset() {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        Frequency = double(freq.QuadPart);
        QueryPerformanceCounter(&StartingTime);
    }

    double GetTimeMs() {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        return ((double(EndingTime.QuadPart - StartingTime.QuadPart) * 1000.0) / Frequency);
    }

    double GetTimeMicro() {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        return ((double(EndingTime.QuadPart - StartingTime.QuadPart) * 1000000.0) / Frequency);
    }

    static StopWatch *it() {
        static StopWatch timer;
        return &timer;
    }

    static double msec() {
        return it()->GetTimeMs();
    }

    static double microsec() {
        return it()->GetTimeMicro();
    }
};



void* Mem_Alloc(uint64_t size);
void Mem_Free(void *ptr);

void Memset_SIMD(const void *start_address, int value, uint64_t length);
void Memset_Manual(const void *start_address, int value, uint64_t length);

#endif
