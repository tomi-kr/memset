#include <cstddef>
#include <stdio.h>
#include <memory>
#include <string_view>
#include "memset.h"
#include "stopwatch.h"

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

void Test_Init() {
	static_assert(__STDCPP_DEFAULT_NEW_ALIGNMENT__ == 16, "This is not going to work. Consider using Mem_Alloc/Mem_Free.");

	buffer_size = uint64_t(1024) * 1024 * 512;
	buffer_1 = new std::byte[buffer_size];
	buffer_2 = new std::byte[buffer_size];
	
	AssertRelease(IsAligned(buffer_1, 16) == true, "Buffer1 is not aligned!");
	AssertRelease(IsAligned(buffer_2, 16) == true, "Buffer2 is not aligned!");
}

void Test_Run() {
	std::memset(buffer_1, 9099, buffer_size);
	std::memset(buffer_2, 9099, buffer_size);

	double start1 = StopWatch::MSec();
	std::memset(buffer_1, 1, buffer_size);
	double end1 = StopWatch::MSec();

	double start2 = StopWatch::MSec();
	Memset_SIMD(buffer_2, 1, buffer_size);
	double end2 = StopWatch::MSec();

	if (std::memcmp(buffer_1, buffer_2, buffer_size) != 0) {
		for (int i = 0; i < 10; i++) {
			printf("********** Memset_SIMD is broken **********\n");
		}
	}

	// reset
	std::memset(buffer_2, 9099, buffer_size);

	double start3 = StopWatch::MSec();
	Memset_Manual(buffer_2, 1, buffer_size);
	double end3 = StopWatch::MSec();

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
	delete [] buffer_1;
	delete [] buffer_2;	
}

int main(int argc, char *argv[]) {

	Test_Init();
	
	for (int i = 0; i < 10; i++) {		
		Test_Run();		
	}
	
	Test_Shutdown();
	
	int i = ASM_Test();
	printf("\n\nASM_Test is %d\n", i);

	while (1) {
		Sleep(50);
	}

	return 0;
}
