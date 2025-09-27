/* ************************************************************************** */
/*                                                                            */
/*                          MEMCPY BENCHMARK			                      */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Configuration for improved benchmark
#define BUFLEN_SMALL    1024    // For small buffer tests
#define BUFLEN_MEDIUM   8192    // For medium buffer tests  
#define BUFLEN_LARGE    32768   // For large buffer tests
#define ITERATIONS      500     // Number of iterations per test

// Forward declarations for old libft functions (only available in full mode) 
#ifdef FULL_COMPARISON_MODE
void *ft_memcpy_old(void *dst, const void *src, size_t n);
#endif

// Improved memcpy benchmark with variable copy patterns
static size_t benchmark_memcpy_variable_pattern(void* (*memcpy_func)(void*, const void*, size_t), size_t buflen, const char* test_name) {
    char *src_buf = malloc(buflen);
    char *dst_buf = malloc(buflen);
    if (!src_buf || !dst_buf) {
        free(src_buf);
        free(dst_buf);
        return 0;
    }
    
    size_t i;
    size_t total_bytes = 0;
    
    // Initialize source buffer with varied content
    for (size_t j = 0; j < buflen; j++) {
        src_buf[j] = 'A' + (j % 26) + (j / 100);
    }
    
    // Vary source offset, destination offset, and copy size during iterations
    for (i = 0; i < ITERATIONS; i++) {
        // Variable source offset
        size_t src_offset = i % (buflen / 8);
        
        // Variable destination offset (with some overlap patterns)
        size_t dst_offset = (i * 3) % (buflen / 8);
        
        // Variable copy size
        size_t copy_size = (buflen / 2) - (i % 512);
        if (copy_size == 0) copy_size = 64;
        
        // Ensure we don't exceed buffer bounds
        if (src_offset + copy_size > buflen) src_offset = buflen - copy_size;
        if (dst_offset + copy_size > buflen) dst_offset = buflen - copy_size;
        
        // Perform the copy operation
        memcpy_func(dst_buf + dst_offset, src_buf + src_offset, copy_size);
        total_bytes += copy_size;
        
        // Occasionally modify source buffer to create different patterns
        if (i % 25 == 0) {
            size_t mod_pos = (src_offset + i) % buflen;
            src_buf[mod_pos] = '0' + (i % 10);
        }
        
        // Occasionally create overlapping copy scenario (memmove-like)
        if (i % 100 == 0 && copy_size < buflen / 4) {
            size_t overlap_offset = dst_offset + copy_size / 2;
            if (overlap_offset + copy_size / 2 < buflen) {
                memcpy_func(dst_buf + overlap_offset, dst_buf + dst_offset, copy_size / 2);
                total_bytes += copy_size / 2;
            }
        }
    }
    
    // Calculate simple checksum to prevent optimization
    size_t checksum = 0;
    for (i = 0; i < buflen; i++) {
        checksum += (unsigned char)dst_buf[i];
    }
    
    free(src_buf);
    free(dst_buf);
    return checksum + total_bytes;
}
    
    // Benchmark with aligned/unaligned buffer addresses  
static void benchmark_memcpy_alignment(size_t buflen, const char* size_desc, bool include_old) {
    unsigned long long start, end;
    size_t cs;
    
    printf("%-20s Tests:\n", size_desc);
    printf("  Function         | Alignment | Cycles/Call | Checksum      | MB/s    | Relative Performance\n");
    printf("  -----------------------------------------------------------------------------------------\n");
    
    // System memcpy - aligned
    start = __rdtsc();
    cs = benchmark_memcpy_variable_pattern(memcpy, buflen, "system");
    end = __rdtsc();
    unsigned long long system_cycles = (end - start) / ITERATIONS;
    double system_mbps = (double)(buflen * ITERATIONS / 2) / (1024 * 1024) * 2400.0 / system_cycles; // Approximate MB/s
    printf("  %-15s | %-9s | %11llu | %13zu | %7.1f | %s\n", 
           "system memcpy", "aligned", system_cycles, cs, system_mbps, "baseline");
    
    // ft_memcpy - aligned  
    start = __rdtsc();
    cs = benchmark_memcpy_variable_pattern((void* (*)(void*, const void*, size_t))ft_memcpy, buflen, "optimized");
    end = __rdtsc();
    unsigned long long ft_cycles = (end - start) / ITERATIONS;
    double ft_ratio = (double)ft_cycles / system_cycles;
    double ft_mbps = (double)(buflen * ITERATIONS / 2) / (1024 * 1024) * 2400.0 / ft_cycles;
    printf("  %-15s | %-9s | %11llu | %13zu | %7.1f | %.2fx %s\n", 
           "ft_memcpy", "aligned", ft_cycles, cs, ft_mbps, ft_ratio,
           ft_ratio < 1.0 ? "(faster)" : "(slower)");
    
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        start = __rdtsc();
        cs = benchmark_memcpy_variable_pattern((void* (*)(void*, const void*, size_t))ft_memcpy_old, buflen, "old");
        end = __rdtsc();
        unsigned long long ft_old_cycles = (end - start) / ITERATIONS;
        double old_ratio = (double)ft_old_cycles / system_cycles;
        double old_mbps = (double)(buflen * ITERATIONS / 2) / (1024 * 1024) * 2400.0 / ft_old_cycles;
        printf("  %-15s | %-9s | %11llu | %13zu | %7.1f | %.2fx %s\n", 
               "ft_memcpy_old", "aligned", ft_old_cycles, cs, old_mbps, old_ratio,
               old_ratio < 1.0 ? "(faster)" : "(slower)");
    }
