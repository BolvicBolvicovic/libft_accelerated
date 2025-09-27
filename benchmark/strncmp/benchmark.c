/* ************************************************************************** */
/*                                                                            */
/*                     		STRING COMPARE BENCHMARK			              */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include "../common/benchmark_common.h"

// Configuration for improved benchmark
#define BUFLEN_SMALL    256     // For small string tests
#define BUFLEN_MEDIUM   2048    // For medium string tests  
#define BUFLEN_LARGE    8192    // For large string tests
#define ITERATIONS      1000    // Number of iterations per test

// Forward declarations for old libft functions (only available in full mode) 
#ifdef FULL_COMPARISON_MODE
int ft_strncmp_old(const char* s1, const char* s2, size_t n);
#endif

// Improved strncmp benchmark with variable comparison patterns
static size_t benchmark_strncmp_variable_pattern(int (*strncmp_func)(const char*, const char*, size_t), size_t buflen, const char* test_name) {
    char *str1 = malloc(buflen);
    char *str2 = malloc(buflen);
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }
    
    size_t i;
    size_t comparison_results = 0;
    size_t early_exits = 0;
    size_t full_comparisons = 0;
    
    // Initialize strings with base content
    for (size_t j = 0; j < buflen - 1; j++) {
        str1[j] = 'a' + (j % 26);
        str2[j] = 'a' + (j % 26);
    }
    str1[buflen - 1] = '\0';
    str2[buflen - 1] = '\0';
    
    // Vary comparison patterns during iterations
    for (i = 0; i < ITERATIONS; i++) {
        // Variable comparison length
        size_t cmp_len = (buflen / 2) + (i % (buflen / 4));
        if (cmp_len >= buflen) cmp_len = buflen - 1;
        
        // Create different comparison scenarios
        if (i % 5 == 0) {
            // Equal strings - full comparison needed
            memcpy(str2, str1, cmp_len);
            full_comparisons++;
        } else if (i % 5 == 1) {
            // Early difference - should exit quickly
            str2[0] = str1[0] + 1;
            early_exits++;
        } else if (i % 5 == 2) {
            // Middle difference
            size_t diff_pos = cmp_len / 2;
            str2[diff_pos] = str1[diff_pos] + 1;
        } else if (i % 5 == 3) {
            // Late difference
            size_t diff_pos = cmp_len - 10;
            if (diff_pos >= cmp_len) diff_pos = cmp_len - 1;
            str2[diff_pos] = str1[diff_pos] + 1;
        } else {
            // Mixed pattern - modify multiple positions
            str2[i % (cmp_len/4)] = str1[i % (cmp_len/4)] + (i % 3);
            str2[cmp_len/2] = str1[cmp_len/2] + (i % 2);
        }
        
        // Perform the comparison
        int result = strncmp_func(str1, str2, cmp_len);
        comparison_results += (result < 0) ? 1 : (result > 0) ? 2 : 0;
        
        // Occasionally modify base strings to vary patterns
        if (i % 100 == 0) {
            size_t mod_pos = i % (buflen - 1);
            str1[mod_pos] = 'A' + (i % 26);
            str2[mod_pos] = 'A' + (i % 26);
        }
    }
    
    free(str1);
    free(str2);
    return comparison_results * 1000 + early_exits * 100 + full_comparisons;
}

