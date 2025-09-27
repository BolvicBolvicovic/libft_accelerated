#include "../libft.h"

char*
ft_strchr(char* s, uint8 c)
{
#if defined(SIMD_SUPPORTED)
	vector*	vs = (vector*)s;
	vector	zero = Zero();
	vector	value = Set1_int8(c);

	vector	chunk0 = LoadUnaligned(vs);
	vector	chunk1 = LoadUnaligned(vs + 1);
	vector	chunk2 = LoadUnaligned(vs + 2);
	vector	chunk3 = LoadUnaligned(vs + 3);

	chunk0 = Min_uint8(chunk0 ^ value, chunk0);
	chunk1 = Min_uint8(chunk1 ^ value, chunk1);
	chunk2 = Min_uint8(chunk2 ^ value, chunk2);
	chunk3 = Min_uint8(chunk3 ^ value, chunk3);

	uint32	mask0 = MoveMask_uint8(CmpEq_int8(Min_uint8(Min_uint8(chunk0, chunk1), Min_uint8(chunk2, chunk3)), zero));
#if defined(__AVX2__)
	word	wmask;
#else
	uint32	mask1;
	uint32	mask2;
	uint32	mask3;
#endif

	if (mask0)
	{
#if defined(__AVX2__)
		wmask = (uint32)MoveMask_uint8(CmpEq_int8(chunk0, zero)) | ((word)MoveMask_uint8(CmpEq_int8(chunk1, zero)) << 32);
		if (wmask)
		{
			s += BitScanForward(wmask);
			return *s == c ? s : 0;
		}
		wmask = (uint32)MoveMask_uint8(CmpEq_int8(chunk2, zero)) | ((word)MoveMask_uint8(CmpEq_int8(chunk3, zero)) << 32);
		if (wmask)
		{
			s += BitScanForward(wmask) + 64;
			return *s == c ? s : 0;
		}
#else
		mask0 = MoveMask_uint8(CmpEq_int8(chunk0, zero));
		mask1 = MoveMask_uint8(CmpEq_int8(chunk1, zero));
		mask2 = MoveMask_uint8(CmpEq_int8(chunk2, zero));
		mask3 = MoveMask_uint8(CmpEq_int8(chunk3, zero));
		s = s + BitScanForward((((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32));
		return *s == c ? s : 0;
#endif
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
#if defined(__AVX2__)
			wmask = (uint32)MoveMask_uint8(CmpEq_int8(chunk0, zero)) | ((word)MoveMask_uint8(CmpEq_int8(chunk1, zero)) << 32);
			if (wmask)
			{
				s = (char*)vs + BitScanForward(wmask);
				return *s == c ? s : 0;
			}
			wmask = (uint32)MoveMask_uint8(CmpEq_int8(chunk2, zero)) | ((word)MoveMask_uint8(CmpEq_int8(chunk3, zero)) << 32);
			if (wmask)
			{
				s = (char*)vs + BitScanForward(mask0) + 64;
				return *s == c ? s : 0;
			}
#else
			mask0 = MoveMask_uint8(CmpEq_int8(chunk0, zero));
			mask1 = MoveMask_uint8(CmpEq_int8(chunk1, zero));
			mask2 = MoveMask_uint8(CmpEq_int8(chunk2, zero));
			mask3 = MoveMask_uint8(CmpEq_int8(chunk3, zero));
			s = (char*)vs + BitScanForward((((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32));
			return *s == c ? s : 0;
#endif
		}

		vs += 4;
	}
#else
	while (*s && *(uint8*)s != (uint8)c) { s++; }
	return *s == c ? s : 0;
#endif
}
