#include "../libft.h"

size_t
ft_strlcat(char* dst, const char* src, size_t size)
{
	size_t	dst_len = ft_strlen(dst);

	if (dst_len >= size)
	{
		return size;
	}

	return dst_len + ft_strlcpy(dst + dst_len, src, size - dst_len);
}
