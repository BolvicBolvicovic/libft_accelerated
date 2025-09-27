#include "../libft.h"

size_t
ft_strlcpy(char* dst, const char* src, size_t size)
{
	size_t		src_len = ft_strlen(src);

	if (size)
	{
		size_t		to_copy = src_len < size ? src_len : size - 1;
		ft_memcpy(dst, src, to_copy);
		dst[to_copy] = 0;
	}

	return src_len;
}
	