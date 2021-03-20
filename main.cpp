#include <cstddef>
#include <stdio.h>
#include <memory>
#include <string_view>
#include <inttypes.h>
#include <vector>
#include <atomic>
#include <thread>
#include <iostream>
#include "memset.h"
#include "stopwatch.h"

#if !_M_AMD64
#error x64 or don't bother.
#endif

extern "C" int ASM_Test();

namespace {
	std::byte *whatever;
	uint64_t buffer_size;
	std::byte *buffer_1;
	std::byte *buffer_2;
}

inline void AssertRelease(bool condition, std::string_view message) {
	if (!condition) {
		MessageBoxA(0, message.data(), "Error", MB_OK + MB_ICONERROR);
		DebugBreak();
	}
}

inline constexpr bool IsAligned(const void * ptr, std::uintptr_t alignment) noexcept {
    std::uintptr_t address = (std::uintptr_t)ptr;
    return !(address % alignment);
}


void CheckBuffers(std::string_view name) {
	if (std::memcmp(buffer_1, buffer_2, buffer_size) != 0) {
		for (int i = 0; i < 10; i++) {
			printf("********** %s is broken **********\n", name.data());
		}
	}
}

void Test_Run() {
	//
	// Make sure these pages are in the working set.
	//
	std::memset(buffer_1, 1, buffer_size);
	std::memset(buffer_2, 1, buffer_size);

	//
	// We will compare the other memsets against this (perf and correctness)
	//
	double start1 = StopWatch::MSec();
	std::memset(buffer_1, 1, buffer_size);
	double end1 = StopWatch::MSec();

	//
	// SIMD I: MOVDQA x 8
	//
	double start2 = StopWatch::MSec();
	Memset_SIMD_MOVDQA(buffer_2, 1, buffer_size);
	double end2 = StopWatch::MSec();

	CheckBuffers("MOVDQA");
	

	//
	// SIMD II: MOVNTPS x 8
	//
	std::memset(buffer_2, 9099, buffer_size);
	double start3 = StopWatch::MSec();
	Memset_SIMD_MOVNTPS(buffer_2, 1, buffer_size);
	double end3 = StopWatch::MSec();

	CheckBuffers("MOVNTPS");

	
	//
	// Manual 64bit stores
	//
	std::memset(buffer_2, 9099, buffer_size);
	double start4 = StopWatch::MSec();
	Memset_8bytes(buffer_2, 1, buffer_size);
	double end4 = StopWatch::MSec();

	CheckBuffers("x64");

	printf("\n");
	printf("std::memset        : %2.2f\n", end1 - start1);
	printf("SIMD (MOVDQA)      : %2.2f\n", end2 - start2);	
	printf("SIMD2 (MOVNTPS)    : %2.2f\n", end3 - start3);
	printf("x64 (64bit stores) : %2.2f\n", end4 - start4);
}

void ASM_Code() {
	int i = ASM_Test();
	printf("\n\nASM_Test is %d\n", i);
}

void Test_1() {
	printf("\n");
	printf("	Starting Test 1\n");
	printf("\n");

	static_assert(__STDCPP_DEFAULT_NEW_ALIGNMENT__ == 16, "This is not going to work. Consider using Mem_Alloc/Mem_Free.");

	buffer_size = (uint64_t(1024) * 1024 * 512);
	buffer_1 = new std::byte[buffer_size];
	buffer_2 = new std::byte[buffer_size];
	
	AssertRelease(IsAligned(buffer_1, 16) == true, "Buffer1 is not aligned!");
	AssertRelease(IsAligned(buffer_2, 16) == true, "Buffer2 is not aligned!");

	printf("buffer_size is %" PRIu64 " bytes\n", buffer_size);
	
	for (int i = 0; i < 1; i++) {
		Test_Run();		
	}
	
	delete [] buffer_1;
	delete [] buffer_2;	

	printf("\n");
	printf("===============================\n");
	printf("\n");
}


void Test_2() {
	printf("\n");
	printf("	Starting Test 2\n");
	printf("\n");


	printf("\n");
	printf("===============================\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	Test_1();
	
	Test_2();

	ASM_Code();

	while (1) {
		Sleep(50);
	}

	return 0;
}
