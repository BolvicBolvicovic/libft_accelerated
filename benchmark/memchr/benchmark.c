/* ************************************************************************** */
/*                                                                            */
/*                          MEMCHR COMPARISON BENCHMARK                      */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Configuration for improved benchmark
#define BUFLEN_SMALL    512     // For small buffer tests
#define BUFLEN_MEDIUM   4096    // For medium buffer tests  
#define BUFLEN_LARGE    16384   // For large buffer tests
#define ITERATIONS      1000    // Number of iterations per test

// Forward declarations for old libft functions (only available in full mode) 
#ifdef FULL_COMPARISON_MODE
void *ft_memchr_old(const void *s, int c, size_t n);
#endif

// Improved memchr benchmark with variable search patterns
static size_t benchmark_memchr_variable_pattern(void* (*memchr_func)(const void*, int, size_t), size_t buflen, const char* test_name) {
    char *buf = malloc(buflen);
    if (!buf) return 0;
    
    size_t i;
    size_t found_count = 0;
    size_t total_offset = 0;
    
    // Initialize buffer with varied content
    for (size_t j = 0; j < buflen; j++) {
        buf[j] = 'a' + (j % 26);  // Fill with alphabet pattern
    }
    
    // Vary search character, start offset, and search length during iterations
    for (i = 0; i < ITERATIONS; i++) {
        // Variable search character - cycle through different chars
        int search_char = 'a' + (i % 26);
        
        // Variable start offset
        size_t offset = i % (buflen / 8);
        
        // Variable search length
        size_t search_len = buflen - offset - (i % 128);
        if (search_len == 0) search_len = 1;
        
        // Place the search character at different positions to vary hit/miss patterns
        if (i % 3 == 0) {
            // Place search char near the beginning (early hit)
            size_t pos = offset + (i % 16);
            if (pos < buflen) buf[pos] = search_char;
        } else if (i % 3 == 1) {
            // Place search char near the end (late hit)
            size_t pos = offset + search_len - 1 - (i % 16);
            if (pos < buflen && pos >= offset) buf[pos] = search_char;
        }
        // else: no guaranteed hit (miss case)
        
        // Perform the search
        void *res = memchr_func(buf + offset, search_char, search_len);
        if (res) {
            found_count++;
            total_offset += (char*)res - (buf + offset);
        }
        
        // Occasionally modify buffer content to prevent caching
        if (i % 50 == 0) {
            size_t mod_pos = (offset + i) % buflen;
            buf[mod_pos] = 'x' + (i % 3);
        }
    }
    
    free(buf);
    return found_count * 1000 + total_offset; // Combine metrics for checksum
}

// Benchmark with aligned/unaligned buffer addresses
static void benchmark_memchr_alignment(size_t buflen, const char* size_desc, bool include_old) {
    unsigned long long start, end;
    size_t cs;
    
    printf("%-20s Tests:\n", size_desc);
    printf("  Function         | Alignment | Cycles/Call | Checksum    | Hit Rate %% | Relative Performance\n");
    printf("  --------------------------------------------------------------------------------------\n");
    
    // System memchr - aligned
    start = __rdtsc();
    cs = benchmark_memchr_variable_pattern(memchr, buflen, "system");
    end = __rdtsc();
    unsigned long long system_cycles = (end - start) / ITERATIONS;
    size_t hit_rate = cs / 1000; // Extract hit count from combined checksum
    printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %s\n", 
           "system memchr", "aligned", system_cycles, cs, (hit_rate * 100.0) / ITERATIONS, "baseline");
    
    // ft_memchr - aligned  
    start = __rdtsc();
    cs = benchmark_memchr_variable_pattern((void* (*)(const void*, int, size_t))ft_memchr, buflen, "optimized");
    end = __rdtsc();
    unsigned long long ft_cycles = (end - start) / ITERATIONS;
    double ft_ratio = (double)ft_cycles / system_cycles;
    hit_rate = cs / 1000;
    printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %.2fx %s\n", 
           "ft_memchr", "aligned", ft_cycles, cs, (hit_rate * 100.0) / ITERATIONS, ft_ratio,
           ft_ratio < 1.0 ? "(faster)" : "(slower)");
    
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        start = __rdtsc();
        cs = benchmark_memchr_variable_pattern((void* (*)(const void*, int, size_t))ft_memchr_old, buflen, "old");
        end = __rdtsc();
        unsigned long long ft_old_cycles = (end - start) / ITERATIONS;
        double old_ratio = (double)ft_old_cycles / system_cycles;
        hit_rate = cs / 1000;
        printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %.2fx %s\n", 
               "ft_memchr_old", "aligned", ft_old_cycles, cs, (hit_rate * 100.0) / ITERATIONS, old_ratio,
               old_ratio < 1.0 ? "(faster)" : "(slower)");
    }