// Benchmark with aligned/unaligned string addresses
static void benchmark_strncmp_alignment(size_t buflen, const char* size_desc, bool include_old) {
    unsigned long long start, end;
    size_t cs;
    
    printf("%-20s Tests:\n", size_desc);
    printf("  Function         | Alignment | Cycles/Call | Checksum    | Scenarios     | Relative Performance\n");
    printf("  -------------------------------------------------------------------------------------------\n");
    
    // System strncmp - aligned
    start = __rdtsc();
    cs = benchmark_strncmp_variable_pattern(strncmp, buflen, "system");
    end = __rdtsc();
    unsigned long long system_cycles = (end - start) / ITERATIONS;
    size_t early_exits = (cs % 1000) / 100;
    size_t full_comps = cs % 100;
    printf("  %-15s | %-9s | %11llu | %11zu | E%zu/F%zu | %s\n", 
           "system strncmp", "aligned", system_cycles, cs, early_exits, full_comps, "baseline");
    
    // ft_strncmp - aligned  
    start = __rdtsc();
    cs = benchmark_strncmp_variable_pattern((int (*)(const char*, const char*, size_t))ft_strncmp, buflen, "optimized");
    end = __rdtsc();
    unsigned long long ft_cycles = (end - start) / ITERATIONS;
    double ft_ratio = (double)ft_cycles / system_cycles;
    early_exits = (cs % 1000) / 100;
    full_comps = cs % 100;
    printf("  %-15s | %-9s | %11llu | %11zu | E%zu/F%zu | %.2fx %s\n", 
           "ft_strncmp", "aligned", ft_cycles, cs, early_exits, full_comps, ft_ratio,
           ft_ratio < 1.0 ? "(faster)" : "(slower)");
    
#ifdef FULL_COMPARISON_MODE
    if (include_old) {
        start = __rdtsc();
        cs = benchmark_strncmp_variable_pattern((int (*)(const char*, const char*, size_t))ft_strncmp_old, buflen, "old");
        end = __rdtsc();
        unsigned long long ft_old_cycles = (end - start) / ITERATIONS;
        double old_ratio = (double)ft_old_cycles / system_cycles;
        early_exits = (cs % 1000) / 100;
        full_comps = cs % 100;
        printf("  %-15s | %-9s | %11llu | %11zu | E%zu/F%zu | %.2fx %s\n", 
               "ft_strncmp_old", "aligned", ft_old_cycles, cs, early_exits, full_comps, old_ratio,
               old_ratio < 1.0 ? "(faster)" : "(slower)");
    }
#endif

    // Test unaligned access patterns
    char *str1_unaligned = malloc(buflen + 16);
    char *str2_unaligned = malloc(buflen + 16);
    if (str1_unaligned && str2_unaligned) {
        // Create unaligned strings
        char *str1_offset = str1_unaligned + 5;
        char *str2_offset = str2_unaligned + 9; // Different offset
        
        // Initialize with pattern
        for (size_t j = 0; j < buflen - 1; j++) {
            str1_offset[j] = 'a' + (j % 26);
            str2_offset[j] = 'a' + (j % 26);
        }
        str1_offset[buflen - 1] = '\0';
        str2_offset[buflen - 1] = '\0';
        
        // Quick unaligned test for ft_strncmp only
        start = __rdtsc();
        size_t unaligned_results = 0;
        
        for (size_t i = 0; i < ITERATIONS; i++) {
            size_t cmp_len = (buflen / 2) + (i % (buflen / 4));
            if (cmp_len >= buflen) cmp_len = buflen - 1;
            
            // Vary comparison patterns
            if (i % 3 == 0) {
                str2_offset[0] = str1_offset[0] + 1; // Early diff
            } else if (i % 3 == 1) {
                memcpy(str2_offset, str1_offset, cmp_len); // Equal
            } else {
                str2_offset[cmp_len/2] = str1_offset[cmp_len/2] + 1; // Mid diff
            }
            
            int result = ft_strncmp(str1_offset, str2_offset, cmp_len);
            unaligned_results += (result < 0) ? 1 : (result > 0) ? 2 : 0;
        }
        
        end = __rdtsc();
        unsigned long long unaligned_cycles = (end - start) / ITERATIONS;
        double unaligned_ratio = (double)unaligned_cycles / system_cycles;
        
        printf("  %-15s | %-9s | %11llu | %11zu | Mixed     | %.2fx %s\n", 
               "ft_strncmp", "unaligned", unaligned_cycles, unaligned_results, unaligned_ratio,
               unaligned_ratio < 1.0 ? "(faster)" : "(slower)");
        
        free(str1_unaligned);
        free(str2_unaligned);
    }
    
    printf("\n");
}

void benchmark_strncmp_comparison(bool include_old) {
    printf("=== IMPROVED STRNCMP BENCHMARK ===\n");
    printf("Using variable comparison pattern technique with %d iterations per test\n\n", ITERATIONS);
    
    // Test different string sizes
    benchmark_strncmp_alignment(BUFLEN_SMALL, "Small (256 chars)", include_old);
    benchmark_strncmp_alignment(BUFLEN_MEDIUM, "Medium (2KB)", include_old);
    benchmark_strncmp_alignment(BUFLEN_LARGE, "Large (8KB)", include_old);
    
    printf("=== SUMMARY ===\n");
    printf("This benchmark creates mixed comparison scenarios: early differences (E),\n");
    printf("full comparisons (F), middle/late differences for realistic patterns.\n");
    printf("Early exits should be faster, full comparisons exercise the entire algorithm.\n");
    printf("Checksum encodes comparison results and scenario distribution.\n");
}

// Wrapper functions for compatibility
void benchmark_strncmp(void) {
    benchmark_strncmp_comparison(false);
}

void benchmark_strncmp_full(void) {
    benchmark_strncmp_comparison(true);
}
