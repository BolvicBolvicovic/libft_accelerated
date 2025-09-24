#ifndef LIBFT_H
#define LIBFT_H

#define function static

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)

#include <emmintrin.h>
#include <immintrin.h>

// Note: Returns the number of trailing 0-bits in x, starting at the least significant bit position.
#if defined(__BMI__)
#define BitScanForward(a)	(_tzcnt_u64(a))
#endif


#if defined(__SSE2__)

typedef __m128i				vector;
#define	VecSize				(sizeof(vector))
#define Zero				(_mm_setzero_si128)
// Note: Set1 is actually a sequence of instruction.
// 1. Loads the char into vector register	"c"
// 2. Duplicate it into a 16-bits word		"cc"
// 3. Duplicate it into a 32-bits word		"cccc"
// 4. Select 32-bits word for each slots, 	"cccccccc"
// 	(32 * 4 = 128) and shuffle them in.
#define Set1_int8(a)		(_mm_set1_epi8(a))
#define CmpEq_int8(a, b)	(_mm_cmpeq_epi8(a, b))
#define Min_uint8(a, b)		(_mm_min_epu8(a, b))
#define MoveMask_uint8(a)	(_mm_movemask_epi8(a))

#elif defined(__AVX__)

typedef __m256i			vector;
#define	VecSize			(sizeof(vector))
#define Zero			(_mm256_setzero_si256)
#define Set1_int8(a)		(_mm256_set1_epi8(a))
#define CmpEq_int8(a, b)	(_mm256_cmpeq_epi8(a, b))
#define MoveMask_uint8(a)	(_mm256_movemask_epi8(a))

#endif // __AVX__

#endif // Defined architecture x64

#if defined(__aarch64__) || defined(_M_ARM64)

#include <arm_neon.h>

#endif // Defined architecture ARM64

// Note: Returns the number of trailing 0-bytes in x, starting at the least significant bit position.
#define ByteScanForward(a)	(BitScanForward(a) / ByteSize)
#define Aligned(a)		(((uintptr)(a) & (VecSize - 1)) == 0)
#define GetLoader(a)		(Aligned(a) ? load_aligned : load_unaligned)
#define GetStore(a)		(Aligned(a) ? store_aligned : store_unaligned)

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
typedef size_t			word;

#define __always_inline		inline

#endif

typedef unsigned long 		uintptr;
typedef unsigned char 		uint8;
typedef unsigned		uint32;
typedef __uint16_t		uint16;	
typedef long int		ssize_t;
typedef __int128_t		uint128;
typedef _Bool			bool;

#define true			1
#define false			0

static const size_t		ByteSize = 8;
static const size_t		UCharMax = 255;
static const int		IntMin = -2147483648;
static const int		IntMax = 2147483647;
static const size_t		SizeTMax = ((size_t)-1);
static const size_t		SSizeTMin = (ssize_t)-1;

static const size_t		WordSize = (sizeof(word));
static const size_t		WordAlignSize = (WordSize - 1);
#define WordAligned(a)		((uintptr)(a) & WordAlignSize == 0)

// Note: 0x01010101... for size_t
static const size_t		LSB = ((size_t)-1 / UCharMax);
// Note: 0x80808080... for size_t
static const size_t		MSB = (LSB * (UCharMax / 2 + 1));
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
function __always_inline word
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
function __always_inline  word
AllZeros(word x)
{
	// Note: Masking MSB => 0x7f == 0b01111111
	static const word	m = (RepeatByteInWord(0x7f));

	return ~(((x & m) + m) | x | m);
}

#define WordAlignDown(a)	((__typeof__(a)) ((uintptr)(a) & -WordSize))

#define	ShiftFind(a, s_int)	((a) >> (ByteSize * ((s_int) % WordSize)))

#if defined(__SSE2__) || defined(__AVX__) || defined(__aarch64__) || defined(_M_ARM64)
function __always_inline vector
load_aligned(const vector* ptr)
{
#if defined(__SSE2__)
	return _mm_load_si128(ptr);
#elif defined(__AVX__)
	return _mm256_load_si256(ptr);
#endif
}

function __always_inline vector
load_unaligned(const vector* ptr)
{
#if defined(__SSE2__)
	return _mm_loadu_si128(ptr);
#elif defined(__AVX__)
	return _mm256_loadu_si256(ptr);
#endif
}

function __always_inline void
store_aligned(vector* ptr, vector a)
{
#if defined(__SSE2__)
	_mm_store_si128(ptr, a);
#elif defined(__AVX__)
	_mm256_store_si256(ptr, a);
#endif
}

