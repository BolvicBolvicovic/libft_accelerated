/* ************************************************************************** */
/*                                                                            */
/*                          BENCHMARK COMMON HEADER                          */
/*                                                                            */
/* ************************************************************************** */

#ifndef BENCHMARK_COMMON_H
# define BENCHMARK_COMMON_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

// Prevent compiler optimizations
extern volatile size_t result;

// Force cache eviction
void flush_cache_lines(void* ptr, size_t size);

typedef struct {
    char* aligned_str;      // 16-byte aligned
    char* unaligned_str;    // Deliberately unaligned
    size_t length;
    void* allocation;       // For freeing
} test_string;

// Test string creation and management
test_string create_test_strings(size_t len);
void free_test_strings(test_string* ts);
char* create_random_string(size_t len);

// Function declarations for individual benchmarks
void benchmark_strlen(void);
void benchmark_memset(void);
void benchmark_strlen_full(void);
void benchmark_memset_full(void);

#endif /* BENCHMARK_COMMON_H */
