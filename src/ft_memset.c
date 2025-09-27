#include "../libft.h"

void*
ft_memset(void* s, uint8 c, size_t n)
{
#if defined(SIMD_SUPPORTED)
	void*		start = s;
	vector*	end = (vector*)(s + n);
	vector*	vs = (vector*)s;
	vector	value = Set1_int8(c);
	
	if (n < VecSize)
	{
		word	svalue = RepeatByteInWord(c);
#if defined(__AVX2__)
		if (n >= 16)
		{
			*(uint128*)s = (uint128)svalue << 64 | svalue;
			*(uint128*)(s - 16 + n) = (uint128)svalue << 64 | svalue;
			return s;
		}
#endif
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
		StoreUnaligned(vs, value);
		return s;
	}

	StoreUnaligned(vs, value);
	StoreUnaligned(vs + 1, value);

	if (n <= VecSize * 4)
	{
		StoreUnaligned(end - 1, value);
		StoreUnaligned(end - 2, value);
		return start;
	}
	
	StoreUnaligned(vs + 2, value);
	StoreUnaligned(vs + 3, value);

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
		*vs = value;
		*(vs + 1) = value;
		*(vs + 2) = value;
		*(vs + 3) = value;
		vs += 4;
	}
	while (vs < end);

FourLastVectors:
	StoreUnaligned(end, value);
	StoreUnaligned(end + 1, value);
	StoreUnaligned(end + 2, value);
	StoreUnaligned(end + 3, value);
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
