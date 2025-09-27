/* ************************************************************************** */
/*                                                                            */
/*                          STRLEN COMPARISON BENCHMARK                      */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Configuration for improved benchmark
#define BUFLEN_SMALL    64      // For short string tests
#define BUFLEN_MEDIUM   1024    // For medium string tests  
#define BUFLEN_LARGE    8192    // For long string tests
#define ITERATIONS      1000    // Number of iterations per test

// Forward declarations for old libft functions (only available in full mode)
#ifdef FULL_COMPARISON_MODE
size_t ft_strlen_old(const char *str);
#endif

// Improved strlen benchmark based on variable-content technique
static size_t benchmark_strlen_variable_content(size_t (*strlen_func)(const char*), size_t buflen, const char* test_name) {
    char *buf = malloc(buflen);
    if (!buf) return 0;
    
    size_t i;
    size_t cs = 0;
    
    // Initialize buffer with 'a' characters, null-terminate
    memset(buf, 'a', buflen - 1);
    buf[buflen - 1] = '\0';
    
    // Vary string content and length during iterations
    for (i = 0; i < ITERATIONS; i++) {
        // Change some characters to create different string patterns
        // This prevents CPU from optimizing for a fixed pattern
        size_t pos1 = i % (buflen - 1);
        size_t pos2 = (i * 7) % (buflen - 1);
        size_t pos3 = (i * 13) % (buflen - 1);
        
        buf[pos1] = '0' + (i % 10);
        buf[pos2] = 'A' + (i % 26);
        buf[pos3] = 'x' + (i % 5);
        
        // Also vary the effective string length occasionally
        if (i % 50 == 0 && i > 0) {
            size_t temp_end = (buflen - 1) * (i % 4 + 1) / 4;
            char saved = buf[temp_end];
            buf[temp_end] = '\0';
            cs += strlen_func(buf);
            buf[temp_end] = saved;
        } else {
            cs += strlen_func(buf);
        }
    }
    
    free(buf);
    return cs;
}

// Benchmark with aligned/unaligned buffer addresses
static void benchmark_strlen_alignment(size_t buflen, const char* size_desc, bool include_old) {
    unsigned long long start, end;
    size_t cs;
    
    printf("%-20s Tests:\n", size_desc);
    printf("  Function         | Alignment | Cycles/Call | Checksum    | Relative Performance\n");
    printf("  -----------------------------------------------------------------------------\n");
    
    // System strlen - aligned
    start = __rdtsc();
    cs = benchmark_strlen_variable_content(strlen, buflen, "system");
    end = __rdtsc();
    unsigned long long system_cycles = (end - start) / ITERATIONS;
    printf("  %-15s | %-9s | %11llu | %11zu | %s\n", 
           "system strlen", "aligned", system_cycles, cs, "baseline");
    
    // ft_strlen - aligned  
    start = __rdtsc();
    cs = benchmark_strlen_variable_content(ft_strlen, buflen, "optimized");
    end = __rdtsc();
    unsigned long long ft_cycles = (end - start) / ITERATIONS;
    double ft_ratio = (double)ft_cycles / system_cycles;
    printf("  %-15s | %-9s | %11llu | %11zu | %.2fx %s\n", 
           "ft_strlen", "aligned", ft_cycles, cs, ft_ratio,
           ft_ratio < 1.0 ? "(faster)" : "(slower)");
    
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        start = __rdtsc();
        cs = benchmark_strlen_variable_content((size_t (*)(const char*))ft_strlen_old, buflen, "old");
        end = __rdtsc();
        unsigned long long ft_old_cycles = (end - start) / ITERATIONS;
        double old_ratio = (double)ft_old_cycles / system_cycles;
        printf("  %-15s | %-9s | %11llu | %11zu | %.2fx %s\n", 
               "ft_strlen_old", "aligned", ft_old_cycles, cs, old_ratio,
               old_ratio < 1.0 ? "(faster)" : "(slower)");
    }
#endif

    // Test unaligned access patterns
    char *unaligned_buf = malloc(buflen + 16);
    if (unaligned_buf) {
        // Offset by 7 bytes to create unaligned access
        char *offset_buf = unaligned_buf + 7;
        
        memset(offset_buf, 'a', buflen - 1);
        offset_buf[buflen - 1] = '\0';
        
        // Quick unaligned test for ft_strlen only (most interesting case)
        start = __rdtsc();
        size_t unaligned_cs = 0;
        for (size_t i = 0; i < ITERATIONS; i++) {
            size_t pos = i % (buflen - 1);
            offset_buf[pos] = '0' + (i % 10);
            unaligned_cs += ft_strlen(offset_buf);
        }
        end = __rdtsc();
        unsigned long long unaligned_cycles = (end - start) / ITERATIONS;
        double unaligned_ratio = (double)unaligned_cycles / system_cycles;
        
        printf("  %-15s | %-9s | %11llu | %11zu | %.2fx %s\n", 
               "ft_strlen", "unaligned", unaligned_cycles, unaligned_cs, unaligned_ratio,
               unaligned_ratio < 1.0 ? "(faster)" : "(slower)");
        
        free(unaligned_buf);
    }
    
    printf("\n");
}
void benchmark_strlen_comparison(bool include_old) {
    printf("=== IMPROVED STRLEN BENCHMARK ===\n");
    printf("Using variable content technique with %d iterations per test\n\n", ITERATIONS);
    
    // Test different buffer sizes
    benchmark_strlen_alignment(BUFLEN_SMALL, "Small (64 bytes)", include_old);
    benchmark_strlen_alignment(BUFLEN_MEDIUM, "Medium (1KB)", include_old);
    benchmark_strlen_alignment(BUFLEN_LARGE, "Large (8KB)", include_old);
    
    printf("=== SUMMARY ===\n");
    printf("This benchmark uses varying string content to prevent CPU optimizations\n");
    printf("and provides more realistic performance measurements.\n");
    printf("Checksum values should be identical for correct implementations.\n");
}

// Wrapper functions for compatibility
void benchmark_strlen(void) {
    benchmark_strlen_comparison(false);
}

void benchmark_strlen_full(void) {
    benchmark_strlen_comparison(true);
}
