/* ************************************************************************** */
/*                                                                            */
/*                          MEMSET COMPARISON BENCHMARK                      */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Configuration for improved benchmark
#define BUFLEN_SMALL    256     // For small buffer tests
#define BUFLEN_MEDIUM   4096    // For medium buffer tests  
#define BUFLEN_LARGE    16384   // For large buffer tests
#define ITERATIONS      1000    // Number of iterations per test

// Forward declarations for old libft functions (only available in full mode) 
#ifdef FULL_COMPARISON_MODE
void *ft_memset_old(void *s, int c, size_t n);
#endif

// Improved memset benchmark based on variable-offset technique
static size_t benchmark_memset_variable_pattern(void* (*memset_func)(void*, int, size_t), size_t buflen, const char* test_name) {
    char *buf = malloc(buflen);
    if (!buf) return 0;
    
    size_t i;
    size_t operations = 0;
    
    // Clear initial buffer
    memset(buf, 0, buflen);
    
    // Vary offset, value, and size during iterations to create realistic patterns
    for (i = 0; i < ITERATIONS; i++) {
        // Variable offset: start from different positions
        size_t offset = i % (buflen / 4);  // Use quarter of buffer for offset variation
        
        // Variable size: set different amounts of memory
        size_t size = buflen - offset - (i % 64);  // Vary size, ensuring we stay in bounds
        if (size == 0) size = 1;  // Ensure minimum size
        
        // Variable value: use different byte values
        int value = (i % 256);  // Cycle through all possible byte values
        
        // Perform the memset operation
        memset_func(buf + offset, value, size);
        operations++;
        
        // Occasionally do overlapping patterns (realistic for some use cases)
        if (i % 20 == 0 && i > 0) {
            size_t overlap_offset = offset / 2;
            size_t overlap_size = size / 2;
            if (overlap_size > 0) {
                memset_func(buf + overlap_offset, value ^ 0xFF, overlap_size);
                operations++;
            }
        }
    }
    
    // Calculate simple checksum to prevent optimization and verify correctness
    size_t checksum = 0;
    for (i = 0; i < buflen; i++) {
        checksum += (unsigned char)buf[i];
    }
    
    free(buf);
    return checksum;
}

// Benchmark with aligned/unaligned buffer addresses
static void benchmark_memset_alignment(size_t buflen, const char* size_desc, bool include_old) {
    unsigned long long start, end;
    size_t cs;
    
    printf("%-20s Tests:\n", size_desc);
    printf("  Function         | Alignment | Cycles/Call | Checksum    | Operations | Relative Performance\n");
    printf("  ---------------------------------------------------------------------------------------\n");
    
    // System memset - aligned
    start = __rdtsc();
    cs = benchmark_memset_variable_pattern(memset, buflen, "system");
    end = __rdtsc();
    unsigned long long system_cycles = (end - start) / (ITERATIONS + ITERATIONS/50); // Account for extra operations
    printf("  %-15s | %-9s | %11llu | %11zu | %10d | %s\n", 
           "system memset", "aligned", system_cycles, cs, ITERATIONS + ITERATIONS/50, "baseline");
    
    // ft_memset - aligned  
    start = __rdtsc();
    cs = benchmark_memset_variable_pattern((void* (*)(void*, int, size_t))ft_memset, buflen, "optimized");
    end = __rdtsc();
    unsigned long long ft_cycles = (end - start) / (ITERATIONS + ITERATIONS/50);
    double ft_ratio = (double)ft_cycles / system_cycles;
    printf("  %-15s | %-9s | %11llu | %11zu | %10d | %.2fx %s\n", 
           "ft_memset", "aligned", ft_cycles, cs, ITERATIONS + ITERATIONS/50, ft_ratio,
           ft_ratio < 1.0 ? "(faster)" : "(slower)");
    
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        start = __rdtsc();
        cs = benchmark_memset_variable_pattern((void* (*)(void*, int, size_t))ft_memset_old, buflen, "old");
        end = __rdtsc();
        unsigned long long ft_old_cycles = (end - start) / (ITERATIONS + ITERATIONS/50);
        double old_ratio = (double)ft_old_cycles / system_cycles;
        printf("  %-15s | %-9s | %11llu | %11zu | %10d | %.2fx %s\n", 
               "ft_memset_old", "aligned", ft_old_cycles, cs, ITERATIONS + ITERATIONS/50, old_ratio,
               old_ratio < 1.0 ? "(faster)" : "(slower)");
    }
#endif

    // Test unaligned access patterns
    char *unaligned_buf = malloc(buflen + 16);
    if (unaligned_buf) {
        // Offset by 7 bytes to create unaligned access
        char *offset_buf = unaligned_buf + 7;
        
        // Quick unaligned test for ft_memset only (most interesting case)
        start = __rdtsc();
        size_t unaligned_cs = 0;
        size_t unaligned_ops = 0;
        
        for (size_t i = 0; i < ITERATIONS; i++) {
            size_t offset = i % (buflen / 4);
            size_t size = buflen - offset - (i % 64);
            if (size == 0) size = 1;
            int value = i % 256;
            
            ft_memset(offset_buf + offset, value, size);
            unaligned_ops++;
        }
        
        // Calculate checksum
        for (size_t i = 0; i < buflen; i++) {
            unaligned_cs += (unsigned char)offset_buf[i];
        }
        
        end = __rdtsc();
        unsigned long long unaligned_cycles = (end - start) / ITERATIONS;
        double unaligned_ratio = (double)unaligned_cycles / system_cycles;
        
        printf("  %-15s | %-9s | %11llu | %11zu | %10zu | %.2fx %s\n", 
               "ft_memset", "unaligned", unaligned_cycles, unaligned_cs, unaligned_ops, unaligned_ratio,
               unaligned_ratio < 1.0 ? "(faster)" : "(slower)");
        
        free(unaligned_buf);
    }
    
    printf("\n");
}
void benchmark_memset_comparison(bool include_old) {
    printf("=== IMPROVED MEMSET BENCHMARK ===\n");
    printf("Using variable offset/size/value technique with %d iterations per test\n\n", ITERATIONS);
    
    // Test different buffer sizes
    benchmark_memset_alignment(BUFLEN_SMALL, "Small (256 bytes)", include_old);
    benchmark_memset_alignment(BUFLEN_MEDIUM, "Medium (4KB)", include_old);
    benchmark_memset_alignment(BUFLEN_LARGE, "Large (16KB)", include_old);
    
    printf("=== SUMMARY ===\n");
    printf("This benchmark uses varying offsets, sizes, and values to prevent CPU optimizations\n");
    printf("and provides more realistic performance measurements for memset operations.\n");
    printf("Checksum values should be identical for correct implementations.\n");
    printf("Operations count includes occasional overlapping writes (realistic pattern).\n");
}

// Wrapper functions for compatibility
void benchmark_memset(void) {
    benchmark_memset_comparison(false);
}

void benchmark_memset_full(void) {
    benchmark_memset_comparison(true);
}
