#include "../libft.h"

__always_inline bool
ft_isalpha(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25);
}

__always_inline bool
ft_isdigit(uint8 c)
{
	return (uint8)(c - 48) <= 9;
}

__always_inline bool
ft_isalnum(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) |
		((uint8)(c - 97) <= 25) |
		((uint8)(c - 48) <= 9);
}

__always_inline bool
ft_isascii(uint8 c)
{
	return (uint8)c <= 127;
}

__always_inline bool
ft_isprint(uint8 c)
{
	return (uint8)(c - 32) <= 94;
}

__always_inline uint8
ft_toupper(uint8 c)
{
	return ((uint8)(c - 97) <= 25) ? (uint8)c - 32 : c;
}

__always_inline uint8
ft_tolower(uint8 c)
{
	return 	((uint8)(c - 65) <= 25) ? (uint8)c + 32 : c;
}
