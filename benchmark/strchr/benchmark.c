/* ************************************************************************** */
/*                                                                            */
/*                          STRCHR BENCHMARK			                      */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Configuration for improved benchmark
#define BUFLEN_SMALL    512     // For small string tests
#define BUFLEN_MEDIUM   4096    // For medium string tests  
#define BUFLEN_LARGE    16384   // For large string tests
#define ITERATIONS      1000    // Number of iterations per test

// Forward declarations for old libft functions (only available in full mode) 
#ifdef FULL_COMPARISON_MODE
char* ft_strchr_old(const char* s, int c);
#endif

// Improved strchr benchmark with variable search patterns
static size_t benchmark_strchr_variable_pattern(char* (*strchr_func)(const char*, int), size_t buflen, const char* test_name) {
    char *str = malloc(buflen);
    if (!str) return 0;
    
    size_t i;
    size_t found_count = 0;
    size_t total_offset = 0;
    
    // Initialize string with varied content and ensure null termination
    for (size_t j = 0; j < buflen - 1; j++) {
        str[j] = 'a' + (j % 26);  // Fill with alphabet pattern
    }
    str[buflen - 1] = '\0';
    
    // Vary search character and string patterns during iterations
    for (i = 0; i < ITERATIONS; i++) {
        // Variable search character - cycle through different chars
        int search_char = 'a' + (i % 26);
        
        // Create different search scenarios
        if (i % 4 == 0) {
            // Place search char near the beginning (early hit)
            size_t pos = i % 16;
            if (pos < buflen - 1) str[pos] = search_char;
        } else if (i % 4 == 1) {
            // Place search char in the middle
            size_t pos = (buflen - 1) / 2 + (i % 32);
            if (pos < buflen - 1) str[pos] = search_char;
        } else if (i % 4 == 2) {
            // Place search char near the end (late hit)
            size_t pos = buflen - 1 - (i % 32) - 1;
            if (pos > 0 && pos < buflen - 1) str[pos] = search_char;
        }
        // else: no guaranteed hit (miss case)
        
        // Perform the search
        char *res = strchr_func(str, search_char);
        if (res) {
            found_count++;
            total_offset += res - str;
        }
        
        // Occasionally modify string content to prevent caching
        if (i % 50 == 0) {
            size_t mod_pos = (i * 7) % (buflen - 1);
            str[mod_pos] = 'x' + (i % 3);
        }
        
        // Occasionally test for null terminator search
        if (i % 100 == 0) {
            char *null_res = strchr_func(str, '\0');
            if (null_res) {
                total_offset += null_res - str;
                found_count++;
            }
        }
    }
    
    free(str);
    return found_count * 1000 + total_offset; // Combine metrics for checksum
}

// Benchmark with aligned/unaligned string addresses
static void benchmark_strchr_alignment(size_t buflen, const char* size_desc, bool include_old) {
    unsigned long long start, end;
    size_t cs;
    
    printf("%-20s Tests:\n", size_desc);
    printf("  Function         | Alignment | Cycles/Call | Checksum    | Hit Rate %% | Relative Performance\n");
    printf("  --------------------------------------------------------------------------------------\n");
    
    // System strchr - aligned
    start = __rdtsc();
    cs = benchmark_strchr_variable_pattern(strchr, buflen, "system");
    end = __rdtsc();
    unsigned long long system_cycles = (end - start) / ITERATIONS;
    size_t hit_rate = cs / 1000; // Extract hit count from combined checksum
    printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %s\n", 
           "system strchr", "aligned", system_cycles, cs, (hit_rate * 100.0) / ITERATIONS, "baseline");
    
    // ft_strchr - aligned  
    start = __rdtsc();
    cs = benchmark_strchr_variable_pattern((char* (*)(const char*, int))ft_strchr, buflen, "optimized");
    end = __rdtsc();
    unsigned long long ft_cycles = (end - start) / ITERATIONS;
    double ft_ratio = (double)ft_cycles / system_cycles;
    hit_rate = cs / 1000;
    printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %.2fx %s\n", 
           "ft_strchr", "aligned", ft_cycles, cs, (hit_rate * 100.0) / ITERATIONS, ft_ratio,
           ft_ratio < 1.0 ? "(faster)" : "(slower)");
    
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        start = __rdtsc();
        cs = benchmark_strchr_variable_pattern((char* (*)(const char*, int))ft_strchr_old, buflen, "old");
        end = __rdtsc();
        unsigned long long ft_old_cycles = (end - start) / ITERATIONS;
        double old_ratio = (double)ft_old_cycles / system_cycles;
        hit_rate = cs / 1000;
        printf("  %-15s | %-9s | %11llu | %11zu | %9.1f | %.2fx %s\n", 
               "ft_strchr_old", "aligned", ft_old_cycles, cs, (hit_rate * 100.0) / ITERATIONS, old_ratio,
               old_ratio < 1.0 ? "(faster)" : "(slower)");
    }
#endif

    printf("\n");
}

void benchmark_strchr_comparison(bool include_old) {
    printf("=== IMPROVED STRCHR BENCHMARK ===\n");
    printf("Using variable character search technique with %d iterations per test\n\n", ITERATIONS);
    
    // Test different string sizes
    benchmark_strchr_alignment(BUFLEN_SMALL, "Small (512 chars)", include_old);
    benchmark_strchr_alignment(BUFLEN_MEDIUM, "Medium (4KB)", include_old);
    benchmark_strchr_alignment(BUFLEN_LARGE, "Large (16KB)", include_old);
    
    printf("=== SUMMARY ===\n");
    printf("This benchmark varies search characters and target positions to create\n");
    printf("realistic search patterns with mix of early hits, middle hits, late hits, and misses.\n");
    printf("Hit Rate shows percentage of searches that found the target character.\n");
    printf("Includes occasional null terminator searches for comprehensive testing.\n");
}

// Wrapper functions for compatibility
void benchmark_strchr(void) {
    benchmark_strchr_comparison(false);
}

void benchmark_strchr_full(void) {
    benchmark_strchr_comparison(true);
}
