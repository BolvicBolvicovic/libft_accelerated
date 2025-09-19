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

typedef __m128i			vector128;
#define Aligned128(a)		(((uintptr)(a) & (sizeof(vector128) - 1)) == 0)
#define Zero128			(_mm_setzero_si128)
// TODO: Delete these two macros as they are not good for performance.
#define GetLoader128(a)		(Aligned128(a) ? load_aligned128 : load_unaligned128)
#define GetStore128(a)		(Aligned128(a) ? store_aligned128 : store_unaligned128)
// ENDTODO
#define Set1_int8(a)		(_mm_set1_epi8(a))
#define CmpEq_int8(a, b)	(_mm_cmpeq_epi8(a, b))
#define Min_uint8(a, b)		(_mm_min_epu8(a, b))
#define MoveMask_uint8(a)	(_mm_movemask_epi8(a))

#endif // __SSE2__

#if defined(__AVX__)

typedef __m256i			vector256;
#define Aligned256(a)		(((uintptr)(a) & (sizeof(vector256) - 1)) == 0)
#define Zero256			(_mm256_setzero_si256)
#define GetLoader256(a)		(Aligned256(a) ? load_aligned256 : load_unaligned256)
#define GetStore256(a)		(Aligned256(a) ? store_aligned256 : store_unaligned256)
#define Set1_int8_256(a)	(_mm256_set1_epi8(a))
#define CmpEq_int8_256(a, b)	(_mm256_cmpeq_epi8(a, b))
#define MoveMask_uint8_256(a)	(_mm256_movemask_epi8(a))

#endif // __AVX__

#endif // Defined architecture x64

#if defined(__aarch64__) || defined(_M_ARM64)

#include <arm_neon.h>

#endif // Defined architecture ARM64

// Note: Returns the number of trailing 0-bytes in x, starting at the least significant bit position.
#define ByteScanForward(a)	(BitScanForward(a) / ByteSize)

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
typedef long int		ssize_t;
typedef __int128_t		uint128;
typedef _Bool			bool;

#define true			1
#define false			0


static const size_t		ByteSize = 8;
static const size_t		UCharMax = 255;
static const size_t		IntMin = -2147483648;
static const size_t		IntMax = 2147483647;
static const size_t		SizeTMax = ((size_t)-1);
static const size_t		SSizeTMin = (ssize_t)-1;


static const size_t		WordSize = (sizeof(word));
static const size_t		WordAlignSize = (WordSize - 1);
#define WordAligned(a)		((uintptr)(a) & WordAlignSize == 0)

// Note: 0x01010101... for size_t
static const size_t	LSB = ((size_t)-1 / UCharMax);
// Note: 0x80808080... for size_t
static const size_t	MSB = (LSB * (UCharMax / 2 + 1));
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

// Find all zeros in a word
function __always_inline  word
AllZeros(word x)
{
	word m = (RepeatByteInWord(0x7f));
	return ~(((x & m) + m) | x | m);
}

#define WordAlignDown(a)	((__typeof__(a)) ((uintptr)(a) & -WordSize))

#define	ShiftFind(a, s_int)	((a) >> (ByteSize * ((s_int) % WordSize)))

#if defined(__SSE2__) || defined(__aarch64__) || defined(_M_ARM64)
function __always_inline vector128
load_aligned128(const vector128* ptr)
{
	return _mm_load_si128(ptr);
}

function __always_inline vector128
load_unaligned128(const vector128* ptr)
{
	return _mm_loadu_si128(ptr);
}

function __always_inline void
store_aligned128(vector128* ptr, vector128 a)
{
	_mm_store_si128(ptr, a);
}

function __always_inline void
store_unaligned128(vector128* ptr, vector128 a)
{
	_mm_storeu_si128(ptr, a);
}

#endif // defined(__SSE2__) || defined(__aarch64__) || defined(_M_ARM64)

#if defined(__AVX__)
function __always_inline vector256
load_aligned256(const vector256* ptr)
{
	return _mm256_load_si256(ptr);
}

function __always_inline vector256
load_unaligned256(const vector256* ptr)
{
	return _mm256_loadu_si256(ptr);
}

function __always_inline void
store_aligned256(vector256* ptr, vector256 a)
{
	_mm256_store_si256(ptr, a);
}

function __always_inline void
store_unaligned256(vector256* ptr, vector256 a)
{
	_mm256_storeu_si256(ptr, a);
}
#endif // defined(__AVX__)

function __always_inline bool
ft_isalpha(uint8 c)
{
	// A = 65; a = 97; alphabet size is 25;
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25);
}

function __always_inline bool
ft_isdigit(uint8 c)
{
	// '0' = 48;
	return (uint8)(c - 48) <= 9;
}

