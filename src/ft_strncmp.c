#include "../libft.h"

int
ft_strncmp(const char *s1, const char *s2, size_t n)
{
#if defined(SIMD_SUPPORTED)
	if (n <= 1)
	{
		switch (n)
		{
			case 0:
			{
				return 0;
			} break;

			case 1:
			{
				return (uint8)*s1 - (uint8)*s2;
			} break;
// Note: If one finds a way to trigger an overflow, one can add a default case to handle the situation.
		}
	}

	vector	zero = Zero();
	vector*	v1 = (vector*)s1;
	vector*	v2 = (vector*)s2;
	vector	vcmp = CmpEq_int8(LoadUnaligned(v1), LoadUnaligned(v2)) & ~CmpEq_int8(LoadUnaligned(v1), zero);
	size_t	mask = MoveMask_uint8(vcmp);

	if (n <= VecSize)
	{
		mask = ~mask;
		if (BitZeroHigh(mask, n))
		{
			mask = BitScanForward(mask);
			return (uint8)*(s1 + mask) - (uint8)*(s2 + mask);
		}
		return 0;
	}

	if (++mask)
	{
		mask = BitScanForward(mask);
		return (uint8)*(s1 + mask) - (uint8)*(s2 + mask);
	}

	vcmp = CmpEq_int8(LoadUnaligned(v1 + 1), LoadUnaligned(v2 + 1)) & ~CmpEq_int8(LoadUnaligned(v1 + 1), zero);
	mask = MoveMask_uint8(vcmp);

	if (++mask)
	{
		mask = BitScanForward(mask);

		if (mask >= n - VecSize)
		{
			return 0;
		}

		return (uint8)*(s1 + mask + VecSize) - (uint8)*(s2 + mask + VecSize);
	}

	n -= VecSize * 2;
	if ((ssize_t)n <= 0)
	{
		return 0;
	}

	vcmp = CmpEq_int8(LoadUnaligned(v1 + 2), LoadUnaligned(v2 + 2)) & ~CmpEq_int8(LoadUnaligned(v1 + 2), zero);
	mask = MoveMask_uint8(vcmp);

	if (++mask)
	{
		mask = BitScanForward(mask);

		if (mask >= n)
		{
			return 0;
		}

		return (uint8)*(s1 + mask + VecSize * 2) - (uint8)*(s2 + mask + VecSize * 2);
	}

	if (n <= VecSize)
	{
		return 0;
	}

	n += (uintptr)v1 + VecSize * 2;
	v2 = (vector*)((uintptr)v2 - (uintptr)v1);
	v1 = (vector*)((uintptr)v1 & -(VecSize * 4));
	v2 = (vector*)((uintptr)v2 + (uintptr)v1);
	n -= (uintptr)v1;

	while (true)
	{
		n -= VecSize * 4;

		if ((ssize_t)n <= 0)
		{
			return 0;
		}

		v1 += 4;
		v2 += 4;

		vector	cp0 = CmpEq_int8(*v1, LoadUnaligned(v2)) & *v1;
		vector	cp1 = CmpEq_int8(*(v1 + 1), LoadUnaligned(v2 + 1)) & *(v1 + 1);
		vector	cp2 = CmpEq_int8(*(v1 + 2), LoadUnaligned(v2 + 2)) & *(v1 + 2);
		vector	cp3 = CmpEq_int8(*(v1 + 3), LoadUnaligned(v2 + 3)) & *(v1 + 3);

		vector	cp5 = Min_uint8(Min_uint8(cp0, cp1), Min_uint8(cp2, cp3));
		mask = MoveMask_uint8(CmpEq_int8(cp5, zero));

		if (mask)
		{
			mask = MoveMask_uint8(CmpEq_int8(cp0, zero));
			if (mask)
			{
				mask = BitScanForward(mask);
				if (mask >= n)
				{
					return 0;
				}
				return *((uint8*)v1 + mask) - *((uint8*)v2 + mask);
			}
			mask = MoveMask_uint8(CmpEq_int8(cp1, zero));
			if (mask)
			{
				mask = BitScanForward(mask << VecSize);
				if (mask >= n)
				{
					return 0;
				}
				return *((uint8*)(v1 + 1) + mask) - *((uint8*)(v2 + 1) + mask);
			}
			n -= VecSize * 2;
			if ((ssize_t)n <= 0)
			{
				return 0;
			}
			mask = MoveMask_uint8(CmpEq_int8(cp2, zero));
			if (mask)
			{
				mask = BitScanForward(mask);
				if (mask >= n)
				{
					return 0;
				}
				return *((uint8*)(v1 + 2) + mask) - *((uint8*)(v2 + 2) + mask);
			}

			mask = BitScanForward(mask) - VecSize;
			if ((ssize_t)mask <= 0)
			{
				return 0;
			}
			return *((uint8*)(v1 + 3) + mask) - *((uint8*)(v2 + 3) + mask);
		}
	}
#else
	const uint8* l = (void*)s1;
	const uint8* r = (void*)s2;

	if (!n--)
	{
		return 0;
	}

	while (*l && *r && n && *l == *r) { n--, l++, r++; }
	return *l - *r;
#endif
}