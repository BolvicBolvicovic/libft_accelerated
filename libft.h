#ifndef LIBFT_H
#define LIBFT_H

#pragma region Architecture Detection
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)

# include <emmintrin.h>
# include <immintrin.h>

# ifndef __BMI2__
#  error "The Bit Manipulation Set 2 is necessary"
# endif

# define BitScanForward(a)	(_tzcnt_u64(a))
# define BitScanBackward(a) (_lzcnt_u64(a))
// Note: Copies a and sets its high bits to 0 starting at index i.
# define BitZeroHigh(a, i)	(_bzhi_u64(a, i))

# if defined(__AVX2__)

typedef __m256i				vector;
#  define VecSize			(sizeof(vector))
#  define Zero				(_mm256_setzero_si256)
#  define Set1_int8(a)		(_mm256_set1_epi8(a))
#  define CmpEq_int8(a, b)	(_mm256_cmpeq_epi8(a, b))
#  define Min_uint8(a, b)	(_mm256_min_epu8(a, b))
#  define MoveMask_uint8(a)	(_mm256_movemask_epi8(a))

# elif defined(__SSE2__)

typedef __m128i				vector;
#  define VecSize			(sizeof(vector))
#  define Zero				(_mm_setzero_si128)
// Note: Set1 is actually a sequence of instruction.
// 1. Loads the char into vector register	"c"
// 2. Duplicate it into a 16-bits word		"cc"
// 3. Duplicate it into a 32-bits word		"cccc"
// 4. Select 32-bits word for each slots, 	"cccccccc"
// 	(32 * 4 = 128) and shuffle them in.
#  define Set1_int8(a)		(_mm_set1_epi8(a))
#  define CmpEq_int8(a, b)	(_mm_cmpeq_epi8(a, b))
#  define Min_uint8(a, b)	(_mm_min_epu8(a, b))
#  define MoveMask_uint8(a)	(_mm_movemask_epi8(a))

# endif
#endif // Defined architecture x64

#if defined(__aarch64__) || defined(_M_ARM64)

#include <arm_neon.h>

#endif // Defined architecture ARM64

// Note: Returns the number of trailing 0-bytes in x, starting at the least significant bit position.
#define ByteScanForward(a)	(BitScanForward(a) / ByteSize)
#define Aligned(a)		(((uintptr)(a) & (VecSize - 1)) == 0)

#pragma endregion Architecture Detection
#pragma region Type Definitions

#if defined(__GNUC__) || defined(__clang__)

#ifndef size_t
typedef __SIZE_TYPE__		size_t;
#endif
typedef	size_t			__attribute__((may_alias)) word;

#elif defined(_MSC_VER)

#ifndef size_t
typedef SIZE_T			size_t;
#endif
typedef size_t			word;

#define __always_inline		__forceinline

#else

#ifndef size_t
typedef long unsigned int	size_t;
#endif
typedef size_t				word;

#define __always_inline		inline

#endif

typedef unsigned long 	uintptr;
typedef unsigned char 	uint8;
typedef unsigned		uint32;
typedef __uint16_t		uint16;	
typedef __int128_t		uint128;
typedef long int		ssize_t;
typedef _Bool			bool;

#define true			1
#define false			0

#define ByteSize		8
#define UCharMax		255
#define IntMin			(-2147483648)
#define IntMax			2147483647
#define SizeTMax		((size_t)-1)
#define SSizeTMin		((ssize_t)-1)

#pragma endregion Type Definitions
#pragma region Helper Macros

#define WordSize		(sizeof(word))
#define WordAlignSize	(WordSize - 1)
#define WordAligned(a)	((uintptr)(a) & WordAlignSize == 0)

