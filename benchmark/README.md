# Libft Accelerated Benchmarks

## Overview

This is a benchmarking system for comparing optimized libft functions against their original implementations and system equivalents. The system uses a unified structure where each function has a single benchmark file that can test multiple implementations (system, optimized, and old versions) depending on build options.

## Directory Structure

```
benchmark/
├── build.sh                    # Unified build script
├── README.md                   # This documentation
├── common/                     # Shared utilities
│   ├── benchmark_common.h      # Common header with shared types/functions
│   ├── benchmark_common.c      # Common benchmark utilities
│   └── old_wrappers.c          # Wrappers for old libft functions
├── strlen/                     # strlen function benchmarks
│   └── benchmark.c             # Benchmark implementation
├── memset/                     # memset function benchmarks
│   └── benchmark.c             # Benchmark implementation
└── libft_old/                  # Original libft implementation
    ├── libft.h                 # Original header
    ├── libft.a                 # Compiled library
    ├── Makefile                # Build configuration
    └── ft_*.c                  # Original implementations (42+ functions)
```

## Usage

### Build Options

```bash
# Compare system vs optimized (all functions)
./build.sh

# Compare system vs optimized vs old (all functions)
./build.sh --full

# Compare system vs optimized (specific function)
./build.sh -f strlen
./build.sh -f memset

# Compare system vs optimized vs old (specific function)
./build.sh -f strlen --full
./build.sh -f memset --full

# Show help
./build.sh -h
```

### Running Benchmarks

After building, run the benchmark:

```bash
./benchmark.out
```

The output will depend on what was built:

- **Standard mode**: Compares system functions vs optimized libft functions
- **Full mode** (`--full`): Compares system vs optimized vs old libft implementations
- **All functions**: Runs benchmarks for all available functions
- **Specific function**: Runs benchmark for that function only

## Available Functions

Currently supported functions:

- `strlen` - String length
- `memset` - Memory setting
- `memcpy` - Memory copy
- `strchr` - Byte finder
- `strncmp`- String compare
- `memchr` - Memory character search

## Benchmark Features

Each benchmark includes:

- **Alignment testing**: Tests both aligned and unaligned memory access patterns
- **Cache effects**: Tests both hot (cached) and cold (cache-flushed) scenarios
- **Multiple sizes**: Tests various data sizes (10 bytes, 1KB, 10KB, 100KB)
- **Performance metrics**: Measures CPU cycles and calculates improvement ratios
- **Individual measurements**: Shows detailed timing for variance analysis
- **Memory alignment analysis**: Reports actual memory addresses and alignment

## Adding New Functions

To add a new function benchmark:

1. Create a new directory: `mkdir newfunc/`
2. Create the benchmark file: `newfunc/benchmark.c`
3. Implement both optimized and old version tests in the single file
4. Add any necessary function declarations to `common/benchmark_common.h`
5. Add wrapper functions for old implementations in `common/old_wrappers.c` if needed
6. Update the build script to include the new function

## Technical Details

- **Compiler**: GCC with `-O2 -march=native` optimizations
- **Timing**: Uses `__rdtsc()` for precise CPU cycle measurement
- **Cache management**: Uses `clflush` and `mfence` instructions for cache control
- **Memory alignment**: Creates both 16-byte aligned and deliberately unaligned test data
- **Optimization prevention**: Uses volatile variables to prevent compiler optimizations

## Notes

- The system requires x86 architecture for RDTSC and cache flush instructions
- Results may vary depending on CPU architecture and system load
- For accurate results, run on a relatively idle system
- The benchmark automatically seeds random number generation for consistent test data
