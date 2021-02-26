#include <cstddef>
#include <stdio.h>
#include <memory>
#include "memset.h"

extern "C" int ASM_Test();

namespace {
	uint64_t buffer_size;
	std::byte *buffer_1;
	std::byte *buffer_2;
}

void Test_Init() {
	buffer_size = uint64_t(1024) * 1024 * 512;
	buffer_1 = (std::byte*)Mem_Alloc(buffer_size);
	buffer_2 = (std::byte*)Mem_Alloc(buffer_size);
}

void Test_Run() {
	std::memset(buffer_1, 9099, buffer_size);
	std::memset(buffer_2, 9099, buffer_size);

	double start1 = StopWatch::msec();
	std::memset(buffer_1, 1, buffer_size);
	double end1 = StopWatch::msec();

	double start2 = StopWatch::msec();
	Memset_SIMD(buffer_2, 1, buffer_size);
	double end2 = StopWatch::msec();

	if (std::memcmp(buffer_1, buffer_2, buffer_size) != 0) {
		for (int i = 0; i < 10; i++) {
			printf("********** Memset_SIMD is broken **********\n");
		}
	}

	// reset
	std::memset(buffer_2, 9099, buffer_size);

	double start3 = StopWatch::msec();
	Memset_Manual(buffer_2, 1, buffer_size);
	double end3 = StopWatch::msec();

	if (std::memcmp(buffer_1, buffer_2, buffer_size) != 0) {
		for (int i = 0; i < 10; i++) {
			printf("********** Memset_Manual is broken **********\n");
		}
	}

	printf("\n");
	printf("std::memset: %2.2f\n", end1 - start1);
	printf("SIMD       : %2.2f\n", end2 - start2);
	printf("Manual     : %2.2f\n", end3 - start3);
}

void Test_Shutdown() {
	Mem_Free(buffer_2);
	Mem_Free(buffer_1);
}

int main(int argc, char *argv[]) {
	
	for (int i = 0; i < 1; i++) {
		Test_Init();	
		Test_Run();
		Test_Shutdown();
	}
	
	int i = ASM_Test();
	printf("\n\nASM_Test is %d\n", i);


	while (1) {
		Sleep(50);
	}

	return 0;
}