// Note: 0x01010101... for size_t
#define LSB ((size_t)-1 / UCharMax)
// Note: 0x80808080... for size_t
#define MSB (LSB * (UCharMax / 2 + 1))
// Note: Here is how it works:
// 1. Subtract 0x01010101... from each byte.
// This will set the high bit of each byte that was originally zero.
// Example: 0x00D3FF7A - 0x01010101 = 0xFF02FE79 (only the first byte has high bit set 0xFF)
// 2. Bitwise NOT of the original value (~v).
// For each byte x:
//   ~x has its high bit = 1 iff x < 0x80.
// We later require BOTH (x - 1) high bit (only x == 0) AND ~x high bit,
// so only x == 0 survives after ANDing with 0x80.
// Example: 0xFF02FE79 & 0xFF2C0085 = 0xFF000079 (only the first byte has high bit set 0xFF)
// 3. AND the results of step 1 and step 2.
// x as a byte of v:
// Only x == 0x00 makes both (x - 1) and ~x have their high bit set simultaneously.
// This will leave only the high bits of bytes that were originally zero.
//#define FirstZero(v)		((v) - LSB & ~(v) & MSB)
__always_inline word
FirstZero(word x)
{
	return (x - LSB) & ~x & MSB;
}

#define	ZeroScanForward(a)	(ByteScanForward(FirstZero(a)))

#define RepeatByteInWord(a)	((word)((uint8)(a) * LSB))

// Note: Explanation
// 1. Mask out all MSB to avoid them being carried on the next byte when we are adding.
// 2. Adding Mask so that only 0-byte turn into 0x7f.
// 3. Since we masked out the MSB, if one of the byte had it set, OR to add it and OR to set all bits that are not MSB to 1.
// 4. Invert so that only where there is an MSB, it is a 0-byte.
__always_inline  word
AllZeros(word x)
{
	// Note: Masking MSB => 0x7f == 0b01111111
	static const word	m = (RepeatByteInWord(0x7f));

	return ~(((x & m) + m) | x | m);
}

#define WordAlignDown(a)	((__typeof__(a)) ((uintptr)(a) & -WordSize))

#define	ShiftFind(a, s_int)	((a) >> (ByteSize * ((s_int) % WordSize)))

#if defined(__SSE2__) || defined(__AVX2__) || defined(__aarch64__) || defined(_M_ARM64)
# define SIMD_SUPPORTED 1
#endif

#if defined(SIMD_SUPPORTED)
__always_inline vector
LoadUnaligned(const vector* ptr)
{
# if defined(__AVX2__)
	return _mm256_loadu_si256(ptr);
# elif defined(__SSE2__)
	return _mm_loadu_si128(ptr);
# endif
}

__always_inline void
StoreUnaligned(vector* ptr, vector a)
{
# if defined(__AVX2__)
	_mm256_storeu_si256(ptr, a);
# elif defined(__SSE2__)
	_mm_storeu_si128(ptr, a);
# endif
}
#endif

#pragma endregion Helper Macros
#pragma region Libft Function Prototypes

bool	ft_isalpha(uint8 c);
bool	ft_isdigit(uint8 c);
bool	ft_isalnum(uint8 c);
bool	ft_isascii(uint8 c);
bool	ft_isprint(uint8 c);
size_t	ft_strlen(const char *s);
void*	ft_memset(void* s, uint8 c, size_t n);
void	ft_bzero(void* s, size_t n);
void*	ft_memcpy(void* dst, const char* src, size_t n);
void*	ft_memmove(void* dst, const char* src, size_t n);
size_t	ft_strlcpy(char* dst, const char* src, size_t n);
size_t	ft_strlcat(char* dst, const char* src, size_t n);
uint8	ft_toupper(uint8 c);
uint8	ft_tolower(uint8 c);
char*	ft_strchr(char* s, uint8 c);
char*	ft_strrchr(char* s, uint8 c);
int		ft_strncmp(const char* s1, const char* s2, size_t n);
void*	ft_memchr(void* s, uint8 c, size_t n);

char*	ft_itoa(char dst[12], int n);

ssize_t	ft_write(int fd, const void* buf, size_t count);
void	ft_printline(int fd, char* s);

#pragma endregion Libft Function Prototypes

#endif /* LIBFT_H */