function __always_inline bool
ft_isalnum(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25) |
		(uint8)(c - 48) <= 9;
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
	const char* 	start = s;
	vector128	zero = Zero128();
	vector128*	vs;
	uint32		mask0;
	uint32		mask1;
	uint32		mask2;
	uint32		mask3;
	word*		w_ptr = (word*)WordAlignDown(s);
	word		mask = ShiftFind(AllZeros(*w_ptr), (const uintptr)s);


	if (mask)
	{
		return ByteScanForward(mask);
	}

	while (!(mask = FirstZero(*++w_ptr)) && (uintptr)w_ptr & 15);

	if (mask)
	{
		 return ((const char*)w_ptr + ByteScanForward(mask) - s);
	}

	vs = (vector128*)w_ptr;

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
			return s - start + BitScanForward(mask);
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
			return s - start + BitScanForward(mask);
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
	void*	SStart = s;
	void*	SEnd = s + n;

	void	(*store)(vector128*, vector128) = GetStore128(s);
	vector128	Value = Set1_int8((uint8)c);
	void*	Bulk = s + ((n / 16) * 16);

	while (s < Bulk)
	{
		store((vector128*)s, Value);
		s += 16;
	}

	// Note: Add tail
	while (s < SEnd)
	{
		*(uint8*)s++ = (uint8)c;
	}

	return SStart;
}

function void
ft_bzero(void* s, size_t n)
{
	void	(*store)(vector128*, vector128) = GetStore128(s);
	vector128	Value = Zero128();
	void*	SStart = s;
	void*	SEnd = s + n;
	void*	Bulk = s + ((n / 16) * 16);

	while (s < Bulk)
	{
		store((vector128*)s, Value);
		s += 16;
	}

	// Note: Erase tail
	while (s < SEnd)
	{
		*(uint8*)s++ = 0;
	}
}

function void*
ft_memcpy(void* dst, const uint8 *src, size_t n)
{
	if (dst == src)
	{
		return dst;
	}

	vector128	(*load)(const vector128*);
	void	(*store)(vector128*, vector128);
	void*	DstStart = dst;
	void*	DstEnd = dst + n;
	void*	Bulk = dst + ((n / 16) * 16);

	if (Aligned128(dst) && Aligned128(src))
	{
		load = load_aligned128;
		store = store_aligned128;
	} else {
		load = load_unaligned128;
		store = store_unaligned128;
	}

	while (dst < Bulk)
	{
		vector128	Value = load((const vector128*)src);
		store((vector128*)dst, Value);
		dst += 16;
		src += 16;
	}

	while (dst < DstEnd)
	{
		*(uint8*)dst++ = *src++;
	}

	return DstStart;
}

function void*
ft_memmove(void* dst, const uint8* src, size_t n)
{
	if ((uint8*)dst <= src || (uint8*)dst >= src + n)
	{
		return ft_memcpy(dst, src, n);
	}

	uint8*	SrcEnd = (uint8*)src + n;
	uint8*	DstEnd = (uint8*)dst + n;
	vector128	(*load)(const vector128*);
	void	(*store)(vector128*, vector128);
	uint8*	Bulk = DstEnd - ((n / 16) * 16);

	if (Aligned128(SrcEnd) && Aligned128(DstEnd))
	{
		load = load_aligned128;
		store = store_aligned128;
	} else {
		load = load_unaligned128;
		store = store_unaligned128;
	}

	while (DstEnd > Bulk)
	{
		DstEnd -= 16;
		SrcEnd -= 16;
		vector128	Value = load((const vector128*)SrcEnd);
		store((vector128*)DstEnd, Value);
	}

	while (DstEnd > (uint8*)dst)
	{
		*--DstEnd = *--SrcEnd;
	}

	return dst;
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
	size_t		SrcLen = ft_strlen(src);

	if (size)
	{
		size_t		ToCopy = SrcLen < size ? SrcLen : size - 1;
		ft_memcpy(dst, src, ToCopy);
		dst[ToCopy] = 0;
	}

	return SrcLen;
}
	
function size_t
ft_strlcat(char* dst, const char* src, size_t size)
{
	size_t	DstLen = ft_strlen(dst);

	if (DstLen >= size)
	{
		return (size + ft_strlen(src));
	}

	return (DstLen + ft_strlcpy(dst + DstLen, src, size - DstLen));
}

function char*
ft_strchr(char* s, uint8 c)
{
	vector128	(*load)(const vector128*) = GetLoader128(s);
	vector128	zero = Zero128();
	vector128	Value = Set1_int8(c);

	while (true)
	{
		vector128	Chunk = load((vector128*)s);
		vector128	CmpV = CmpEq_int8(Chunk, Value);
		vector128	CmpZ = CmpEq_int8(Chunk, zero);

		if ((uint128)CmpV | (uint128)CmpZ)
		{
			size_t VLen = BitScanForward(MoveMask_uint8(CmpV));
			size_t ZLen = BitScanForward(MoveMask_uint8(CmpZ)); 	

			return VLen <= ZLen ? s + VLen : NULL;
		}

		s += 16;
	}
}

#endif /* LIBFT_H */
