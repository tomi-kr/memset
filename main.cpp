#include <cstddef>

#include "memset.h"

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

}

void Test_Shutdown() {
	Mem_Free(buffer_2);
	Mem_Free(buffer_1);
}

int main(int argc, char *argv[]) {
	
	Test_Init();	

	Test_Run();

	Test_Shutdown();
	
	return 0;
}