#endif

    // Test unaligned access patterns
    char *unaligned_buf = malloc(buflen + 16);
    if (unaligned_buf) {
        // Offset by 7 bytes to create unaligned access
        char *offset_buf = unaligned_buf + 7;
        
        // Initialize with pattern
        for (size_t j = 0; j < buflen; j++) {
            offset_buf[j] = 'a' + (j % 26);
        }
        
        // Quick unaligned test for ft_memchr only
        start = __rdtsc();
        size_t unaligned_found = 0;
        size_t unaligned_offset_sum = 0;
        
        for (size_t i = 0; i < ITERATIONS; i++) {
            int search_char = 'a' + (i % 26);
            size_t offset = i % (buflen / 8);
            size_t search_len = buflen - offset - (i % 128);
            if (search_len == 0) search_len = 1;
            
            // Ensure some hits
            if (i % 3 == 0) {
                size_t pos = offset + (i % 16);
                if (pos < buflen) offset_buf[pos] = search_char;
            }
            
            void *res = ft_memchr(offset_buf + offset, search_char, search_len);
            if (res) {
                unaligned_found++;
                unaligned_offset_sum += (char*)res - (offset_buf + offset);
            }
        }
        
        end = __rdtsc();
        unsigned long long unaligned_cycles = (end - start) / ITERATIONS;
        double unaligned_ratio = (double)unaligned_cycles / system_cycles;
        size_t unaligned_cs = unaligned_found * 1000 + unaligned_offset_sum;
        
        printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %.2fx %s\n", 
               "ft_memchr", "unaligned", unaligned_cycles, unaligned_cs, (unaligned_found * 100.0) / ITERATIONS, unaligned_ratio,
               unaligned_ratio < 1.0 ? "(faster)" : "(slower)");
        
        free(unaligned_buf);
    }
    
    printf("\n");
}

void benchmark_memchr_comparison(bool include_old) {
    printf("=== IMPROVED MEMCHR BENCHMARK ===\n");
    printf("Using variable search pattern technique with %d iterations per test\n\n", ITERATIONS);
    
    // Test different buffer sizes
    benchmark_memchr_alignment(BUFLEN_SMALL, "Small (512 bytes)", include_old);
    benchmark_memchr_alignment(BUFLEN_MEDIUM, "Medium (4KB)", include_old);
    benchmark_memchr_alignment(BUFLEN_LARGE, "Large (16KB)", include_old);
    
    printf("=== SUMMARY ===\n");
    printf("This benchmark varies search characters, offsets, and lengths to create\n");
    printf("realistic search patterns with mix of early hits, late hits, and misses.\n");
    printf("Hit Rate shows percentage of searches that found the target character.\n");
    printf("Checksum combines hit count and offset positions for verification.\n");
}

// Wrapper functions for compatibility
void benchmark_memchr(void) {
    benchmark_memchr_comparison(false);
}

void benchmark_memchr_full(void) {
    benchmark_memchr_comparison(true);
}