/* ************************************************************************** */
/*                                                                            */
/*                          BENCHMARK COMMON UTILITIES                       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark_common.h"

// Prevent compiler optimizations
volatile size_t result;

// Force cache eviction
void flush_cache_lines(void* ptr, size_t size) {
    const size_t cache_line_size = 64;
    char* p = (char*)ptr;
    for (size_t i = 0; i < size; i += cache_line_size) {
        __builtin_ia32_clflush(p + i);
    }
    __builtin_ia32_mfence();
}

test_string create_test_strings(size_t len) {
    test_string ts;
    ts.length = len;
    
    // Allocate extra space for alignment manipulation
    size_t alloc_size = len + 32;
    ts.allocation = malloc(alloc_size);
    
    // Create aligned string (16-byte aligned)
    uintptr_t addr = (uintptr_t)ts.allocation;
    uintptr_t aligned_addr = (addr + 15) & ~15UL;  // Round up to 16-byte boundary
    ts.aligned_str = (char*)aligned_addr;
    
    // Create unaligned string (offset by 1-15 bytes from alignment)
    size_t offset = 7;  // Choose 7 to be maximally unaligned for both 8 and 16 byte boundaries
    ts.unaligned_str = (char*)(aligned_addr + offset);
    
    // Fill both strings with random data
    for (size_t i = 0; i < len; i++) {
        char c = 'A' + (rand() % 26);  // Random letters
        ts.aligned_str[i] = c;
        ts.unaligned_str[i] = c;
    }
    ts.aligned_str[len] = '\0';
    ts.unaligned_str[len] = '\0';
    
    return ts;
}

void free_test_strings(test_string* ts) {
    free(ts->allocation);
}

char* create_random_string(size_t len) {
    char* str = malloc(len + 1);
    for (size_t i = 0; i < len; i++) {
        str[i] = 'A' + (rand() % 26);  // Random letters
    }
    str[len] = '\0';
    return str;
}
