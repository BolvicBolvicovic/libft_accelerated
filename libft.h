#ifndef LIBFT_H
#define LIBFT_H

#define function static

typedef unsigned long 	uintptr_t;
typedef unsigned char 	uint8;
typedef long int		ssize_t;
typedef __int128_t	uint128;
typedef _Bool		bool;

#define true 1
#define false 0

#define INT_MIN (-2147483648)
#define INT_MAX 2147483647
#define SIZE_T_MAX ((size_t)-1)
#define SSIZE_T_MIN ((ssize_t)-1)

#include <emmintrin.h>
#include <immintrin.h>

// Note: All SIMD operations are currently 16 bytes wide.
#define Aligned(a) (((uintptr_t)(a) & (sizeof(__m128i) - 1)) == 0)
#define Zero (_mm_setzero_si128)
#define GetLoader(a) (Aligned(a) ? load_aligned : load_unaligned)
#define GetStore(a) (Aligned(a) ? store_aligned : store_unaligned)
#define Set1_uint8(a) (_mm_set1_epi8(a))
#define CmpEq_uint8(a, b) (_mm_cmpeq_epi8(a, b))
#define MoveMask_uint8(a) (_mm_movemask_epi8(a))
// Note: Returns the number of trailing 0-bits in x, starting at the least significant bit position.
#define BitScanForward(a) (_tzcnt_u32(a))

extern int* __errno_location(void);


// Note: Function wrappers necessary for aligned/unaligned selection 
function inline __m128i
load_aligned(const __m128i* ptr)
{
	return _mm_load_si128(ptr);
}

function inline __m128i
load_unaligned(const __m128i* ptr)
{
	return _mm_loadu_si128(ptr);
}

function inline void
store_aligned(__m128i* ptr, __m128i a)
{
	_mm_store_si128(ptr, a);
}

function inline void
store_unaligned(__m128i* ptr, __m128i a)
{
	_mm_storeu_si128(ptr, a);
}

function inline bool
ft_isalpha(uint8 c)
{
	// A = 65; a = 97; alphabet size is 25;
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25);
}

function inline bool
ft_isdigit(uint8 c)
{
	// '0' = 48;
	return (uint8)(c - 48) <= 9;
}

function inline bool
ft_isalnum(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25) |
		(uint8)(c - 48) <= 9;
}

function inline bool
ft_isascii(uint8 c)
{
	return (uint8)c <= 127;
}

function inline bool
ft_isprint(uint8 c)
{
	return (uint8)(c - 32) <= 94;
}

function inline uint8
ft_toupper(uint8 c)
{
	return ((uint8)(c - 97) <= 25) ? (uint8)c - 32 : c;
}

function inline uint8
ft_tolower(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) ? (uint8)c + 32 : c;
}

function inline ssize_t
ft_write(int fd, const void* buf, size_t count)
{
    ssize_t ret;

    asm volatile (
		"syscall"						// Notes:
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
	__m128i			(*load)(const __m128i*) = GetLoader(s);
	const __m128i	zero = Zero();
	char*			tmp = (char*)s;

	while (true)
	{
		__m128i	chunk = load((const __m128i*)tmp);
		__m128i	cmp = CmpEq_uint8(chunk, zero);

		if ((uint128)cmp)
		{
	// Note: The mask is the compression of the boolean results into 16 bits that can be used by BitScanForward.
			return tmp - s + BitScanForward(MoveMask_uint8(cmp));
		}
		tmp += 16;
	}	
}

function void*
ft_memset(void* s, uint8 c, size_t n)
{
	void*	SStart = s;
	void*	SEnd = s + n;

	void	(*store)(__m128i*, __m128i) = GetStore(s);
	__m128i	Value = Set1_uint8((uint8)c);
	void*	Bulk = s + ((n / 16) * 16);

	while (s < Bulk)
	{
		store((__m128i*)s, Value);
		s += 16;
	}

	// Note: Add tail
	while (s < SEnd)
	{
		*(uint8*)s++ = (uint8)c;
	}
}

function void
ft_bzero(void* s, size_t n)
{
	void	(*store)(__m128i*, __m128i) = GetStore(s);
	__m128i	Value = Zero();
	void*	SStart = s;
	void*	SEnd = s + n;
	void*	Bulk = s + ((n / 16) * 16);

	while (s < Bulk)
	{
		store((__m128i*)s, Value);
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

	__m128i	(*load)(const __m128i*);
	void	(*store)(__m128i*, __m128i);
	void*	DstStart = dst;
	void*	DstEnd = dst + n;
	void*	Bulk = dst + ((n / 16) * 16);

	if (Aligned(dst) && Aligned(src))
	{
		load = load_aligned;
		store = store_aligned;
	} else {
		load = load_unaligned;
		store = store_unaligned;
	}

	while (dst < Bulk)
	{
		__m128i	Value = load((const __m128i*)src);
		store((__m128i*)dst, Value);
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
	__m128i	(*load)(const __m128i*);
	void	(*store)(__m128i*, __m128i);
	uint8*	Bulk = DstEnd - ((n / 16) * 16);

	if (Aligned(SrcEnd) && Aligned(DstEnd))
	{
		load = load_aligned;
		store = store_aligned;
	} else {
		load = load_unaligned;
		store = store_unaligned;
	}

	while (DstEnd > Bulk)
	{
		DstEnd -= 16;
		SrcEnd -= 16;
		__m128i	Value = load((const __m128i*)SrcEnd);
		store((__m128i*)DstEnd, Value);
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

	if (n == INT_MIN)
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
		return (DstLen + ft_strlen(src));
	}

	return (DstLen + ft_strlcpy(dst + DstLen, src, size - DstLen));
}

function char*
ft_strchr(char* s, uint8 c)
{
	__m128i	(*load)(const __m128i*) = GetLoader(s);
	__m128i	zero = Zero();
	__m128i	Value = Set1_uint8(c);

	while (true)
	{
		__m128i	Chunk = load((__m128i*)s);
		__m128i	CmpV = CmpEq_uint8(Chunk, Value);
		__m128i	CmpZ = CmpEq_uint8(Chunk, zero);

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
