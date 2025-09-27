#include "../libft.h"

size_t
ft_strlen(const char* s)
{
#if defined(SIMD_SUPPORTED)
	const char* start = s;
	vector		zero = Zero();
	vector*		vs = (vector*)s;
	uint32		mask0;
#if defined(__SSE2__) && !defined(__AVX2__)
	uint32		mask1;
	uint32		mask2;
	uint32		mask3;
	word		mask;
#endif

	if (!s || !*s)
	{
		return 0;
	}

	mask0 = MoveMask_uint8(CmpEq_int8(zero, LoadUnaligned(vs)));

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
#if defined(__AVX2__)
			mask0 = MoveMask_uint8(CmpEq_int8(vs[0], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0);
			}
			mask0 = MoveMask_uint8(CmpEq_int8(vs[1], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0) + VecSize;
			}
			mask0 = MoveMask_uint8(CmpEq_int8(vs[2], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0) + VecSize * 2;
			}
			mask0 = MoveMask_uint8(CmpEq_int8(vs[3], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0) + VecSize * 3;
			}
#elif defined(__SSE2__)
			mask0 = MoveMask_uint8(CmpEq_int8(vs[0], zero));
			mask1 = MoveMask_uint8(CmpEq_int8(vs[1], zero));
			mask2 = MoveMask_uint8(CmpEq_int8(vs[2], zero));
			mask3 = MoveMask_uint8(CmpEq_int8(vs[3], zero));
			mask  = (((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32);
			return (const char*)vs - start + BitScanForward(mask);
#endif
		}

		vs += 4;
		mask0 = MoveMask_uint8(CmpEq_int8(zero, Min_uint8(Min_uint8(vs[0], vs[1]), Min_uint8(vs[2], vs[3]))));

		if (mask0)
		{
#if defined(__AVX2__)
			mask0 = MoveMask_uint8(CmpEq_int8(vs[0], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0);
			}
			mask0 = MoveMask_uint8(CmpEq_int8(vs[1], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0) + VecSize;
			}
			mask0 = MoveMask_uint8(CmpEq_int8(vs[2], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0) + VecSize * 2;
			}
			mask0 = MoveMask_uint8(CmpEq_int8(vs[3], zero));
			if (mask0)
			{
				return (const char*)vs - start + BitScanForward(mask0) + VecSize * 3;
			}
#elif defined(__SSE2__)
			mask0 = MoveMask_uint8(CmpEq_int8(vs[0], zero));
			mask1 = MoveMask_uint8(CmpEq_int8(vs[1], zero));
			mask2 = MoveMask_uint8(CmpEq_int8(vs[2], zero));
			mask3 = MoveMask_uint8(CmpEq_int8(vs[3], zero));
			mask  = (((word)mask1 << 16) | (word)mask0) | ((((word)mask3 << 16) | (word)mask2) << 32);
			return (const char*)vs - start + BitScanForward(mask);
#endif
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