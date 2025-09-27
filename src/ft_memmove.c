#include "../libft.h"

__always_inline void*
ft_memmove(void* dst, const char* src, size_t n)
{
	return ft_memcpy(dst, src, n);
}
