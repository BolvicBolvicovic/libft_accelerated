/* ************************************************************************** */
/*                                                                            */
/*                          MEMSET COMPARISON BENCHMARK                      */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Forward declarations for old libft functions (only available in full mode) 
#ifdef FULL_COMPARISON_MODE
void *ft_memset_old(void *s, int c, int n);
#endif

static void benchmark_memset_comparison_cache(const char* test_name, void* ptr, size_t len, int value, int iterations, bool flush_cache, bool include_old) {
    unsigned long long system_cycles = 0;
    unsigned long long ft_cycles = 0;
    unsigned long long ft_old_cycles = 0;
    
    // Warm up (optional - only for cached tests)
    if (!flush_cache) {
        for (int i = 0; i < 10; i++) {
            memset(ptr, value, len);
            ft_memset(ptr, value, len);
#ifdef FULL_COMPARISON_MODE
            if (include_old) ft_memset_old(ptr, value, len);
#endif
        }
    }
    
    // Benchmark system memset
    if (flush_cache) flush_cache_lines(ptr, len);
    unsigned long long start = __rdtsc();
    for (int i = 0; i < iterations; i++) {
        if (flush_cache && i % 100 == 0) flush_cache_lines(ptr, len);
        memset(ptr, value, len);
    }
    unsigned long long end = __rdtsc();
    system_cycles = (end - start) / iterations;
    
    // Benchmark ft_memset (optimized)
    if (flush_cache) flush_cache_lines(ptr, len);
    start = __rdtsc();
    for (int i = 0; i < iterations; i++) {
        if (flush_cache && i % 100 == 0) flush_cache_lines(ptr, len);
        ft_memset(ptr, value, len);
    }
    end = __rdtsc();
    ft_cycles = (end - start) / iterations;
    
    // Benchmark ft_memset_old if requested
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        if (flush_cache) flush_cache_lines(ptr, len);
        start = __rdtsc();
        for (int i = 0; i < iterations; i++) {
            if (flush_cache && i % 100 == 0) flush_cache_lines(ptr, len);
            ft_memset_old(ptr, value, len);
        }
        end = __rdtsc();
        ft_old_cycles = (end - start) / iterations;
    }
#endif
    
    // Calculate ratios
    double opt_ratio = (double)ft_cycles / system_cycles;
    double old_ratio = include_old ? (double)ft_old_cycles / system_cycles : 0.0;
    
    // Print results with alignment info
    const char* alignment = ((uintptr_t)ptr & 15) == 0 ? "Aligned" : "Unaligned";
    const char* cache_state = flush_cache ? "Cold" : "Hot";
    
    if (include_old) {
        printf("%-20s | %-9s | %-5s | %13llu | %15llu | %13llu | %9.2fx | %.2fx\n", 
               test_name, alignment, cache_state, system_cycles, ft_cycles, ft_old_cycles, opt_ratio, old_ratio);
    } else {
        printf("%-20s | %-9s | %-5s | %13llu | %15llu | %.2fx\n", 
               test_name, alignment, cache_state, system_cycles, ft_cycles, opt_ratio);
    }
}

void benchmark_memset_comparison(bool include_old) {
    const int iterations = 50000;
    const int test_value = 0x42;  // Test value to set
    
    // Create test buffers of different sizes with both aligned and unaligned versions
    test_string short_ts = create_test_strings(10);
    test_string mid_ts = create_test_strings(1000);
    test_string long_ts = create_test_strings(10000);
    test_string very_long_ts = create_test_strings(100000);
    
    if (include_old) {
        printf("Memory Set Benchmark - System vs Optimized vs Old (averaged over %d iterations)\n", iterations);
        printf("==============================================================================================\n");
        printf("%-20s | %-9s | %-4s | %-13s | %-15s | %-13s | %-9s | %s\n", 
               "Size", "Alignment", "Cache", "System (cyc)", "Optimized (cyc)", "Old (cyc)", "Opt Ratio", "Old Ratio");
        printf("==============================================================================================\n");
    } else {
        printf("Memory Set Benchmark - System vs Optimized (averaged over %d iterations)\n", iterations);
        printf("==================================================================================\n");
        printf("%-20s | %-9s | %-4s | %-13s | %-15s | %s\n", 
               "Size", "Alignment", "Cache", "System (cyc)", "Optimized (cyc)", "Opt Ratio");
        printf("==================================================================================\n");
    }
    
    // Test all combinations: aligned/unaligned × cached/uncached
    benchmark_memset_comparison_cache("Short (10 bytes)", short_ts.aligned_str, short_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Short (10 bytes)", short_ts.unaligned_str, short_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Short (10 bytes)", short_ts.aligned_str, short_ts.length, test_value, iterations, true, include_old);
    benchmark_memset_comparison_cache("Short (10 bytes)", short_ts.unaligned_str, short_ts.length, test_value, iterations, true, include_old);
    
    printf("----------------------------------------------------------------------------------\n");
    
    benchmark_memset_comparison_cache("Mid (1K bytes)", mid_ts.aligned_str, mid_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Mid (1K bytes)", mid_ts.unaligned_str, mid_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Mid (1K bytes)", mid_ts.aligned_str, mid_ts.length, test_value, iterations, true, include_old);
    benchmark_memset_comparison_cache("Mid (1K bytes)", mid_ts.unaligned_str, mid_ts.length, test_value, iterations, true, include_old);
    
    printf("----------------------------------------------------------------------------------\n");
    
    benchmark_memset_comparison_cache("Long (10K bytes)", long_ts.aligned_str, long_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Long (10K bytes)", long_ts.unaligned_str, long_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Long (10K bytes)", long_ts.aligned_str, long_ts.length, test_value, iterations, true, include_old);
    benchmark_memset_comparison_cache("Long (10K bytes)", long_ts.unaligned_str, long_ts.length, test_value, iterations, true, include_old);
    
    printf("----------------------------------------------------------------------------------\n");
    
    benchmark_memset_comparison_cache("Very Long (100K)", very_long_ts.aligned_str, very_long_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Very Long (100K)", very_long_ts.unaligned_str, very_long_ts.length, test_value, iterations, false, include_old);
    benchmark_memset_comparison_cache("Very Long (100K)", very_long_ts.aligned_str, very_long_ts.length, test_value, iterations, true, include_old);
    benchmark_memset_comparison_cache("Very Long (100K)", very_long_ts.unaligned_str, very_long_ts.length, test_value, iterations, true, include_old);
    
    // Cleanup
    free_test_strings(&short_ts);
    free_test_strings(&mid_ts);
    free_test_strings(&long_ts);
    free_test_strings(&very_long_ts);
}

// Wrapper functions for compatibility
void benchmark_memset(void) {
    benchmark_memset_comparison(false);
}

void benchmark_memset_full(void) {
    benchmark_memset_comparison(true);
}