function __always_inline void
store_unaligned(vector* ptr, vector a)
{
#if defined(__SSE2__)
	_mm_storeu_si128(ptr, a);
#elif defined(__AVX__)
	_mm256_storeu_si256(ptr, a);
#endif
}
#endif // defined(__SSE2__) defined(__AVX__) || defined(__aarch64__) || defined(_M_ARM64)

function __always_inline bool
ft_isalpha(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25);
}

function __always_inline bool
ft_isdigit(uint8 c)
{
	return (uint8)(c - 48) <= 9;
}

function __always_inline bool
ft_isalnum(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25) |
		((uint8)(c - 48) <= 9);
}

function __always_inline bool
ft_isascii(uint8 c)
{
	return (uint8)c <= 127;
}

function __always_inline bool
ft_isprint(uint8 c)
{
	return (uint8)(c - 32) <= 94;
}

function __always_inline uint8
ft_toupper(uint8 c)
{
	return ((uint8)(c - 97) <= 25) ? (uint8)c - 32 : c;
}

function __always_inline uint8
ft_tolower(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) ? (uint8)c + 32 : c;
}

extern int* __errno_location(void);

function __always_inline ssize_t
ft_write(int fd, const void* buf, size_t count)
{
	ssize_t ret;

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
	asm volatile (
		"syscall"			// Notes:
		: "=a" (ret)                    // Output: rax -> ret
		: "a" (1),                      // Input: write syscall number in rax
		  "D" (fd),                     // Input: fd in rdi  
		  "S" (buf),                    // Input: buf in rsi
		  "d" (count)                   // Input: count in rdx
		: "rcx", "r11", "memory"        // Clobbered by syscall:
						// RCX: Used internally by syscall to store return address
						// R11: Gets the RFLAGS register value
	);
#elif defined(__aarch64__) || defined(_M_ARM64)
    asm volatile (
        "svc #0"
        : "=r" (ret)
        : "r" (fd), "r" (buf), "r" (count), "r" (64)
        : "memory"
    );
#endif

	if (ret < 0)
	{
		*__errno_location() = -ret;
		return -1;
	}
	
	return ret;
}

