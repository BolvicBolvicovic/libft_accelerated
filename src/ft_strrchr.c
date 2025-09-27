#include "../libft.h"

char*
ft_strrchr(char* s, uint8 c)
{
#if defined(SIMD_SUPPORTED)
	size_t	len = ft_strlen(s);
	if (!c)
	{
		return s + len;
	}
	vector*	vs = (vector*)(s + len - 4 * VecSize);
	vector	zero = Zero();
	vector	value = Set1_int8(c);

	vector	chunk3 = LoadUnaligned(vs + 3);
	vector	chunk2 = LoadUnaligned(vs + 2);
	vector	chunk1 = LoadUnaligned(vs + 1);
	vector	chunk0 = LoadUnaligned(vs);

	chunk3 = Min_uint8(chunk3 ^ value, chunk3);
	chunk2 = Min_uint8(chunk2 ^ value, chunk2);
	chunk1 = Min_uint8(chunk1 ^ value, chunk1);
	chunk0 = Min_uint8(chunk0 ^ value, chunk0);

	uint32	mask = MoveMask_uint8(CmpEq_int8(Min_uint8(Min_uint8(chunk3, chunk2), Min_uint8(chunk1, chunk0)), zero));
#if defined(__AVX2__)
	word	wmask;
#else
	uint32	mask1;
	uint32	mask2;
	uint32	mask3;
#endif

	if (mask)
	{
#if defined(__AVX2__)
		wmask = (uint32)MoveMask_uint8(CmpEq_int8(chunk2, zero)) |  ((word)MoveMask_uint8(CmpEq_int8(chunk3, zero)) << 32);
		if (wmask)
		{
			s += len - BitScanBackward(wmask) - 1;
			return *s == c ? s : 0;
		}
		wmask = ((word)MoveMask_uint8(CmpEq_int8(chunk1, zero)) << 32) | (uint32)MoveMask_uint8(CmpEq_int8(chunk1, zero));
		if (wmask)
		{
			s += len - BitScanBackward(wmask) - 65;
			return *s == c ? s : 0;
		}
#else
		mask = MoveMask_uint8(CmpEq_int8(chunk0, zero));
		mask1 = MoveMask_uint8(CmpEq_int8(chunk1, zero));
		mask2 = MoveMask_uint8(CmpEq_int8(chunk2, zero));
		mask3 = MoveMask_uint8(CmpEq_int8(chunk3, zero));
		s += len - 1 - BitScanBackward(((mask1 << 16) | mask) | ((((word)mask3 << 16) | (word)mask2) << 32));
		return *s == c ? s : 0;
#endif
	}

	vs = (vector*)(((uintptr)vs | (VecSize - 1)) + 1) - 4;

	while (true)
	{
		chunk3 = Min_uint8(*(vs + 3) ^ value, *(vs + 3));
		chunk2 = Min_uint8(*(vs + 2) ^ value, *(vs + 2));
		chunk1 = Min_uint8(*(vs + 1) ^ value, *(vs + 1));
		chunk0 = Min_uint8(*vs ^ value, *vs);

		mask = MoveMask_uint8(CmpEq_int8(Min_uint8(Min_uint8(chunk3, chunk2), Min_uint8(chunk1, chunk0)), zero));
		if (mask)
		{
#if defined(__AVX2__)
			wmask = ((word)MoveMask_uint8(CmpEq_int8(chunk3, zero)) << 32) | (uint32)MoveMask_uint8(CmpEq_int8(chunk2, zero));
			if (wmask)
			{
				s = (char*)vs + 127 - BitScanBackward(wmask);
				return *s == c ? s : 0;
			}
			wmask = ((word)MoveMask_uint8(CmpEq_int8(chunk1, zero)) << 32) | (uint32)MoveMask_uint8(CmpEq_int8(chunk1, zero));
			if (wmask)
			{
				s = (char*)vs + 63 - BitScanBackward(wmask);
				return *s == c ? s : 0;
			}
#else
			mask = MoveMask_uint8(CmpEq_int8(chunk0, zero));
			mask1 = MoveMask_uint8(CmpEq_int8(chunk1, zero));
			mask2 = MoveMask_uint8(CmpEq_int8(chunk2, zero));
			mask3 = MoveMask_uint8(CmpEq_int8(chunk3, zero));
			s = (char*)vs + 63 - BitScanBackward((((word)mask1 << 16) | (word)mask) | ((((word)mask3 << 16) | (word)mask2) << 32));
			return *s == c ? s : 0;
#endif
		}

		vs += 4;
	}
#else
	const uint8*	str = (void*)s;
	uintptr			pos = -1;

	while (*str)
	{
		if (*str == c)
		{
			pos = (uintptr)str - (uintptr)s;
		}
		str++;
	}

	if (*str == c)
	{
		return (void*)str;
	}
	
	return pos == (uintptr)-1 ? 0 : s + pos;
#endif
}