#endif

    // Test unaligned access patterns
    char *src_unaligned = malloc(buflen + 32);
    char *dst_unaligned = malloc(buflen + 32);
    if (src_unaligned && dst_unaligned) {
        // Create unaligned buffers
        char *src_offset = src_unaligned + 7;
        char *dst_offset = dst_unaligned + 11; // Different offset for more realistic test
        
        // Initialize source
        for (size_t j = 0; j < buflen; j++) {
            src_offset[j] = 'A' + (j % 26);
        }
        
        // Quick unaligned test for ft_memcpy only
        start = __rdtsc();
        size_t unaligned_total = 0;
        
        for (size_t i = 0; i < ITERATIONS; i++) {
            size_t copy_offset = i % (buflen / 16);
            size_t copy_size = (buflen / 4) - (i % 128);
            if (copy_size == 0) copy_size = 64;
            if (copy_offset + copy_size > buflen) copy_offset = buflen - copy_size;
            
            ft_memcpy(dst_offset + copy_offset, src_offset + copy_offset, copy_size);
            unaligned_total += copy_size;
        }
        
        // Calculate checksum
        size_t unaligned_cs = 0;
        for (size_t i = 0; i < buflen; i++) {
            unaligned_cs += (unsigned char)dst_offset[i];
        }
        
        end = __rdtsc();
        unsigned long long unaligned_cycles = (end - start) / ITERATIONS;
        double unaligned_ratio = (double)unaligned_cycles / system_cycles;
        double unaligned_mbps = (double)unaligned_total / (1024 * 1024) * 2400.0 / unaligned_cycles;
        
        printf("  %-15s | %-9s | %11llu | %13zu | %7.1f | %.2fx %s\n", 
               "ft_memcpy", "unaligned", unaligned_cycles, unaligned_cs + unaligned_total, unaligned_mbps, unaligned_ratio,
               unaligned_ratio < 1.0 ? "(faster)" : "(slower)");
        
        free(src_unaligned);
        free(dst_unaligned);
    }
    
    printf("\n");
}

void benchmark_memcpy_comparison(bool include_old) {
    printf("=== IMPROVED MEMCPY BENCHMARK ===\n");
    printf("Using variable offset/size copy technique with %d iterations per test\n\n", ITERATIONS);
    
    // Test different buffer sizes
    benchmark_memcpy_alignment(BUFLEN_SMALL, "Small (1KB)", include_old);
    benchmark_memcpy_alignment(BUFLEN_MEDIUM, "Medium (8KB)", include_old);
    benchmark_memcpy_alignment(BUFLEN_LARGE, "Large (32KB)", include_old);
    
    printf("=== SUMMARY ===\n");
    printf("This benchmark uses varying source/destination offsets and copy sizes\n");
    printf("to create realistic memory copy patterns with different alignments.\n");
    printf("MB/s shows approximate throughput (assumes 2.4 GHz CPU).\n");
    printf("Checksum includes both destination content and total bytes copied.\n");
}

// Wrapper functions for compatibility
void benchmark_memcpy(void) {
    benchmark_memcpy_comparison(false);
}

void benchmark_memcpy_full(void) {
    benchmark_memcpy_comparison(true);
}
