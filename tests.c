#include "libft.h"

#define __ASSERT_FUNCTION	((const char *) 0)
#define assert(expr)					\
    ((expr)								\
     ?  (void) (0)						\
     : __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))

function void
__assert_fail (const char *__assertion, const char *__file,
           unsigned int __line, const char *__function)
{
	ft_write(2, "Assertion failed: ", 18);
	ft_write(2, __assertion, ft_strlen(__assertion) + 1);
	ft_write(2, ", in file ", 10);
	ft_write(2, __file, ft_strlen(__file));
	ft_write(2, ", at line ", 10);

	char	line[12];
	ft_itoa(line, (int)__line);
	ft_write(2, line, ft_strlen(line));

	ft_write(2, "\n", 1);
	// TODO: Convert line number to string and print it
    asm volatile("mov $60, %rax; mov $1, %rdi; syscall");
}

int	main(void)
{
	// Note: tests for ft_isalpha

	assert(ft_isalpha(' ') == false);
	assert(ft_isalpha('5') == false);
	assert(ft_isalpha('a') == true);
	assert(ft_isalpha('A') == true);
	// Note: Uncomment to get a compilation warning 
	//assert(ft_isalpha(1000) == true);

	// Note: tests for ft_isdigit
	assert(ft_isdigit(' ') == false);
	assert(ft_isdigit('5') == true);
	assert(ft_isdigit('a') == false);
	assert(ft_isdigit('A') == false);
	// Note: Uncomment to get a compilation warning
	//assert(ft_isdigit(1000) == true);

	// Note: tests for ft_isalnum
	assert(ft_isalnum(' ') == false);
	assert(ft_isalnum('5') == true);
	assert(ft_isalnum('a') == true);
	assert(ft_isalnum('A') == true);
	// Note: Uncomment to get a compilation warning
	//assert(ft_isalnum(1000) == true);

	// Note: tests for ft_isascii
	assert(ft_isascii(' ') == true);
	assert(ft_isascii('5') == true);
	assert(ft_isascii('a') == true);
	assert(ft_isascii('A') == true);
	assert(ft_isascii(127) == true);
	assert(ft_isascii(128) == false);

	// Note: tests for ft_isprint
	assert(ft_isprint(' ') == true);
	assert(ft_isprint('5') == true);
	assert(ft_isprint('a') == true);
	assert(ft_isprint('A') == true);
	assert(ft_isprint(126) == true);
	assert(ft_isprint(127) == false);
	assert(ft_isprint(31) == false);
	assert(ft_isprint(0) == false);

	// Note: tests for ft_toupper
	assert(ft_toupper(' ') == ' ');
	assert(ft_toupper('5') == '5');
	assert(ft_toupper('a') == 'A');
	assert(ft_toupper('z') == 'Z');
	assert(ft_toupper('A') == 'A');
	assert(ft_toupper('Z') == 'Z');
	assert(ft_toupper('!') == '!');

	// Note: tests for ft_tolower
	assert(ft_tolower(' ') == ' ');
	assert(ft_tolower('5') == '5');
	assert(ft_tolower('a') == 'a');
	assert(ft_tolower('z') == 'z');
	assert(ft_tolower('A') == 'a');
	assert(ft_tolower('Z') == 'z');
	assert(ft_tolower('!') == '!');

	// Note: tests for ft_strlen
	assert(ft_strlen("") == 0);
	assert(ft_strlen("a") == 1);
	assert(ft_strlen("abc") == 3);
	assert(ft_strlen("abcdefghijklmno") == 15);
	assert(ft_strlen("abcdefghijklmnop") == 16);
	assert(ft_strlen("This is a longer string to test strlen function.") == 48);

	// Note: tests for ft_itoa (no alloc)
	// TODO: improve testing
	char	numC[12];
	int	num = 54;
	char*	res = ft_itoa(numC, num);

	// Note: tests for ft_memset
	char	buf1[20];
	char	buf2[20];
	ft_memset(buf1, 'A', 20);
	for (size_t i = 0; i < 20; i++)
	{
		assert(buf1[i] == 'A');
	}
	ft_memset(buf2, 'B', 5);
	for (size_t i = 0; i < 5; i++)
	{
		assert(buf2[i] == 'B');
	}
	ft_memset(buf2 + 5, 'C', 10);
	for (size_t i = 0; i < 5; i++)
	{
		assert(buf2[i] == 'B');
	}
	for (size_t i = 5; i < 15; i++)
	{
		assert(buf2[i] == 'C');
	}

	// Note: tests for ft_bzero
	char	buf3[20];
	ft_memset(buf3, 'A', 20);
	ft_bzero(buf3, 20);
	for (size_t i = 0; i < 20; i++)
	{
		assert(buf3[i] == 0);
	}

	// Note: tests for ft_memcpy
	char	src1[14] = "Hello, World!";
	char	dst1[14];
	char	dst2[14];
	char*	dst3 = ft_memcpy(dst1, src1, 14);
	char*	dst4 = ft_memcpy(dst2, src1, 14);
	assert(dst3 == dst1);
	assert(dst4 == dst2);
	for (size_t i = 0; i < 14; i++)
	{
		assert(dst1[i] == src1[i]);
		assert(dst2[i] == src1[i]);
	}
	char	src2[50] = "This is a longer string to test memcpy function.";
	char	dst5[50];
	char*	dst6 = ft_memcpy(dst5, src2, 50);
	assert(dst6 == dst5);
	for (size_t i = 0; i < 50; i++)
	{
		assert(dst5[i] == src2[i]);
	}

	// Note: tests for ft_memmove
	char	src3[50] = "This is a longer string to test memmove function.";
	char	dst7[50];
	char*	dst8 = ft_memmove(dst7, src3, 50);
	assert(dst8 == dst7);
	for (size_t i = 0; i < 50; i++)
	{
		assert(dst7[i] == src3[i]);
	}
	// Note: test overlapping memory with src < dst
	char	overlap_buf[20] = "Hello, World!";
	char*	src_ptr = overlap_buf;
	char*	dst_ptr = overlap_buf + 5;
	assert(dst_ptr > src_ptr);
	ft_memmove(dst_ptr, src_ptr, 10);
	char*	expected = "HelloHello, Wor";
	for (size_t i = 0; i < 15; i++)
	{
		assert(overlap_buf[i] == expected[i]);
	}
	// Note: another test with src < dst
	char	overlap_buf2[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char*	src2_ptr = overlap_buf2 + 2;
	char*	dst2_ptr = overlap_buf2 + 8;
	assert(dst2_ptr > src2_ptr);
	ft_memmove(dst2_ptr, src2_ptr, 10);
	char*	expected2 = "ABCDEFGHCDEFGHIJKLSTUVWXYZ";
	for (size_t i = 0; i < 26; i++)
	{
		assert(overlap_buf2[i] == expected2[i]);
	}

	// Note: tests for ft_strlcpy
	char	dest1[20];
	size_t	ret1 = ft_strlcpy(dest1, "Hello, World!", sizeof(dest1));
	assert(ft_strlen(dest1) == ret1);
	char	dest2[50];
	char*	src_str = "This is a longer string to test strlcpy function.";
	size_t	ret2 = ft_strlcpy(dest2, src_str, sizeof(dest2));
	assert(ft_strlen(dest2) == ret2);
	assert(ft_strlen(src_str) == ret2);

	// Note: tests for ft_strlcat
	char	dest3[50] = "Hello, ";
	size_t	ret3 = ft_strlcat(dest3, "World!", sizeof(dest3));
	assert(ft_strlen(dest3) == ret3);
	assert(ft_strlen("Hello, World!") == ret3);
	char	dest4[50] = "This is a ";
	char*	src_str2 = "longer string to test strlcat function.";
	size_t	ret4 = ft_strlcat(dest4, src_str2, sizeof(dest4));
	assert(ft_strlen(dest4) == ret4);
	assert(ft_strlen("This is a longer string to test strlcat function.") == ret4);

	return (0);
}
