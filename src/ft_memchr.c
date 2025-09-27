#include "../libft.h"


void*
ft_memchr(void* s, uint8 c, size_t n)
{
	uint8*	ptr = s;

	while (n-- && *ptr != c)
	{
		ptr++;
	}

	return *ptr == c ? ptr : 0;
}
