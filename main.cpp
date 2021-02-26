#include <stdint.h>
#include <cstddef>
#include <malloc.h>

void* Mem_Alloc(uint64_t size) {
	return _aligned_malloc(size, 16);
}

void Mem_Free(void *ptr) {
	return _aligned_free(ptr);
}

namespace {
	uint64_t buffer_size;
	std::byte *buffer_1;
	std::byte *buffer_2;
}

void Test_Init() {

}

void Test_Run() {

}

void Test_Shutdown() {

}

int main() {
	
	Test_Init();	

	Test_Run();

	Test_Shutdown();
	
	return 0;
}