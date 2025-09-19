#include "libft_old/libft.h"
// TODO: When printf is writen, remove stdio.
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

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

typedef struct {
    char* aligned_str;      // 16-byte aligned
    char* unaligned_str;    // Deliberately unaligned
    size_t length;
    void* allocation;       // For freeing
} test_string;

test_string create_test_strings(size_t len) {
    test_string ts;
    ts.length = len;
    
    // Allocate extra space for alignment manipulation
    size_t alloc_size = len + 32;
    ts.allocation = malloc(alloc_size);
    
    // Create aligned string (16-byte aligned)
    uintptr addr = (uintptr)ts.allocation;
    uintptr aligned_addr = (addr + 15) & ~15UL;  // Round up to 16-byte boundary
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

void benchmark_strlen_alignment_cache(const char* test_name, char* str, size_t len, int iterations, bool flush_cache) {
    unsigned long long system_cycles = 0;
    unsigned long long ft_cycles = 0;
    
    // Warm up (optional - only for cached tests)
    if (!flush_cache) {
        for (int i = 0; i < 10; i++) {
            result = strlen(str);
            result = ft_strlen(str);
        }
    }
    
    // Benchmark system strlen
    if (flush_cache) flush_cache_lines(str, len + 1);
    unsigned long long start = __rdtsc();
    for (int i = 0; i < iterations; i++) {
        if (flush_cache && i % 100 == 0) flush_cache_lines(str, len + 1);
        result = strlen(str);
    }
    unsigned long long end = __rdtsc();
    system_cycles = (end - start) / iterations;
    
    // Benchmark ft_strlen
    if (flush_cache) flush_cache_lines(str, len + 1);
    start = __rdtsc();
    for (int i = 0; i < iterations; i++) {
        if (flush_cache && i % 100 == 0) flush_cache_lines(str, len + 1);
        result = ft_strlen(str);
    }
    end = __rdtsc();
    ft_cycles = (end - start) / iterations;
    
    const char* alignment = ((uintptr)str & 15) == 0 ? "Aligned  " : "Unaligned";
    const char* cache_state = flush_cache ? "Cold" : "Hot ";
    
    printf("%-20s | %s | %s | System: %6llu | ft_strlen: %6llu | Ratio: %.2fx\n", 
           test_name, alignment, cache_state, system_cycles, ft_cycles, 
           (double)ft_cycles / system_cycles);
}

void benchmark_strlen_detailed(const char* size_name, test_string* ts) {
    printf("\n%s - Individual measurements (5 runs each):\n", size_name);
    
    // Test aligned, cached
    printf("Aligned + Cached   - System: ");
    for (int i = 0; i < 5; i++) {
        unsigned long long start = __rdtsc();
        result = strlen(ts->aligned_str);
        unsigned long long end = __rdtsc();
        printf("%6llu ", end - start);
    }
    printf("| ft_strlen: ");
    for (int i = 0; i < 5; i++) {
        unsigned long long start = __rdtsc();
        result = ft_strlen(ts->aligned_str);
        unsigned long long end = __rdtsc();
        printf("%6llu ", end - start);
    }
    printf("\n");
    
    // Test unaligned, cached
    printf("Unaligned + Cached - System: ");
    for (int i = 0; i < 5; i++) {
        unsigned long long start = __rdtsc();
        result = strlen(ts->unaligned_str);
        unsigned long long end = __rdtsc();
        printf("%6llu ", end - start);
    }
    printf("| ft_strlen: ");
    for (int i = 0; i < 5; i++) {
        unsigned long long start = __rdtsc();
        result = ft_strlen(ts->unaligned_str);
        unsigned long long end = __rdtsc();
        printf("%6llu ", end - start);
    }
    printf("\n");
    
    // Test aligned, cold cache
    printf("Aligned + Cold     - System: ");
    for (int i = 0; i < 5; i++) {
        flush_cache_lines(ts->aligned_str, ts->length + 1);
        unsigned long long start = __rdtsc();
        result = strlen(ts->aligned_str);
        unsigned long long end = __rdtsc();
        printf("%6llu ", end - start);
    }
    printf("| ft_strlen: ");
    for (int i = 0; i < 5; i++) {
        flush_cache_lines(ts->aligned_str, ts->length + 1);
        unsigned long long start = __rdtsc();
        result = ft_strlen(ts->aligned_str);
        unsigned long long end = __rdtsc();
        printf("%6llu ", end - start);
    }
    printf("\n");
}

void benchmark_strlen() {
    const int iterations = 50000;
    
    // Create test strings of different sizes with both aligned and unaligned versions
    test_string short_ts = create_test_strings(10);
    test_string mid_ts = create_test_strings(1000);
    test_string long_ts = create_test_strings(10000);
    test_string very_long_ts = create_test_strings(100000);
    
    printf("String Length Benchmark - Alignment & Cache Effects (averaged over %d iterations)\n", iterations);
    printf("====================================================================================\n");
    printf("%-20s | %-9s | %-4s | %-13s | %-13s | %s\n", 
           "Size", "Alignment", "Cache", "System (cyc)", "ft_strlen (cyc)", "Ratio");
    printf("====================================================================================\n");
    
    // Test all combinations: aligned/unaligned × cached/uncached
    benchmark_strlen_alignment_cache("Short (10 chars)", short_ts.aligned_str, short_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Short (10 chars)", short_ts.unaligned_str, short_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Short (10 chars)", short_ts.aligned_str, short_ts.length, iterations, true);
    benchmark_strlen_alignment_cache("Short (10 chars)", short_ts.unaligned_str, short_ts.length, iterations, true);
    
    printf("------------------------------------------------------------------------------------\n");
    
    benchmark_strlen_alignment_cache("Mid (1K chars)", mid_ts.aligned_str, mid_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Mid (1K chars)", mid_ts.unaligned_str, mid_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Mid (1K chars)", mid_ts.aligned_str, mid_ts.length, iterations, true);
    benchmark_strlen_alignment_cache("Mid (1K chars)", mid_ts.unaligned_str, mid_ts.length, iterations, true);
    
    printf("------------------------------------------------------------------------------------\n");
    
    benchmark_strlen_alignment_cache("Long (10K chars)", long_ts.aligned_str, long_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Long (10K chars)", long_ts.unaligned_str, long_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Long (10K chars)", long_ts.aligned_str, long_ts.length, iterations, true);
    benchmark_strlen_alignment_cache("Long (10K chars)", long_ts.unaligned_str, long_ts.length, iterations, true);
    
    printf("------------------------------------------------------------------------------------\n");
    
    benchmark_strlen_alignment_cache("Very Long (100K)", very_long_ts.aligned_str, very_long_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Very Long (100K)", very_long_ts.unaligned_str, very_long_ts.length, iterations, false);
    benchmark_strlen_alignment_cache("Very Long (100K)", very_long_ts.aligned_str, very_long_ts.length, iterations, true);
    benchmark_strlen_alignment_cache("Very Long (100K)", very_long_ts.unaligned_str, very_long_ts.length, iterations, true);
    
    // Show detailed individual measurements for key sizes
    printf("\n\nDetailed Individual Measurements:\n");
    printf("====================================\n");
    benchmark_strlen_detailed("Short (10 chars)", &short_ts);
    benchmark_strlen_detailed("Mid (1K chars)", &mid_ts);
    benchmark_strlen_detailed("Long (10K chars)", &long_ts);
    
    // Memory alignment analysis
    printf("\n\nMemory Alignment Analysis:\n");
    printf("==========================\n");
    printf("Short strings  - Aligned: %p (mod 16 = %lu), Unaligned: %p (mod 16 = %lu)\n", 
           (void*)short_ts.aligned_str, (uintptr)short_ts.aligned_str & 15,
           (void*)short_ts.unaligned_str, (uintptr)short_ts.unaligned_str & 15);
    printf("Mid strings    - Aligned: %p (mod 16 = %lu), Unaligned: %p (mod 16 = %lu)\n", 
           (void*)mid_ts.aligned_str, (uintptr)mid_ts.aligned_str & 15,
           (void*)mid_ts.unaligned_str, (uintptr)mid_ts.unaligned_str & 15);
    printf("Long strings   - Aligned: %p (mod 16 = %lu), Unaligned: %p (mod 16 = %lu)\n", 
           (void*)long_ts.aligned_str, (uintptr)long_ts.aligned_str & 15,
           (void*)long_ts.unaligned_str, (uintptr)long_ts.unaligned_str & 15);
    
    // Cleanup
    free_test_strings(&short_ts);
    free_test_strings(&mid_ts);
    free_test_strings(&long_ts);
    free_test_strings(&very_long_ts);
}

int main() {
	srand(time(NULL));
	printf("\n\nUNOPTIMIZED LIBFT\n\n");
	benchmark_strlen();
	return 0;
}