function size_t
ft_strlen(const char* s)
{
#if defined(__SSE2__)
	const char* start = s;
	vector		zero = Zero();
	vector*		vs = (vector*)s;
	uint32		mask0;
	uint32		mask1;
	uint32		mask2;
	uint32		mask3;
	word		mask;

	if (!s || !*s)
	{
		return 0;
	}

	mask0 = MoveMask_uint8(CmpEq_int8(zero, load_unaligned(vs)));

	if (mask0)
	{
		return BitScanForward(mask0);
	}

	vs++;
	vs = (vector*)((uintptr)vs & -VecSize);

	while (true)
	{
		mask0 = MoveMask_uint8(CmpEq_int8(zero, Min_uint8(Min_uint8(vs[0], vs[1]), Min_uint8(vs[2], vs[3]))));

		if (mask0)
		{
			mask0 = MoveMask_uint8(CmpEq_int8(vs[0], zero));
			mask1 = MoveMask_uint8(CmpEq_int8(vs[1], zero));
			mask2 = MoveMask_uint8(CmpEq_int8(vs[2], zero));
			mask3 = MoveMask_uint8(CmpEq_int8(vs[3], zero));
			mask  = (((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32);
			return (const char*)vs - start + BitScanForward(mask);
		}

		vs += 4;
		mask0 = MoveMask_uint8(CmpEq_int8(zero, Min_uint8(Min_uint8(vs[0], vs[1]), Min_uint8(vs[2], vs[3]))));

		if (mask0)
		{
			mask0 = MoveMask_uint8(CmpEq_int8(vs[0], zero));
			mask1 = MoveMask_uint8(CmpEq_int8(vs[1], zero));
			mask2 = MoveMask_uint8(CmpEq_int8(vs[2], zero));
			mask3 = MoveMask_uint8(CmpEq_int8(vs[3], zero));
			mask  = (((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32);
			return (const char*)vs - start + BitScanForward(mask);
		}
		vs += 4;
	}
#else
	word*	w_ptr = (word*)WordAlignDown(s);
	word	mask = ShiftFind(AllZeros(*w_ptr), (const uintptr)s);

	if (mask)
	{
		return ByteScanForward(mask);
	}

	while (!(mask = FirstZero(*++w_ptr)));

	return ((const char*)w_ptr + ByteScanForward(mask) - s);

#endif
}

function void*
ft_memset(void* s, uint8 c, size_t n)
{
#if defined(__SSE2__)
	void*		start = s;
	vector*	end = (vector*)(s + n);
	vector*	vs = (vector*)s;
	vector	value = Set1_int8(c);
	
	if (n < VecSize)
	{
		word	svalue = RepeatByteInWord(c);

		if (n >= 8)
		{
			*(word*)s = svalue;
			*(word*)(s - 8 + n) = svalue;
			return s;
		}
		else if (n >= 4)
		{
			*(uint32*)s = (uint32)svalue;
			*(uint32*)(s - 4 + n) = (uint32)svalue;
			return s;
		}
		else if (n >= 2)
		{
			*(uint16*)s = (uint16)svalue;
			*(uint8*)(s - 1 + n) = c;
			return s;
		}
		else if (n == 1)
		{
			*(uint8*)s = c;
			return s;
		}
		
		return s;

	}

	if (n == VecSize)
	{
		store_unaligned(vs, value);
		return s;
	}

	store_unaligned(vs, value);
	store_unaligned(vs + 1, value);

	if (n <= VecSize * 4)
	{
		store_unaligned(end - 1, value);
		store_unaligned(end - 2, value);
		return start;
	}
	
	store_unaligned(vs + 2, value);
	store_unaligned(vs + 3, value);

	vs += 4;
	end -= 4; 

	if (n <= VecSize * 8)
	{
		goto FourLastVectors;
	}

	// Note: Align down s and since we already copied the previous bytes,
	// it does not matter to overwrite them.
	vs = (vector*)((uintptr)vs & -VecSize);

	do
	{
		store_aligned(vs, value);
		store_aligned(vs + 1, value);
		store_aligned(vs + 2, value);
		store_aligned(vs + 3, value);
		vs += 4;
	}
	while (vs < end);

FourLastVectors:
	store_unaligned(end, value);
	store_unaligned(end + 1, value);
	store_unaligned(end + 2, value);
	store_unaligned(end + 3, value);
	return start;


#else
	uint8*	ptr;
	word	wc = RepeatByteInWord(c);

	if (!n)
	{
		return s;
	}

	*(uint8*)s = c;
	*((uint8*)s + n - 1) = c;

	if (n <= 2)
	{
		return s;
	}

	*(uint16*)(s + 1) = (uint16)wc;
	*(uint16*)(s + n - 3) = (uint16)wc;

	if (n <= 6)
	{
		return s;
	}

	*((uint8*)s + 3) = c;
	*((uint8*)s + n - 4) = c;
	
	if (n <= 8)
	{
		return s;
	}

	// Note: Same as for SS2, aligning pointer.
	ptr = (s | 3) + 1;
	// Note: Truncate n to a multiple of 4 to not overflow.
	n &= -4;

	*(uint32*)(ptr) = (uint32)wc;
	*(uint32*)(ptr + n - 4) = (uint32)wc;

	if (n <= 8)
	{
		return s;
	}
	
	*(word*)(ptr + 4) = wc;
	*(word*)(ptr + n - 12) = wc;

	if (n <= 24)
	{
		return s;
	}

	*(word*)(ptr + 12) = wc;
	*(word*)(ptr + 20) = wc;
	*(word*)(ptr + n - 20) = wc;
	*(word*)(ptr + n - 28) = wc;

	size_t	m = 24 + ((uintptr)s & 4);
	ptr += m;
	n -= m;

	do
	{
		*(word*)(ptr) = wc;
		*(word*)(ptr + 8) = wc;
		*(word*)(ptr + 16) = wc;
		*(word*)(ptr + 24) = wc;

		n -= 32;
		ptr += 32;

	} while (n >= 32);
	
	return s;

#endif
}

function void
ft_bzero(void* s, size_t n)
{
	ft_memset(s, 0, n);
}

function void*
ft_memcpy(void* dst, const char* src, size_t n)
{
#if defined(__SSE2__)
	if (n < VecSize)
	{
		if (n >= 8)
		{
			*(word*)dst = *(word*)src;
			*(word*)(dst - 8 + n) = *(word*)(src - 8 + n);
			return dst;
		}
		else if (n >= 4)
		{
			*(uint32*)dst = *(uint32*)src;
			*(uint32*)(dst - 4 + n) = *(uint32*)(src - 4 + n);
			return dst;
		}
		else if (n >= 2)
		{
			*(uint16*)dst = *(uint16*)src;
			*(uint8*)(dst - 1 + n) = *(src - 1 + n);
			return dst;
		}
		else if (n == 1)
		{
			*(uint8*)dst = *src;
			return dst;
		}
		
		return dst;
	}

	if (n <= VecSize * 2)
	{
		store_unaligned((vector*)dst, load_unaligned((vector*)src));
		store_unaligned((vector*)(dst - VecSize + n), load_unaligned((vector*)(src - VecSize + n)));
		return dst;
	}

	if (n <= VecSize * 4)
	{
		store_unaligned((vector*)dst, load_unaligned((vector*)src));
		store_unaligned((vector*)(dst + VecSize), load_unaligned((vector*)(src + VecSize)));
		store_unaligned((vector*)(dst - VecSize + n), load_unaligned((vector*)(src - VecSize + n)));
		store_unaligned((vector*)(dst - 2 * VecSize + n), load_unaligned((vector*)(src - 2 * VecSize + n)));
		return dst;
	}

	if (n <= VecSize * 8)
	{
		store_unaligned((vector*)dst, load_unaligned((vector*)src));
		store_unaligned((vector*)(dst + VecSize), load_unaligned((vector*)(src + VecSize)));
		store_unaligned((vector*)(dst + 2 * VecSize), load_unaligned((vector*)(src + 2 * VecSize)));
		store_unaligned((vector*)(dst + 3 * VecSize), load_unaligned((vector*)(src + 3 * VecSize)));
		store_unaligned((vector*)(dst - VecSize + n), load_unaligned((vector*)(src - VecSize + n)));
		store_unaligned((vector*)(dst - 2 * VecSize + n), load_unaligned((vector*)(src - 2 * VecSize + n)));
		store_unaligned((vector*)(dst - 3 * VecSize + n), load_unaligned((vector*)(src - 3 * VecSize + n)));
		store_unaligned((vector*)(dst - 4 * VecSize + n), load_unaligned((vector*)(src - 4 * VecSize + n)));
		return dst;
	}

	size_t	span = (uintptr)dst - (uintptr)src;
	if (!span)
	{
		return dst;
	}

	if (span >= n || ((span + n) ^ span) >> 63)
	{
// Forward
		vector	head  = load_unaligned((vector*)src);
		vector*	dst_ptr = (vector*)(((uintptr)dst | (VecSize - 1)) + 1);
		vector*	vsrc  = (vector*)((uintptr)src + (uintptr)dst_ptr - (uintptr)dst);
		vector*	src_tail_ptr = (vector*)(src + n - VecSize * 4);
		vector	tail0 = load_unaligned(src_tail_ptr);
		vector	tail1 = load_unaligned(src_tail_ptr + 1);
		vector	tail2 = load_unaligned(src_tail_ptr + 2);
		vector	tail3 = load_unaligned(src_tail_ptr + 3);

		do
		{
			*dst_ptr = load_unaligned(vsrc); 
			*(dst_ptr + 1) = load_unaligned(vsrc + 1);
			*(dst_ptr + 2) = load_unaligned(vsrc + 2);
			*(dst_ptr + 3) = load_unaligned(vsrc + 3);

			vsrc += 4;
			dst_ptr += 4;
		}
		while (vsrc < src_tail_ptr);

		dst_ptr = (vector*)(dst + n - VecSize * 4);
		store_unaligned((vector*)dst, head);
		store_unaligned(dst_ptr, tail0);
		store_unaligned(dst_ptr + 1, tail1);
		store_unaligned(dst_ptr + 2, tail2);
		store_unaligned(dst_ptr + 3, tail3);
	}
	else
	{
// Backward
		vector	tail  = load_unaligned((vector*)(src - VecSize + n));
		vector* dst_ptr = (vector*)(((uintptr)(dst + n - VecSize * 4 - 1) & -VecSize));
		vector* vsrc = (vector*)((uintptr)src + (uintptr)dst_ptr - (uintptr)dst);
		vector*	src_head_ptr = (vector*)(src);
		vector	head0 = load_unaligned(src_head_ptr);
		vector	head1 = load_unaligned(src_head_ptr + 1);
		vector	head2 = load_unaligned(src_head_ptr + 2);
		vector	head3 = load_unaligned(src_head_ptr + 3);

		do
		{
			*dst_ptr = load_unaligned(vsrc); 
			*(dst_ptr + 1) = load_unaligned(vsrc + 1);
			*(dst_ptr + 2) = load_unaligned(vsrc + 2);
			*(dst_ptr + 3) = load_unaligned(vsrc + 3);

			vsrc -= 4;
			dst_ptr -= 4;
		}
		while (vsrc > src_head_ptr);

		dst_ptr = (vector*) dst;
		store_unaligned((vector*)(dst - VecSize + n), tail);
		store_unaligned(dst_ptr, head0);
		store_unaligned(dst_ptr + 1, head1);
		store_unaligned(dst_ptr + 2, head2);
		store_unaligned(dst_ptr + 3, head3);
	}

	return dst;
#else
	// TODO: Write no SIMD version
#endif
}

function void*
ft_memmove(void* dst, const char* src, size_t n)
{
	return ft_memcpy(dst, src, n);
}

function inline void
ft_printline(int fd, char* s)
{
	ft_write(fd, s, ft_strlen(s));
}

function char*
ft_itoa(char dst[12], int n)
{
	size_t	Len = n == 0 ? 1 : 0;

	if (n == IntMin)
	{
		dst[0] = '-';
		dst[1] = '2';
		n = 147483648;
		Len += 2;
	}

	bool	Negative = n < 0;
	size_t	UnsignedN = Negative ? -n : n;
	size_t	UnsignedNTmp = UnsignedN;

	while(UnsignedNTmp)
	{
		UnsignedNTmp /= 10;
		Len++;
	}

	dst[Len--] = 0;

	while(UnsignedN)
	{
		dst[Len--] = UnsignedN % 10 + 48;
		UnsignedN /= 10;
	}

	return dst;
}

function size_t
ft_strlcpy(char* dst, const char* src, size_t size)
{
	size_t		src_len = ft_strlen(src);

	if (size)
	{
		size_t		to_copy = src_len < size ? src_len : size - 1;
		ft_memcpy(dst, src, to_copy);
		dst[to_copy] = 0;
	}

	return src_len;
}
	
function size_t
ft_strlcat(char* dst, const char* src, size_t size)
{
	size_t	dst_len = ft_strlen(dst);

	if (dst_len >= size)
	{
		return size;
	}

	return dst_len + ft_strlcpy(dst + dst_len, src, size - dst_len);
}

function char*
ft_strchr(char* s, uint8 c)
{
#if defined(__SSE2__)
	vector*	vs = (vector*)s;
	vector	zero = Zero();
	vector	value = Set1_int8(c);

	vector	chunk0 = load_unaligned(vs);
	vector	chunk1 = load_unaligned(vs + 1);
	vector	chunk2 = load_unaligned(vs + 2);
	vector	chunk3 = load_unaligned(vs + 3);

	chunk0 = Min_uint8(chunk0 ^ value, chunk0);
	chunk1 = Min_uint8(chunk1 ^ value, chunk1);
	chunk2 = Min_uint8(chunk2 ^ value, chunk2);
	chunk3 = Min_uint8(chunk3 ^ value, chunk3);

	uint32	mask0 = MoveMask_uint8(CmpEq_int8(Min_uint8(Min_uint8(chunk0, chunk1), Min_uint8(chunk2, chunk3)), zero));
	uint32	mask1;
	uint32	mask2;
	uint32	mask3;

	if (mask0)
	{
		mask0 = MoveMask_uint8(CmpEq_int8(chunk0, zero));
		mask1 = MoveMask_uint8(CmpEq_int8(chunk1, zero));
		mask2 = MoveMask_uint8(CmpEq_int8(chunk2, zero));
		mask3 = MoveMask_uint8(CmpEq_int8(chunk3, zero));
		s = s + BitScanForward((((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32));
		return *s == c ? s : 0;
	}


	vs = (vector*)((uintptr)s & -64) + 4;

	while (true)
	{
		chunk0 = Min_uint8(*vs ^ value, *vs);
		chunk1 = Min_uint8(*(vs + 1) ^ value, *(vs + 1));
		chunk2 = Min_uint8(*(vs + 2) ^ value, *(vs + 2));
		chunk3 = Min_uint8(*(vs + 3) ^ value, *(vs + 3));

		mask0 = MoveMask_uint8(CmpEq_int8(Min_uint8(Min_uint8(chunk0, chunk1), Min_uint8(chunk2, chunk3)), zero));

		if (mask0)
		{
			mask0 = MoveMask_uint8(CmpEq_int8(chunk0, zero));
			mask1 = MoveMask_uint8(CmpEq_int8(chunk1, zero));
			mask2 = MoveMask_uint8(CmpEq_int8(chunk2, zero));
			mask3 = MoveMask_uint8(CmpEq_int8(chunk3, zero));
			s = (char*)vs + BitScanForward((((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32));
			return *s == c ? s : 0;
		}

		vs += 4;
	}
#endif
}

#endif /* LIBFT_H */
