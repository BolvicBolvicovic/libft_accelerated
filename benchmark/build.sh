#!/bin/bash

# Unified Build Script for libft benchmarks
# Usage: 
#   ./build.sh                         - Compare system vs optimized (all functions)
#   ./build.sh --full                  - Compare system vs optimized vs old (all functions)
#   ./build.sh -f <function>           - Compare system vs optimized (specific function)
#   ./build.sh -f <function> --full    - Compare system vs optimized vs old (specific function)

CC=gcc
CFLAGS="-O2 -march=native"
EXEC=benchmark.out
COMMON_FILES="common/benchmark_common.c"
OLD_IMPLEMENTATION_FILES="common/old_implementations.c"
BM="benchmark.c"
BENCHMARK_FILES="strlen/${BM} memset/${BM} memcpy/${BM} strchr/${BM} strncmp/${BM}"

# Function to show usage
show_usage() {
    echo "Usage:"
    echo "  $0                         - Compare system vs optimized (all functions)"
    echo "  $0 --full                  - Compare system vs optimized vs old (all functions)"
    echo "  $0 -f <function>           - Compare system vs optimized (specific function)"
    echo "  $0 -f <function> --full    - Compare system vs optimized vs old (specific function)"
    echo ""
    echo "Available functions: strlen, memset, memcpy, strchr, strncmp"
    exit 0
}

# Function to build all standard comparisons (system vs optimized)
build_all_standard_comparison() {
    echo "Building all benchmarks comparison (system vs optimized)..."
    
    # Create main file that calls comparison benchmarks for all functions
    cat > main_temp.c << EOF
#include "common/benchmark_common.h"

int main(int argc, char **argv) {
    (void)argc;  // Suppress unused parameter warning
    (void)argv;  // Suppress unused parameter warning
    
    srand(time(NULL));
    
    printf("=======================================================\\n");
    printf("LIBFT BENCHMARKS - ALL FUNCTIONS (System vs Optimized)\\n");
    printf("=======================================================\\n\\n");
    
    benchmark_strlen();
    
    printf("\\n\\n=======================================================\\n\\n");
    
    benchmark_memset();
    
    printf("\\n\\n=======================================================\\n\\n");

	benchmark_memcpy();

    printf("\\n\\n=======================================================\\n\\n");

	benchmark_strchr();

    printf("\\n\\n=======================================================\\n\\n");

	benchmark_strncmp();

    printf("\\n\\n=======================================================\\n\\n");
    
    return 0;
}
EOF
    
    # Compile with comparison versions - link with new libft.a
    $CC $CFLAGS main_temp.c $COMMON_FILES $BENCHMARK_FILES -I.. ../libft.a -o $EXEC
    
    if [ $? -eq 0 ]; then
        echo "✓ Successfully built $EXEC with all functions standard comparison"
        rm -f main_temp.c
    else
        echo "✗ Build failed!"
        rm -f main_temp.c
        exit 1
    fi
}

# Function to build all full comparisons (system vs optimized vs old)
build_all_full_comparison() {
    echo "Building all benchmarks comparison (system vs optimized vs old)..."
    
    # Create main file that calls full comparison benchmarks for all functions
    cat > main_temp.c << EOF
#include "common/benchmark_common.h"

int main(int argc, char **argv) {
    (void)argc;  // Suppress unused parameter warning
    (void)argv;  // Suppress unused parameter warning
    
    srand(time(NULL));
    
    printf("=======================================================\\n");
    printf("LIBFT BENCHMARKS - ALL FUNCTIONS (System vs Optimized vs Old)\\n");
    printf("=======================================================\\n\\n");
    
    benchmark_strlen_full();
    
    printf("\\n\\n=======================================================\\n\\n");
    
    benchmark_memset_full();
    
    printf("\\n\\n=======================================================\\n\\n");
    
	benchmark_memcpy_full();

    printf("\\n\\n=======================================================\\n\\n");

	benchmark_strchr_full();

    printf("\\n\\n=======================================================\\n\\n");

	benchmark_strncmp();

    printf("\\n\\n=======================================================\\n\\n");

    return 0;
}
EOF
    
    # Compile with comparison versions and old implementations - link with new optimized archive
    $CC $CFLAGS -DFULL_COMPARISON_MODE main_temp.c $COMMON_FILES $OLD_IMPLEMENTATION_FILES $BENCHMARK_FILES -I.. ../libft.a -o $EXEC
    
    if [ $? -eq 0 ]; then
        echo "✓ Successfully built $EXEC with all functions full comparison"
        rm -f main_temp.c
    else
        echo "✗ Build failed!"
        rm -f main_temp.c
        exit 1
    fi
}

