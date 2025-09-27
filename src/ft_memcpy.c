#include "../libft.h"

void*
ft_memcpy(void* dst, const char* src, size_t n)
{
#if defined(SIMD_SUPPORTED)
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
		StoreUnaligned((vector*)dst, LoadUnaligned((vector*)src));
		StoreUnaligned((vector*)(dst - VecSize + n), LoadUnaligned((vector*)(src - VecSize + n)));
		return dst;
	}

	if (n <= VecSize * 4)
	{
		StoreUnaligned((vector*)dst, LoadUnaligned((vector*)src));
		StoreUnaligned((vector*)(dst + VecSize), LoadUnaligned((vector*)(src + VecSize)));
		StoreUnaligned((vector*)(dst - VecSize + n), LoadUnaligned((vector*)(src - VecSize + n)));
		StoreUnaligned((vector*)(dst - 2 * VecSize + n), LoadUnaligned((vector*)(src - 2 * VecSize + n)));
		return dst;
	}

	if (n <= VecSize * 8)
	{
		StoreUnaligned((vector*)dst, LoadUnaligned((vector*)src));
		StoreUnaligned((vector*)(dst + VecSize), LoadUnaligned((vector*)(src + VecSize)));
		StoreUnaligned((vector*)(dst + 2 * VecSize), LoadUnaligned((vector*)(src + 2 * VecSize)));
		StoreUnaligned((vector*)(dst + 3 * VecSize), LoadUnaligned((vector*)(src + 3 * VecSize)));
		StoreUnaligned((vector*)(dst - VecSize + n), LoadUnaligned((vector*)(src - VecSize + n)));
		StoreUnaligned((vector*)(dst - 2 * VecSize + n), LoadUnaligned((vector*)(src - 2 * VecSize + n)));
		StoreUnaligned((vector*)(dst - 3 * VecSize + n), LoadUnaligned((vector*)(src - 3 * VecSize + n)));
		StoreUnaligned((vector*)(dst - 4 * VecSize + n), LoadUnaligned((vector*)(src - 4 * VecSize + n)));
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
		vector	head  = LoadUnaligned((vector*)src);
		vector*	dst_ptr = (vector*)(((uintptr)dst | (VecSize - 1)) + 1);
		vector*	vsrc  = (vector*)((uintptr)src + (uintptr)dst_ptr - (uintptr)dst);
		vector*	src_tail_ptr = (vector*)(src + n - VecSize * 4);
		vector	tail0 = LoadUnaligned(src_tail_ptr);
		vector	tail1 = LoadUnaligned(src_tail_ptr + 1);
		vector	tail2 = LoadUnaligned(src_tail_ptr + 2);
		vector	tail3 = LoadUnaligned(src_tail_ptr + 3);

		do
		{
			*dst_ptr = LoadUnaligned(vsrc); 
			*(dst_ptr + 1) = LoadUnaligned(vsrc + 1);
			*(dst_ptr + 2) = LoadUnaligned(vsrc + 2);
			*(dst_ptr + 3) = LoadUnaligned(vsrc + 3);

			vsrc += 4;
			dst_ptr += 4;
		}
		while (vsrc < src_tail_ptr);

		dst_ptr = (vector*)(dst + n - VecSize * 4);
		StoreUnaligned((vector*)dst, head);
		StoreUnaligned(dst_ptr, tail0);
		StoreUnaligned(dst_ptr + 1, tail1);
		StoreUnaligned(dst_ptr + 2, tail2);
		StoreUnaligned(dst_ptr + 3, tail3);
	}
	else
	{
// Backward
		vector	tail  = LoadUnaligned((vector*)(src - VecSize + n));
		vector* dst_ptr = (vector*)(((uintptr)(dst + n - VecSize * 4 - 1) & -VecSize));
		vector* vsrc = (vector*)((uintptr)src + (uintptr)dst_ptr - (uintptr)dst);
		vector*	src_head_ptr = (vector*)(src);
		vector	head0 = LoadUnaligned(src_head_ptr);
		vector	head1 = LoadUnaligned(src_head_ptr + 1);
		vector	head2 = LoadUnaligned(src_head_ptr + 2);
		vector	head3 = LoadUnaligned(src_head_ptr + 3);

		do
		{
			*dst_ptr = LoadUnaligned(vsrc); 
			*(dst_ptr + 1) = LoadUnaligned(vsrc + 1);
			*(dst_ptr + 2) = LoadUnaligned(vsrc + 2);
			*(dst_ptr + 3) = LoadUnaligned(vsrc + 3);

			vsrc -= 4;
			dst_ptr -= 4;
		}
		while (vsrc > src_head_ptr);

		dst_ptr = (vector*) dst;
		StoreUnaligned((vector*)(dst - VecSize + n), tail);
		StoreUnaligned(dst_ptr, head0);
		StoreUnaligned(dst_ptr + 1, head1);
		StoreUnaligned(dst_ptr + 2, head2);
		StoreUnaligned(dst_ptr + 3, head3);
	}

	return dst;
#else
// TODO: Opti by copying 8 by 8
	if (!dst || !src || dst == src)
	{
		return dst;
	}

	char*	ptr = dst;

	if ((uintptr)dst > (uintptr)src && (uintptr)dst < (uintptr)src + n)
	{
		ptr += n;
		src += n;

		while (n--)
		{
			*ptr-- = *src--;
		}

		*ptr-- = *src--;
		return dst;
	}

	while (n--)
	{
		*ptr++ = *src++;
	}

	return dst;
#endif
}