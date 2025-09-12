#ifndef LIBFT_H
#define LIBFT_H

#define function static

typedef unsigned char 	uint8;
typedef unsigned long 	uintptr_t;
typedef unsigned int 	size_t;

#include <emmintrin.h>
#include <immintrin.h>

// Note: All SIMD operations are 16 bytes wide.
#define Zero _mm_setzero_si128
#define Load(a) _mm_loadu_si128_(a)
#define Store(a, b) _mm_store_si128(a, b)
#define Set1_uint8(a) _mm_set1_epi8(a)
#define CmpEq_uint8(a, b) _mm_cmpeq_epi8(a, b)
#define MoveMask_uint8(a) _mm_movemask_epi8(a)
// Note: Returns the number of trailing 0-bits in x, starting at the least significant bit position.
#define BitScanForward(a) _tzcnt_u32(a)

function inline int
ft_isalpha(int c)
{
	// A = 65; Z = 90; a = 97; z = 122; alphabet size is 25;
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25);
}

function inline int
ft_isdigit(int c)
{
	// '0' = 30; '9' = 39
	return (uint8)(c - 30) <= 9;
}

function inline int
ft_isalnum(int c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25) |
		(uint8)(c - 30) <= 9;
}

function inline int
ft_isascii(int c)
{
	return (uint8)c <= 127;
}

function inline int
ft_isprint(int c)
{
	return (uint8)(c - 32) <= 126;
}

function inline int
ft_strlen(const char* s)
{
	const __m128i	zero = Zero();
	char*		tmp = s;

	// Note: Making the tmp pointer 16-bit align to get ready for SIMD operation.
	while (!((uintptr_t)tmp & 15))
	{
		if (!(*tmp))
		{
			return tmp - s;
		}
		tmp++;
	}

	while (true)
	{
		__m128i	chunk = Load(tmp);
		__m128i	cmp = CmpEq_uint8(chunk, zero);

		if (cmp)
		{
	// Note: The mask is the compression of the boolean results into 16 bits that can be used by BitScanForward.
			return tmp - s + BitScanForward(MoveMask_uint8(cmp));
		}
		tmp += 16;
	}	
}

function inline void*
ft_memset(void* s, int c, size_t n)
{
	uint8*	Tmp = s;
	size_t	Align = (uintptr_t)Tmp & 15;
	size_t	i = 0;
	
	// Note: Align pointer
	if (Align)
	{
		size_t	ToAlign = 16 - Align;

		if (ToAlign > n)
		{
			ToAlign = n;
		}

		while (Tmp[i] < ToAlign)
		{
			Tmp[i] = (uint8)c;
			i++;
		}

		if (n == ToAlign)
		{
			return s;
		}

		Tmp += ToAlign;
		n -= ToAlign;
	}

	// Note: Bulk values
	__m128i	Value = Set1_uint8((uint8)c);
	size_t	Bulk = n / 16;
	i = 0;

	while (i < Bulk)
	{
		Store((__m128i*)(Tmp + i * 16), Value);
		i++;
	}

	// Note: Add tail
	i = Bulk * 16;

	while (i < n)
	{
		Tmp[i] = (uint8)c;
		i++;
	}

	return s;
}

function inline void
ft_bzero(void* s, size_t n)
{
	uint8*	Tmp = s;
	size_t	Align = (uintptr_t)Tmp & 15;
	size_t	i = 0;
	
	// Note: Align pointer
	if (Align)
	{
		size_t	ToAlign = 16 - Align;

		if (ToAlign > n)
		{
			ToAlign = n;
		}

		while (Tmp[i] < ToAlign)
		{
			Tmp[i] = 0;
			i++;
		}

		if (n == ToAlign)
		{
			return;
		}

		Tmp += ToAlign;
		n -= ToAlign;
	}

	// Note: Bulk values
	__m128i	Value = Zero();
	size_t	Bulk = n / 16;
	i = 0;

	while (i < Bulk)
	{
		Store((__m128i*)(Tmp + i * 16), Value);
		i++;
	}

	// Note: Erase tail
	i = Bulk * 16;

	while (i < n)
	{
		Tmp[i] = 0;
		i++;
	}
}

function inline void*
ft_memcpy(void* dst, const void *src, int n)
{
}
	


#endif