# Function to build standard comparison (system vs optimized)
build_standard_comparison() {
    local func_name=$1
    
    echo "Building ${func_name} comparison benchmark (system vs optimized)..."
    
    # Create main file that calls the comparison benchmark
    cat > main_temp.c << EOF
#include "common/benchmark_common.h"

int main(int argc, char **argv) {
    (void)argc;  // Suppress unused parameter warning
    (void)argv;  // Suppress unused parameter warning
    
    srand(time(NULL));
    
    printf("=======================================================\\n");
    printf("LIBFT BENCHMARK - ${func_name^^} (System vs Optimized)\\n");
    printf("=======================================================\\n\\n");
    
    benchmark_${func_name}();
    
    printf("\\n\\n=======================================================\\n\\n");
    
    return 0;
}
EOF
    
    # Compile with comparison version - link with new libft.a
    $CC $CFLAGS main_temp.c $COMMON_FILES ${func_name}/$BM -I.. ../libft.a -o $EXEC

    if [ $? -eq 0 ]; then
        echo "✓ Successfully built $EXEC with standard comparison benchmark"
        rm -f main_temp.c
    else
        echo "✗ Build failed!"
        rm -f main_temp.c
        exit 1
    fi
}

# Function to build full comparison (system vs optimized vs old)
build_full_comparison() {
    local func_name=$1
    
    echo "Building ${func_name} full comparison benchmark (system vs optimized vs old)..."
    
    # Create main file that calls the full comparison benchmark
    cat > main_temp.c << EOF
#include "common/benchmark_common.h"

int main(int argc, char **argv) {
    (void)argc;  // Suppress unused parameter warning
    (void)argv;  // Suppress unused parameter warning
    
    srand(time(NULL));
    
    printf("=======================================================\\n");
    printf("LIBFT BENCHMARK - ${func_name^^} (System vs Optimized vs Old)\\n");
    printf("=======================================================\\n\\n");
    
    benchmark_${func_name}_full();
    
    printf("\\n\\n=======================================================\\n\\n");
    
    return 0;
}
EOF
    
    # Compile with comparison version and old implementations - link with new optimized archive
    $CC $CFLAGS -DFULL_COMPARISON_MODE main_temp.c $COMMON_FILES $OLD_IMPLEMENTATION_FILES ${func_name}/$BM -I.. ../libft.a -o $EXEC
    
    if [ $? -eq 0 ]; then
        echo "✓ Successfully built $EXEC with full comparison benchmark"
        rm -f main_temp.c
    else
        echo "✗ Build failed!"
        rm -f main_temp.c
        exit 1
    fi
}

# Parse command line arguments
FUNCTION=""
FULL_COMPARISON=false

while getopts "f:h-:" opt; do
    case $opt in
        f)
            FUNCTION="$OPTARG"
            ;;
        h)
            show_usage
            ;;
        -)
            case "${OPTARG}" in
                full)
                    FULL_COMPARISON=true
                    ;;
                *)
                    echo "Invalid long option: --$OPTARG" >&2
                    show_usage
                    ;;
            esac
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            show_usage
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            show_usage
            ;;
    esac
done

# Validate function name if provided
if [ -n "$FUNCTION" ]; then
    case $FUNCTION in
        strlen|memset|memcpy|strchr|strncmp)
            ;;
        *)
            echo "Error: Unknown function '$FUNCTION'"
            echo "Available functions: strlen, memset, memcpy, strchr, strncmp"
            exit 1
            ;;
    esac
fi



# Main logic
if [ -z "$FUNCTION" ]; then
    # No function specified - build all
    if [ "$FULL_COMPARISON" = true ]; then
        build_all_full_comparison
    else
        build_all_standard_comparison
    fi
else
    # Function specified
    if [ "$FULL_COMPARISON" = true ]; then
        build_full_comparison "$FUNCTION"
    else
        build_standard_comparison "$FUNCTION"
    fi
fi

echo ""
echo "Build complete! Run with: ./$EXEC"
rm *.o