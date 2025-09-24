# libft_accelerated

A high-performance, SIMD-optimized implementation of the standard C library functions, inspired by the 42 School's libft project, glibc, and musl libc.

![Some good meme](./public/libft_meme.jpg)

![Architecture Support](https://img.shields.io/badge/Architecture-x86__64-blue)
![SIMD Support](https://img.shields.io/badge/SIMD-SSE2%20%7C%20AVX2-green)
![Project Type](https://img.shields.io/badge/Project-Enhanced%2042%20School-purple)

## ✨ Features

- **🚀 SIMD Optimization**: Leverages SSE2 and AVX2 instructions for maximum performance on x86_64
- **🎯 High Performance**: Optimized assembly routines and bit manipulation techniques
- **🔧 x86_64 Architecture**: Optimized for modern Intel and AMD processors
- **📊 Comprehensive Benchmarking**: Built-in performance comparison tools
- **🧪 Extensive Testing**: Thorough test suite ensuring correctness
- **⚡ Zero Dependencies**: Standalone implementation with minimal system calls
- **🏗️ 42 School Compatible**: Enhanced version of the mandatory libft functions

## 🏛️ Architecture & Design

### SIMD Acceleration

The library automatically detects and utilizes the best available SIMD instruction set on x86_64:

- **AVX2** (256-bit vectors): Faster performance on modern x86_64 CPUs
- **SSE2** (128-bit vectors): Baseline x86_64 support
- **Fallback**: Optimized scalar implementations with bit manipulation tricks

### Key Optimizations

- **Vectorized String Operations**: Process 16-32 bytes per instruction
- **Parallelization**: Process 64 bytes in most loop iterations
- **Bit Scanning**: Hardware-accelerated bit manipulation (BMI instruction sets)
- **Word-Level Processing**: Efficient byte-at-a-time operations using word-sized chunks
- **Memory Alignment**: Automatic alignment optimization for better cache performance

## 📦 Supported Functions

### Character Classification & Conversion

- `ft_isalpha` - Alphabetic character checking
- `ft_isdigit` - Digit character validation
- `ft_isalnum` - Alphanumeric character checking
- `ft_isascii` - ASCII character validation
- `ft_isprint` - Printable character checking
- `ft_toupper` - Uppercase character conversion
- `ft_tolower` - Lowercase character conversion

### String Operations

- `ft_strlen` - SIMD-accelerated string length calculation
- `ft_strchr` - Vectorized character searching
- `ft_strlcpy` - Safe string copying with length control
- `ft_strlcat` - Safe string concatenation

### Memory Operations

- `ft_memset` - SIMD-accelerated memory filling
- `ft_memcpy` - SIMD-accelerated memory copying with safe overlapping
- `ft_memmove` - Wrapper around `ft_memcpy`
- `ft_bzero` - Wrapper around ft_memset

### Conversion & I/O

- `ft_itoa` - Integer to ASCII conversion with custom buffer management (Note: This will be completelly reworked when I will be able to allocate memory 🙂)
- `ft_write` - Write syscall wrapper with inline assembly (unist...what? don't remember...)
- `ft_printline` - Utility function for line output

### Planned Functions (Not Yet Implemented)

The following functions are planned for future releases:

- `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_strdup`
- `ft_strmapi`, `ft_striteri`
- `ft_memchr`, `ft_memcmp`, `ft_calloc`
- `ft_atoi`
- `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`
- Linked list operations (`ft_lst*` functions)
- and more...

## 🚀 Getting Started

### Prerequisites

- GCC or Clang
- x86_64 processor

### Compilation

```bash
# Clone the repository
git clone https://github.com/BolvicBolvicovic/libft_accelerated.git
cd libft_accelerated

# The library is header-only - just include libft.h in your project
gcc -O2 -march=native your_program.c -I. -o your_program
```

### Recommended Compiler Flags

```bash
# For maximum performance
-O3 -march=native -mtune=native

# For AVX2 support specifically
-O3 -mavx2 -mbmi

# For SSE2 baseline
-O3 -msse2

# I personally use these
-O2 -march=native
```

## 📈 Performance Benchmarks

Run comprehensive benchmarks comparing system functions, optimized implementations, and original libft:

```bash
cd benchmark

# Quick benchmark (system vs optimized)
./build.sh

# Full comparison (system vs optimized vs original)
./build.sh --full

# Specific function benchmarks
./build.sh -f strlen
./build.sh -f memset --full
```

### Sample Results

```text
Memory Copy Benchmark - System vs Optimized vs Old (averaged over 50000 iterations)
==============================================================================================
Size                 | Alignment | Cache | System (cyc) | Optimized (cyc) | Old (cyc)     | Opt Ratio | Old Ratio
==============================================================================================
Short (10 bytes)     | Aligned   | Hot   |             7 |               5 |            46 |      0.71x | 6.57x
Short (10 bytes)     | Unaligned | Hot   |             7 |               5 |            46 |      0.71x | 6.57x
Short (10 bytes)     | Aligned   | Cold  |            17 |              11 |            52 |      0.65x | 3.06x
Short (10 bytes)     | Unaligned | Cold  |            13 |              12 |            53 |      0.92x | 4.08x
----------------------------------------------------------------------------------
Mid (1K bytes)       | Aligned   | Hot   |            29 |              29 |          5621 |      1.00x | 193.83x
Mid (1K bytes)       | Unaligned | Hot   |            33 |              29 |          4835 |      0.88x | 146.52x
Mid (1K bytes)       | Aligned   | Cold  |            38 |              40 |          4266 |      1.05x | 112.26x
Mid (1K bytes)       | Unaligned | Cold  |            44 |              38 |          4272 |      0.86x | 97.09x
----------------------------------------------------------------------------------
Long (10K bytes)     | Aligned   | Hot   |           239 |             239 |         50693 |      1.00x | 212.10x
Long (10K bytes)     | Unaligned | Hot   |           251 |             247 |         46193 |      0.98x | 184.04x
Long (10K bytes)     | Aligned   | Cold  |           283 |             278 |         44189 |      0.98x | 156.14x
Long (10K bytes)     | Unaligned | Cold  |           287 |             281 |         42497 |      0.98x | 148.07x
----------------------------------------------------------------------------------
Very Long (100K)     | Aligned   | Hot   |          5006 |            5111 |        481480 |      1.02x | 96.18x
Very Long (100K)     | Unaligned | Hot   |          5085 |            5446 |        495570 |      1.07x | 97.46x
Very Long (100K)     | Aligned   | Cold  |          5318 |            5377 |        482597 |      1.01x | 90.75x
Very Long (100K)     | Unaligned | Cold  |          5333 |            5350 |        492382 |      1.00x | 92.33x
```

## 🧪 Testing

Run the comprehensive test suite:

```bash
cd tests
./build.sh
./tester.out
```

The test suite includes:

- Correctness verification against system functions
- Edge case testing (NULL pointers, empty strings, boundaries)
- Memory safety validation

## 🎯 Use Cases

- **42 School Projects**: Enhanced version of the mandatory libft with modern optimizations. Can used if the Norm is not mandatory and you don't know that the glibc exists.
- **Educational Purpose**: Learning SIMD programming and advanced optimization techniques.
- **High-Performance Applications**: Games, real-time systems, HPC applications.
- **Performance Research**: Reference implementation for benchmarking and optimization studies.
- **x86_64 Development**: Optimized implementations for Intel/AMD architectures.

## 🤝 Contributing

Contributions are welcome! Areas for improvement:

- **Complete libft implementation**: Add remaining standard functions (atoi, string manipulation, linked lists)
- **ARM64/NEON support**: Implement ARM architecture optimizations
- **Additional SIMD instruction sets**: AVX-512 support for newer processors
- **Windows/MSVC compatibility**: Cross-platform development support
- **Additional architecture support**: RISC-V and other emerging platforms
- **Performance optimizations**: Further micro-optimizations and algorithmic improvements

### Development Setup

```bash
# Run benchmarks during development
./benchmark/build.sh -f your_function

# Verify correctness
./tests/build.sh && ./tests/tester.out

# Check different optimization levels
gcc -O1/-O2/-O3 -march=native your_test.c
```

## 📚 References & Inspiration

- **42 School libft**: Original project specification and test requirements
- **glibc**: Advanced optimization techniques and SIMD implementations for x86_64
- **musl libc**: Clean, efficient scalar implementations as fallback reference
- **Intel Optimization Manual**: SIMD programming best practices and instruction references
- **AMD64 Architecture**: x86_64 specific optimization techniques

## ⚖️ License

This is an educational project created as an enhanced version of the 42 School libft assignment. The code is provided for learning and demonstration purposes.

## 🙏 Acknowledgments

- 42 School for the original libft project concept and educational framework
- The glibc and musl libc developers for optimization inspiration and reference implementations
- Intel and ARM for comprehensive SIMD documentation and optimization guides

---
