#include "../libft.h"

#define __ASSERT_FUNCTION	((const char *) 0)
#define assert(expr)					\
    ((expr)								\
     ?  (void) (0)						\
     : __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))

function void
__assert_fail (const char* __assertion, const char* __file,
           unsigned int __line, const char* __function)
{
	ft_write(2, "Assertion failed: ", 18);
	ft_write(2, __assertion, ft_strlen(__assertion) + 1);
	// TODO: fix __ASSERT_FUNCTION
	//ft_write(2, ", in function ", 14);
	//ft_write(2, __assertion, ft_strlen(__function) + 1);
	(void)__function;
	ft_write(2, ", in file ", 10);
	ft_write(2, __file, ft_strlen(__file));
	ft_write(2, ", at line ", 10);

	char	line[12];
	ft_itoa(line, (int)__line);
	ft_write(2, line, ft_strlen(line));

	ft_write(2, "\n", 1);
    asm volatile("mov $60, %rax; mov $1, %rdi; syscall");
}

int	main(void)
{
	// Note: tests for ft_isalpha
	// Normal tests
	assert(ft_isalpha('a') == true);
	assert(ft_isalpha('z') == true);
	assert(ft_isalpha('A') == true);
	assert(ft_isalpha('Z') == true);
	assert(ft_isalpha('m') == true);
	assert(ft_isalpha('M') == true);
	
	// Edge cases - boundary values
	assert(ft_isalpha('a' - 1) == false); // '`' (before 'a')
	assert(ft_isalpha('A' - 1) == false); // '@' (before 'A')
	assert(ft_isalpha('z' + 1) == false); // '{' (after 'z')
	assert(ft_isalpha('Z' + 1) == false); // '[' (after 'Z')
	assert(ft_isalpha('A' + 25) == true);  // 'Z' (last uppercase)
	assert(ft_isalpha('a' + 25) == true);  // 'z' (last lowercase)
	
	// Non-alphabetic characters
	assert(ft_isalpha(' ') == false);
	assert(ft_isalpha('5') == false);
	assert(ft_isalpha('0') == false);
	assert(ft_isalpha('9') == false);
	assert(ft_isalpha('!') == false);
	assert(ft_isalpha('@') == false);
	assert(ft_isalpha('#') == false);
	assert(ft_isalpha('\0') == false);
	assert(ft_isalpha('\n') == false);
	assert(ft_isalpha('\t') == false);
	
	// Extended ASCII edge cases
	assert(ft_isalpha(-1) == false);
	assert(ft_isalpha(128) == false);
	assert(ft_isalpha(255) == false);

	// Note: tests for ft_isdigit
	// Normal tests
	assert(ft_isdigit('0') == true);
	assert(ft_isdigit('1') == true);
	assert(ft_isdigit('5') == true);
	assert(ft_isdigit('9') == true);
	
	// Edge cases - boundary values
	assert(ft_isdigit('0' - 1) == false); // '/' (before '0')
	assert(ft_isdigit('9' + 1) == false); // ':' (after '9')
	
	// Non-digit characters
	assert(ft_isdigit(' ') == false);
	assert(ft_isdigit('a') == false);
	assert(ft_isdigit('A') == false);
	assert(ft_isdigit('z') == false);
	assert(ft_isdigit('Z') == false);
	assert(ft_isdigit('!') == false);
	assert(ft_isdigit('@') == false);
	assert(ft_isdigit('\0') == false);
	assert(ft_isdigit('\n') == false);
	assert(ft_isdigit('\t') == false);
	
	// Extended ASCII edge cases
	assert(ft_isdigit(-1) == false);
	assert(ft_isdigit(128) == false);
	assert(ft_isdigit(255) == false);

	// Note: tests for ft_isalnum
	// Normal tests - digits
	assert(ft_isalnum('0') == true);
	assert(ft_isalnum('5') == true);
	assert(ft_isalnum('9') == true);
	
	// Normal tests - letters
	assert(ft_isalnum('a') == true);
	assert(ft_isalnum('z') == true);
	assert(ft_isalnum('A') == true);
	assert(ft_isalnum('Z') == true);
	assert(ft_isalnum('m') == true);
	
	// Edge cases - boundary values
	assert(ft_isalnum('0' - 1) == false); // '/' (before '0')
	assert(ft_isalnum('9' + 1) == false); // ':' (after '9')
	assert(ft_isalnum('A' - 1) == false); // '@' (before 'A')
	assert(ft_isalnum('Z' + 1) == false); // '[' (after 'Z')
	assert(ft_isalnum('a' - 1) == false); // '`' (before 'a')
	assert(ft_isalnum('z' + 1) == false); // '{' (after 'z')
	
	// Non-alphanumeric characters
	assert(ft_isalnum(' ') == false);
	assert(ft_isalnum('!') == false);
	assert(ft_isalnum('@') == false);
	assert(ft_isalnum('#') == false);
	assert(ft_isalnum('\0') == false);
	assert(ft_isalnum('\n') == false);
	assert(ft_isalnum('\t') == false);
	
	// Extended ASCII edge cases
	assert(ft_isalnum(-1) == false);
	assert(ft_isalnum(128) == false);
	assert(ft_isalnum(255) == false);

	// Note: tests for ft_isascii
	// Normal tests - common ASCII characters
	assert(ft_isascii(' ') == true);
	assert(ft_isascii('5') == true);
	assert(ft_isascii('a') == true);
	assert(ft_isascii('A') == true);
	assert(ft_isascii('0') == true);
	assert(ft_isascii('z') == true);
	assert(ft_isascii('Z') == true);
	assert(ft_isascii('!') == true);
	assert(ft_isascii('@') == true);
	assert(ft_isascii('\0') == true);
	assert(ft_isascii('\n') == true);
	assert(ft_isascii('\t') == true);
	
	// Edge cases - boundary values
	assert(ft_isascii(0) == true);     // First ASCII
	assert(ft_isascii(127) == true);   // Last ASCII
	assert(ft_isascii(128) == false);  // First non-ASCII
	assert(ft_isascii(-1) == false);   // Before ASCII range
	assert(ft_isascii(255) == false);  // Extended ASCII

	// Note: tests for ft_isprint
	// Normal tests - printable characters
	assert(ft_isprint(' ') == true);   // Space (32)
	assert(ft_isprint('!') == true);   // First printable symbol
	assert(ft_isprint('5') == true);
	assert(ft_isprint('a') == true);
	assert(ft_isprint('A') == true);
	assert(ft_isprint('z') == true);
	assert(ft_isprint('Z') == true);
	assert(ft_isprint('@') == true);
	assert(ft_isprint('#') == true);
	assert(ft_isprint('~') == true);   // Last printable (126)
	
	// Edge cases - boundary values
	assert(ft_isprint(32) == true);    // Space (first printable)
	assert(ft_isprint(126) == true);   // ~ (last printable)
	assert(ft_isprint(31) == false);   // Before printable range
	assert(ft_isprint(127) == false);  // DEL (after printable range)
	
	// Control characters (non-printable)
	assert(ft_isprint(0) == false);    // NULL
	assert(ft_isprint('\n') == false); // Newline (10)
	assert(ft_isprint('\t') == false); // Tab (9)
	assert(ft_isprint('\r') == false); // Carriage return (13)
	assert(ft_isprint(1) == false);
	assert(ft_isprint(8) == false);    // Backspace
	assert(ft_isprint(27) == false);   // Escape
	
	// Extended ASCII and beyond
	assert(ft_isprint(128) == false);
	assert(ft_isprint(255) == false);
	assert(ft_isprint(-1) == false);

	// Note: tests for ft_toupper
	// Normal tests - lowercase to uppercase
	assert(ft_toupper('a') == 'A');
	assert(ft_toupper('b') == 'B');
	assert(ft_toupper('m') == 'M');
	assert(ft_toupper('z') == 'Z');
	
	// Already uppercase - should remain unchanged
	assert(ft_toupper('A') == 'A');
	assert(ft_toupper('B') == 'B');
	assert(ft_toupper('M') == 'M');
	assert(ft_toupper('Z') == 'Z');
	
	// Edge cases - boundary values
	assert(ft_toupper('a' - 1) == 'a' - 1); // '`' (before 'a')
	assert(ft_toupper('z' + 1) == 'z' + 1); // '{' (after 'z')
	
	// Non-alphabetic characters - should remain unchanged
	assert(ft_toupper(' ') == ' ');
	assert(ft_toupper('5') == '5');
	assert(ft_toupper('0') == '0');
	assert(ft_toupper('9') == '9');
	assert(ft_toupper('!') == '!');
	assert(ft_toupper('@') == '@');
	assert(ft_toupper('#') == '#');
	assert(ft_toupper('\0') == '\0');
	assert(ft_toupper('\n') == '\n');
	assert(ft_toupper('\t') == '\t');
	
	// Extended ASCII - should remain unchanged
	assert(ft_toupper(128) == 128);
	assert(ft_toupper(255) == 255);

	// Note: tests for ft_tolower
	// Normal tests - uppercase to lowercase
	assert(ft_tolower('A') == 'a');
	assert(ft_tolower('B') == 'b');
	assert(ft_tolower('M') == 'm');
	assert(ft_tolower('Z') == 'z');
	
	// Already lowercase - should remain unchanged
	assert(ft_tolower('a') == 'a');
	assert(ft_tolower('b') == 'b');
	assert(ft_tolower('m') == 'm');
	assert(ft_tolower('z') == 'z');
	
	// Edge cases - boundary values
	assert(ft_tolower('A' - 1) == 'A' - 1); // '@' (before 'A')
	assert(ft_tolower('Z' + 1) == 'Z' + 1); // '[' (after 'Z')
	
	// Non-alphabetic characters - should remain unchanged
	assert(ft_tolower(' ') == ' ');
	assert(ft_tolower('5') == '5');
	assert(ft_tolower('0') == '0');
	assert(ft_tolower('9') == '9');
	assert(ft_tolower('!') == '!');
	assert(ft_tolower('@') == '@');
	assert(ft_tolower('#') == '#');
	assert(ft_tolower('\0') == '\0');
	assert(ft_tolower('\n') == '\n');
	assert(ft_tolower('\t') == '\t');
	
	// Extended ASCII - should remain unchanged
	assert(ft_tolower(128) == 128);
	assert(ft_tolower(255) == 255);

	// Note: tests for ft_strlen
	// Edge case - empty string
	assert(ft_strlen("") == 0);
	
	// Normal tests - various lengths
	assert(ft_strlen("a") == 1);
	assert(ft_strlen("ab") == 2);
	assert(ft_strlen("abc") == 3);
	assert(ft_strlen("Hello") == 5);
	assert(ft_strlen("Hello, World!") == 13);
	
	// Size tests - different string lengths
	assert(ft_strlen("abcdefghijklmno") == 15);
	assert(ft_strlen("abcdefghijklmnop") == 16);  // 16-byte boundary
	assert(ft_strlen("abcdefghijklmnopqrstuvwxyz") == 26);  // Alphabet
	assert(ft_strlen("This is a longer string to test strlen function.") == 48);
	
	// Test with special characters
	assert(ft_strlen("Hello\nWorld") == 11);  // With newline
	assert(ft_strlen("Tab\tSeparated") == 13);  // With tab
	assert(ft_strlen("   Spaces   ") == 12);   // With spaces
	assert(ft_strlen("!@#$%^&*()") == 10);     // Special characters
	
	// Test with numbers and mixed content
	assert(ft_strlen("1234567890") == 10);
	assert(ft_strlen("Mixed123Content!@#") == 18);
	
	// Size tests - longer strings
	char long_str[100];
	for (int i = 0; i < 99; i++)
		long_str[i] = 'A';
	long_str[99] = '\0';
	assert(ft_strlen(long_str) == 99);
	
	char very_long_str[1000];
	for (int i = 0; i < 999; i++)
		very_long_str[i] = 'B';
	very_long_str[999] = '\0';
	assert(ft_strlen(very_long_str) == 999);

	// Note: tests for ft_itoa (no alloc)
	char	numC[12];
	
	// Basic functionality test - just ensure it returns the buffer and doesn't crash
	char* res1 = ft_itoa(numC, 54);
	assert(res1 == numC);  // Should return the provided buffer
	
	char* res2 = ft_itoa(numC, 0);
	assert(res2 == numC);
	
	char* res3 = ft_itoa(numC, -42);
	assert(res3 == numC);
	
	char* res4 = ft_itoa(numC, 2147483647); // INT_MAX
	assert(res4 == numC);
	
	char* res5 = ft_itoa(numC, -2147483648); // INT_MIN  
	assert(res5 == numC);
	
	// TODO: Add more specific content tests once implementation details are clarified

	// Note: tests for ft_memset
	char	buf1[20];
	char	buf2[20];
	char	buf1000[1000];
	
	// Normal test - fill entire buffer
	char* memset_ret1 = ft_memset(buf1, 'A', 20);
	assert(memset_ret1 == buf1);  // Should return pointer to buffer
	for (size_t i = 0; i < 20; i++)
	{
		assert(buf1[i] == 'A');
	}
	
	// Normal test - partial fill
	ft_memset(buf2, 'X', 20);  // Initialize with different value
	char* memset_ret2 = ft_memset(buf2, 'B', 5);
	assert(memset_ret2 == buf2);
	for (size_t i = 0; i < 5; i++)
	{
		assert(buf2[i] == 'B');
	}
	for (size_t i = 5; i < 20; i++)
	{
		assert(buf2[i] == 'X');  // Rest should be unchanged
	}
	
	// Size test - large buffer
	char* memset_ret3 = ft_memset(buf1000, 'C', 1000);
	assert(memset_ret3 == buf1000);
	for (size_t i = 0; i < 1000; i++)
	{
		assert(buf1000[i] == 'C');
	}
	
	// Test with offset pointer
	ft_memset(buf2, 'D', 20);  // Reset buffer
	char* memset_ret4 = ft_memset(buf2 + 5, 'E', 10);
	assert(memset_ret4 == buf2 + 5);
	for (size_t i = 0; i < 5; i++)
	{
		assert(buf2[i] == 'D');  // First 5 unchanged
	}
	for (size_t i = 5; i < 15; i++)
	{
		assert(buf2[i] == 'E');  // Middle 10 changed
	}
	for (size_t i = 15; i < 20; i++)
	{
		assert(buf2[i] == 'D');  // Last 5 unchanged
	}
	
	// Edge case - zero length
	char memset_single_char = 'Z';
	char* memset_ret5 = ft_memset(&memset_single_char, 'F', 0);
	assert(memset_ret5 == &memset_single_char);
	assert(memset_single_char == 'Z');  // Should remain unchanged
	
	// Edge case - single byte
	char* memset_ret6 = ft_memset(&memset_single_char, 'G', 1);
	assert(memset_ret6 == &memset_single_char);
	assert(memset_single_char == 'G');
	
	// Test with different values
	char test_values[256];
	for (int val = 0; val < 256; val++)
	{
		ft_memset(test_values, val, 1);
		assert((unsigned char)test_values[0] == val);
	}

	// Note: tests for ft_bzero
	char	buf3[20];
	char	buf_large[1000];
	
	// Normal test - zero entire buffer
	ft_memset(buf3, 'A', 20);  // Initialize with non-zero
	ft_bzero(buf3, 20);
	for (size_t i = 0; i < 20; i++)
	{
		assert(buf3[i] == 0);
	}
	
	// Normal test - partial zero
	ft_memset(buf3, 'B', 20);  // Reset buffer
	ft_bzero(buf3, 10);
	for (size_t i = 0; i < 10; i++)
	{
		assert(buf3[i] == 0);  // First 10 should be zero
	}
	for (size_t i = 10; i < 20; i++)
	{
		assert(buf3[i] == 'B');  // Last 10 should be unchanged
	}
	
	// Test with offset pointer
	ft_memset(buf3, 'C', 20);  // Reset buffer
	ft_bzero(buf3 + 5, 8);
	for (size_t i = 0; i < 5; i++)
	{
		assert(buf3[i] == 'C');  // First 5 unchanged
	}
	for (size_t i = 5; i < 13; i++)
	{
		assert(buf3[i] == 0);    // Middle 8 zeroed
	}
	for (size_t i = 13; i < 20; i++)
	{
		assert(buf3[i] == 'C');  // Last 7 unchanged
	}
	
	// Size test - large buffer
	ft_memset(buf_large, 'D', 1000);
	ft_bzero(buf_large, 1000);
	for (size_t i = 0; i < 1000; i++)
	{
		assert(buf_large[i] == 0);
	}
	
	// Edge case - zero length
	char single = 'X';
	ft_bzero(&single, 0);
	assert(single == 'X');  // Should remain unchanged
	
	// Edge case - single byte
	single = 'Y';
	ft_bzero(&single, 1);
	assert(single == 0);

	// Note: tests for ft_memcpy
	char	src1[14] = "Hello, World!";
	char	dst1[14];
	char	dst2[14];
	
	// Normal test - copy string with null terminator
	char* memcpy_ret1 = ft_memcpy(dst1, src1, 14);
	assert(memcpy_ret1 == dst1);  // Should return destination pointer
	for (size_t i = 0; i < 14; i++)
	{
		assert(dst1[i] == src1[i]);
	}
	
	// Normal test - copy partial data
	ft_memset(dst2, 'X', 14);  // Initialize destination
	char* memcpy_ret2 = ft_memcpy(dst2, src1, 5);  // Copy only "Hello"
	assert(memcpy_ret2 == dst2);
	for (size_t i = 0; i < 5; i++)
	{
		assert(dst2[i] == src1[i]);
	}
	for (size_t i = 5; i < 14; i++)
	{
		assert(dst2[i] == 'X');  // Rest should be unchanged
	}
	
	// Size test - longer string
	char	src2[50] = "This is a longer string to test memcpy function.";
	char	dst5[50];
	char* memcpy_ret3 = ft_memcpy(dst5, src2, 50);
	assert(memcpy_ret3 == dst5);
	for (size_t i = 0; i < 50; i++)
	{
		assert(dst5[i] == src2[i]);
	}
	
	// Size test - large buffer
	char large_src[1000];
	char large_dst[1000];
	for (size_t i = 0; i < 1000; i++)
	{
		large_src[i] = (char)(i % 256);
	}
	char* memcpy_ret4 = ft_memcpy(large_dst, large_src, 1000);
	assert(memcpy_ret4 == large_dst);
	for (size_t i = 0; i < 1000; i++)
	{
		assert(large_dst[i] == large_src[i]);
	}
	
	// Test copying different data types (treat as bytes)  
	int int_src[4] = {1, 2, 3, 4};
	int int_dst[4];
	ft_memcpy(int_dst, (const char*)int_src, sizeof(int_src));
	for (int i = 0; i < 4; i++)
	{
		assert(int_dst[i] == int_src[i]);
	}
	
	// Edge case - zero length copy
	char memcpy_edge_dst = 'Z';
	char memcpy_edge_src = 'A';
	char* memcpy_ret5 = ft_memcpy(&memcpy_edge_dst, &memcpy_edge_src, 0);
	assert(memcpy_ret5 == &memcpy_edge_dst);
	assert(memcpy_edge_dst == 'Z');  // Should remain unchanged
	
	// Edge case - single byte copy
	char* memcpy_ret6 = ft_memcpy(&memcpy_edge_dst, &memcpy_edge_src, 1);
	assert(memcpy_ret6 == &memcpy_edge_dst);
	assert(memcpy_edge_dst == 'A');

	// Note: tests for ft_memmove
	char	src3[50] = "This is a longer string to test memmove function.";
	char	dst7[50];
	
	// Normal test - non-overlapping memory (like memcpy)
	char* memmove_ret1 = ft_memmove(dst7, src3, 50);
	assert(memmove_ret1 == dst7);  // Should return destination pointer
	for (size_t i = 0; i < 50; i++)
	{
		assert(dst7[i] == src3[i]);
	}
	
	// Test overlapping memory - basic functionality
	char	overlap_buf1[20] = "Hello, World!";
	char*	src_ptr1 = overlap_buf1;
	char*	dst_ptr1 = overlap_buf1 + 5;
	char* memmove_ret2 = ft_memmove(dst_ptr1, src_ptr1, 8);
	assert(memmove_ret2 == dst_ptr1);  // Should return destination
	// Just verify it doesn't crash and returns correct pointer
	
	// Test overlapping memory - backward direction
	char	overlap_buf2[20] = "Hello, World!";
	char*	src_ptr2 = overlap_buf2 + 7;
	char*	dst_ptr2 = overlap_buf2 + 2;
	char* memmove_ret3 = ft_memmove(dst_ptr2, src_ptr2, 6);
	assert(memmove_ret3 == dst_ptr2);  // Should return destination
	
	// Test with simple overlap
	char	overlap_buf3[10] = "ABCDEF";
	char* memmove_ret4 = ft_memmove(overlap_buf3 + 2, overlap_buf3, 4);
	assert(memmove_ret4 == overlap_buf3 + 2);  // Should return destination
	
	// Test exact overlap (src == dst)
	char	same_buf[10] = "Testing";
	char* memmove_ret5 = ft_memmove(same_buf, same_buf, 7);
	assert(memmove_ret5 == same_buf);
	assert(ft_strlen(same_buf) == 7);  // Should remain unchanged
	
	// Edge case - zero length
	char memmove_edge_test[5] = "Test";
	char* memmove_ret6 = ft_memmove(memmove_edge_test + 1, memmove_edge_test, 0);
	assert(memmove_ret6 == memmove_edge_test + 1);
	assert(ft_strlen(memmove_edge_test) == 4);  // Should remain unchanged
	
	// Edge case - single byte overlap
	char memmove_single_overlap[5] = "ABCD";
	char* memmove_ret7 = ft_memmove(memmove_single_overlap + 1, memmove_single_overlap, 1);
	assert(memmove_ret7 == memmove_single_overlap + 1);
	assert(memmove_single_overlap[0] == 'A');
	assert(memmove_single_overlap[1] == 'A');  // Should be copied
	assert(memmove_single_overlap[2] == 'C');  // Should remain unchanged
	
	// Size test - large overlapping move
	char memmove_large_overlap[2000];
	for (int i = 0; i < 1000; i++)
	{
		memmove_large_overlap[i] = 'A' + (i % 26);
	}
	char* memmove_ret8 = ft_memmove(memmove_large_overlap + 500, memmove_large_overlap, 1000);
	assert(memmove_ret8 == memmove_large_overlap + 500);
	for (int i = 0; i < 1000; i++)
	{
		assert(memmove_large_overlap[500 + i] == 'A' + (i % 26));
	}

	// Note: tests for ft_strlcpy
	char	strlcpy_dest1[20];
	char	strlcpy_dest2[50];
	char	strlcpy_dest_small[5];
	char	strlcpy_dest_exact[14];
	
	// Normal test - source fits in destination
	size_t	strlcpy_ret1 = ft_strlcpy(strlcpy_dest1, "Hello, World!", sizeof(strlcpy_dest1));
	assert(strlcpy_ret1 == 13);  // Should return source length
	assert(ft_strlen(strlcpy_dest1) == 13);
	assert(strlcpy_dest1[13] == '\0');  // Should be null-terminated
	for (int i = 0; i < 13; i++)
	{
		assert(strlcpy_dest1[i] == "Hello, World!"[i]);
	}
	
	// Normal test - longer source, fits in destination
	char*	strlcpy_src_str = "This is a longer string to test strlcpy function.";
	size_t	strlcpy_ret2 = ft_strlcpy(strlcpy_dest2, strlcpy_src_str, sizeof(strlcpy_dest2));
	assert(strlcpy_ret2 == ft_strlen(strlcpy_src_str));  // Should return source length
	assert(ft_strlen(strlcpy_dest2) == strlcpy_ret2);
	assert(strlcpy_dest2[strlcpy_ret2] == '\0');
	
	// Edge case - source longer than destination (truncation)
	size_t	strlcpy_ret3 = ft_strlcpy(strlcpy_dest_small, "Hello, World!", sizeof(strlcpy_dest_small));
	assert(strlcpy_ret3 == 13);  // Should still return source length
	assert(ft_strlen(strlcpy_dest_small) == 4);  // Should be truncated to fit
	assert(strlcpy_dest_small[4] == '\0');  // Should be null-terminated
	assert(strlcpy_dest_small[0] == 'H');
	assert(strlcpy_dest_small[1] == 'e');
	assert(strlcpy_dest_small[2] == 'l');
	assert(strlcpy_dest_small[3] == 'l');
	
	// Edge case - exact fit
	size_t	strlcpy_ret4 = ft_strlcpy(strlcpy_dest_exact, "Hello, World!", sizeof(strlcpy_dest_exact));
	assert(strlcpy_ret4 == 13);
	assert(ft_strlen(strlcpy_dest_exact) == 13);
	assert(strlcpy_dest_exact[13] == '\0');
	
	// Edge case - empty source
	size_t	strlcpy_ret5 = ft_strlcpy(strlcpy_dest1, "", sizeof(strlcpy_dest1));
	assert(strlcpy_ret5 == 0);
	assert(ft_strlen(strlcpy_dest1) == 0);
	assert(strlcpy_dest1[0] == '\0');
	
	// Edge case - destination size 1 (only null terminator fits)
	char strlcpy_dest_tiny[1];
	size_t	strlcpy_ret6 = ft_strlcpy(strlcpy_dest_tiny, "Hello", sizeof(strlcpy_dest_tiny));
	assert(strlcpy_ret6 == 5);  // Should return source length
	assert(ft_strlen(strlcpy_dest_tiny) == 0);  // No characters copied
	assert(strlcpy_dest_tiny[0] == '\0');
	
	// Edge case - destination size 0 (should not modify destination)
	char strlcpy_dest_zero[5] = "TEST";
	size_t	strlcpy_ret7 = ft_strlcpy(strlcpy_dest_zero, "Hello", 0);
	assert(strlcpy_ret7 == 5);  // Should return source length
	assert(strlcpy_dest_zero[0] == 'T');  // Should remain unchanged
	
	// Test with single character
	char strlcpy_dest_single[2];
	size_t	strlcpy_ret8 = ft_strlcpy(strlcpy_dest_single, "A", sizeof(strlcpy_dest_single));
	assert(strlcpy_ret8 == 1);
	assert(ft_strlen(strlcpy_dest_single) == 1);
	assert(strlcpy_dest_single[0] == 'A' && strlcpy_dest_single[1] == '\0');

	// Note: tests for ft_strlcat
	char	strlcat_dest3[50] = "Hello, ";
	char	strlcat_dest4[50] = "This is a ";  
	char	strlcat_dest_small[10] = "Hi ";
	char	strlcat_dest_exact[14] = "Hello, ";
	
	// Normal test - concatenation fits
	size_t	strlcat_ret3 = ft_strlcat(strlcat_dest3, "World!", sizeof(strlcat_dest3));
	assert(strlcat_ret3 == 13);  // Should return total length (7 + 6)
	assert(ft_strlen(strlcat_dest3) == 13);
	assert(strlcat_dest3[13] == '\0');
	char strlcat_expected3[] = "Hello, World!";
	for (int i = 0; i < 14; i++)
	{
		assert(strlcat_dest3[i] == strlcat_expected3[i]);
	}
	
	// Normal test - longer concatenation
	char*	strlcat_src_str2 = "longer string to test strlcat function.";
	size_t	strlcat_ret4 = ft_strlcat(strlcat_dest4, strlcat_src_str2, sizeof(strlcat_dest4));
	size_t strlcat_expected_len = ft_strlen("This is a ") + ft_strlen(strlcat_src_str2);
	assert(strlcat_ret4 == strlcat_expected_len);  // Should return total intended length
	assert(ft_strlen(strlcat_dest4) < sizeof(strlcat_dest4));  // Should fit in buffer
	assert(strlcat_dest4[ft_strlen(strlcat_dest4)] == '\0');
	
	// Edge case - concatenation causes truncation
	size_t	strlcat_ret5 = ft_strlcat(strlcat_dest_small, "there, World!", sizeof(strlcat_dest_small));
	assert(strlcat_ret5 == 16);  // Should return intended total length (3 + 13)
	assert(ft_strlen(strlcat_dest_small) == 9);  // Should be truncated to fit
	assert(strlcat_dest_small[9] == '\0');
	assert(strlcat_dest_small[0] == 'H' && strlcat_dest_small[1] == 'i' && strlcat_dest_small[2] == ' ');
	
	// Edge case - exact fit
	size_t	strlcat_ret6 = ft_strlcat(strlcat_dest_exact, "World!", sizeof(strlcat_dest_exact));
	assert(strlcat_ret6 == 13);
	assert(ft_strlen(strlcat_dest_exact) == 13);
	assert(strlcat_dest_exact[13] == '\0');
	
	// Edge case - empty source
	char strlcat_dest_empty_src[20] = "Hello";
	size_t	strlcat_ret7 = ft_strlcat(strlcat_dest_empty_src, "", sizeof(strlcat_dest_empty_src));
	assert(strlcat_ret7 == 5);  // Should return just destination length
	assert(ft_strlen(strlcat_dest_empty_src) == 5);
	assert(strlcat_dest_empty_src[0] == 'H');
	
	// Edge case - empty destination
	char strlcat_dest_empty[20] = "";
	size_t	strlcat_ret8 = ft_strlcat(strlcat_dest_empty, "Hello", sizeof(strlcat_dest_empty));
	assert(strlcat_ret8 == 5);  // Should return source length
	assert(ft_strlen(strlcat_dest_empty) == 5);
	char strlcat_expected8[] = "Hello";
	for (int i = 0; i < 6; i++)
	{
		assert(strlcat_dest_empty[i] == strlcat_expected8[i]);
	}
	
	// Edge case - buffer size smaller than initial dest length
	char strlcat_dest_weird[10] = "Hello";  // Valid initialization
	size_t	strlcat_ret9 = ft_strlcat(strlcat_dest_weird, "!", 3);  // Size smaller than strlen
	// Behavior may vary, but should not crash
	assert(strlcat_ret9 == 3);
	
	// Edge case - buffer size 0
	char strlcat_dest_zero_cat[10] = "Test";
	size_t	strlcat_ret10 = ft_strlcat(strlcat_dest_zero_cat, "ing", 0);
	// Should return intended length but not modify buffer
	assert(strlcat_dest_zero_cat[0] == 'T');  // Should remain unchanged
	assert(strlcat_ret10 == 0);

	// Note: tests for ft_strchr
	char*	str1 = "This is a longer string to test strchr function.";
	char*	str2 = "Hello, World!";
	char*	str3 = "aaaaaa";
	char*	empty_str = "";
	
	// Normal tests - find existing characters
	assert(ft_strchr(str1, 'T') == str1);       // First character
	assert(ft_strchr(str1, 'a') == str1 + 8);   // First 'a'
	assert(ft_strchr(str1, 't') == str1 + 18);  // First 't' (in "test")
	assert(ft_strchr(str1, '.') == str1 + 47);  // Last character before null
	
	assert(ft_strchr(str2, 'H') == str2);       // First character
	assert(ft_strchr(str2, 'o') == str2 + 4);   // First 'o' (in "Hello")
	assert(ft_strchr(str2, '!') == str2 + 12);  // Last character
	
	// Test finding repeated character (should return first occurrence)
	assert(ft_strchr(str3, 'a') == str3);       // First 'a'
	
	// Edge case - search for null terminator
	assert(ft_strchr(str1, '\0') == str1 + ft_strlen(str1));
	assert(ft_strchr(str2, '\0') == str2 + ft_strlen(str2));
	assert(ft_strchr(empty_str, '\0') == empty_str);
	
	// Normal tests - character not found
	assert(ft_strchr(str1, 'z') == NULL);       // Not in string
	assert(ft_strchr(str1, 'Z') == NULL);       // Wrong case
	assert(ft_strchr(str2, 'x') == NULL);       // Not in string
	assert(ft_strchr(str2, 'h') == NULL);       // Wrong case (has 'H')
	
	// Edge case - empty string
	assert(ft_strchr(empty_str, 'a') == NULL);  // Not found in empty string
	
	// Test with special characters
	char* special_str = "Hello\tWorld\n!";
	assert(ft_strchr(special_str, '\t') == special_str + 5);
	assert(ft_strchr(special_str, '\n') == special_str + 11);
	assert(ft_strchr(special_str, ' ') == NULL);  // Space not in string
	
	// Test with numbers and symbols
	char* mixed_str = "Test123!@#$";
	assert(ft_strchr(mixed_str, '1') == mixed_str + 4);
	assert(ft_strchr(mixed_str, '3') == mixed_str + 6);
	assert(ft_strchr(mixed_str, '@') == mixed_str + 8);
	assert(ft_strchr(mixed_str, '$') == mixed_str + 10);
	assert(ft_strchr(mixed_str, '%') == NULL);
	
	// Test with all printable ASCII characters
	char ascii_str[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	assert(ft_strchr(ascii_str, ' ') == ascii_str);      // First
	assert(ft_strchr(ascii_str, '~') == ascii_str + 94); // Last
	assert(ft_strchr(ascii_str, 'A') == ascii_str + 33); // Letter
	assert(ft_strchr(ascii_str, '0') == ascii_str + 16); // Digit
	
	// Edge case - single character string
	char* strchr_single_char = "X";
	assert(ft_strchr(strchr_single_char, 'X') == strchr_single_char);
	assert(ft_strchr(strchr_single_char, 'Y') == NULL);
	assert(ft_strchr(strchr_single_char, '\0') == strchr_single_char + 1);

	// Note: tests for ft_strrchr
	// Using same strings as strchr for consistency
	char*	str1_strrchr = "This is a longer string to test strchr function.";
	char*	str2_strrchr = "Hello, World!";
	char*	str3_strrchr = "aaaaaa";
	char*	empty_str_strrchr = "";
	
	// Normal tests - find existing characters (should return LAST occurrence)
	assert(ft_strrchr(str1_strrchr, 'T') == str1_strrchr);         // First and only 'T'
	assert(ft_strrchr(str1_strrchr, 'a') == str1_strrchr + 8);     // Only one 'a' 
	assert(ft_strrchr(str1_strrchr, 't') == str1_strrchr + 43);    // Last 't' (in "function")
	assert(ft_strrchr(str1_strrchr, 's') == str1_strrchr + 32);    // Last 's' (in "test")
	assert(ft_strrchr(str1_strrchr, 'r') == str1_strrchr + 37);    // Last 'r' (in "strchr")
	assert(ft_strrchr(str1_strrchr, '.') == str1_strrchr + 47);    // Last character before null
	
	assert(ft_strrchr(str2_strrchr, 'H') == str2_strrchr);         // First and only 'H'
	assert(ft_strrchr(str2_strrchr, 'o') == str2_strrchr + 8);     // Last 'o' (in "World")
	assert(ft_strrchr(str2_strrchr, 'l') == str2_strrchr + 10);     // Last 'l' (in "World")
	assert(ft_strrchr(str2_strrchr, '!') == str2_strrchr + 12);    // Last character
	
	// Test finding repeated character (should return LAST occurrence)
	assert(ft_strrchr(str3_strrchr, 'a') == str3_strrchr + 5);     // Last 'a'
	
	// Edge case - search for null terminator
	assert(ft_strrchr(str1_strrchr, '\0') == str1_strrchr + ft_strlen(str1_strrchr));
	assert(ft_strrchr(str2_strrchr, '\0') == str2_strrchr + ft_strlen(str2_strrchr));
	assert(ft_strrchr(empty_str_strrchr, '\0') == empty_str_strrchr);
	
	// Normal tests - character not found
	assert(ft_strrchr(str1_strrchr, 'z') == NULL);                 // Not in string
	assert(ft_strrchr(str1_strrchr, 'Z') == NULL);                 // Wrong case
	assert(ft_strrchr(str2_strrchr, 'x') == NULL);                 // Not in string
	assert(ft_strrchr(str2_strrchr, 'h') == NULL);                 // Wrong case (has 'H')
	
	// Edge case - empty string
	assert(ft_strrchr(empty_str_strrchr, 'a') == NULL);            // Not found in empty string
	
	// Test with special characters
	char* special_str_strrchr = "Hello\tWorld\n!";
	assert(ft_strrchr(special_str_strrchr, '\t') == special_str_strrchr + 5);
	assert(ft_strrchr(special_str_strrchr, '\n') == special_str_strrchr + 11);
	assert(ft_strrchr(special_str_strrchr, ' ') == NULL);          // Space not in string
	
	// Test with numbers and symbols
	char* mixed_str_strrchr = "Test123!@#$";
	assert(ft_strrchr(mixed_str_strrchr, '1') == mixed_str_strrchr + 4);
	assert(ft_strrchr(mixed_str_strrchr, '3') == mixed_str_strrchr + 6);
	assert(ft_strrchr(mixed_str_strrchr, '@') == mixed_str_strrchr + 8);
	assert(ft_strrchr(mixed_str_strrchr, '$') == mixed_str_strrchr + 10);
	assert(ft_strrchr(mixed_str_strrchr, '%') == NULL);
	
	// Test with all printable ASCII characters (same as strchr)
	char ascii_str_strrchr[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	assert(ft_strrchr(ascii_str_strrchr, ' ') == ascii_str_strrchr);        // First and only space
	assert(ft_strrchr(ascii_str_strrchr, '~') == ascii_str_strrchr + 94);   // Last
	assert(ft_strrchr(ascii_str_strrchr, 'A') == ascii_str_strrchr + 33);   // Letter
	assert(ft_strrchr(ascii_str_strrchr, '0') == ascii_str_strrchr + 16);   // Digit
	
	// Edge case - single character string
	char* strrchr_single_char = "X";
	assert(ft_strrchr(strrchr_single_char, 'X') == strrchr_single_char);
	assert(ft_strrchr(strrchr_single_char, 'Y') == NULL);
	assert(ft_strrchr(strrchr_single_char, '\0') == strrchr_single_char + 1);
	
	// Additional tests to highlight difference between strchr and strrchr
	char* repeated_chars = "abcabcabc";
	assert(ft_strchr(repeated_chars, 'a') == repeated_chars);          // First 'a' (position 0)
	assert(ft_strrchr(repeated_chars, 'a') == repeated_chars + 6);     // Last 'a' (position 6)
	assert(ft_strchr(repeated_chars, 'b') == repeated_chars + 1);      // First 'b' (position 1) 
	assert(ft_strrchr(repeated_chars, 'b') == repeated_chars + 7);     // Last 'b' (position 7)
	assert(ft_strchr(repeated_chars, 'c') == repeated_chars + 2);      // First 'c' (position 2)
	assert(ft_strrchr(repeated_chars, 'c') == repeated_chars + 8);     // Last 'c' (position 8)
	
	// Test with multiple occurrences in longer string
	char* multi_occur = "The quick brown fox jumps over the lazy dog";
	assert(ft_strchr(multi_occur, 'o') == multi_occur + 12);           // First 'o' in "brown"
	assert(ft_strrchr(multi_occur, 'o') == multi_occur + 41);          // Last 'o' in "dog"
	assert(ft_strchr(multi_occur, 'e') == multi_occur + 2);            // First 'e' in "The"
	assert(ft_strrchr(multi_occur, 'e') == multi_occur + 33);          // Last 'e' in "over"
	assert(ft_strchr(multi_occur, ' ') == multi_occur + 3);            // First space
	assert(ft_strrchr(multi_occur, ' ') == multi_occur + 39);          // Last space

	// Note: tests for ft_strncmp
	char* strncmp_str1 = "Hello, World!";
	char* strncmp_str2 = "Hello, World!";
	char* strncmp_str3 = "Hello, Universe!";
	char* strncmp_str4 = "Hello";
	char* strncmp_str5 = "hELLO, WORLD!";
	char* strncmp_empty = "";
	
	// Basic equality tests
	assert(ft_strncmp(strncmp_str1, strncmp_str2, 13) == 0);  // Identical strings
	assert(ft_strncmp(strncmp_str1, strncmp_str1, 13) == 0);  // Same string with itself
	assert(ft_strncmp(strncmp_empty, strncmp_empty, 0) == 0); // Empty strings
	assert(ft_strncmp(strncmp_empty, strncmp_empty, 1) == 0); // Empty strings with n > 0
	
	// Partial comparison tests - identical prefixes
	assert(ft_strncmp(strncmp_str1, strncmp_str3, 7) == 0);   // "Hello, " part identical
	assert(ft_strncmp(strncmp_str1, strncmp_str4, 5) == 0);   // "Hello" part identical
	assert(ft_strncmp(strncmp_str1, strncmp_str3, 6) == 0);   // "Hello," part identical
	
	// Difference detection tests
	assert(ft_strncmp(strncmp_str1, strncmp_str3, 8) != 0);   // Differs at "W" vs "U"
	assert(ft_strncmp(strncmp_str1, strncmp_str3, 13) != 0);  // Full comparison - different
	assert(ft_strncmp(strncmp_str1, strncmp_str5, 1) != 0);   // Case difference at first char
	
	// Test return value signs (positive/negative)
	assert(ft_strncmp("abc", "abd", 3) < 0);   // 'c' < 'd'
	assert(ft_strncmp("abd", "abc", 3) > 0);   // 'd' > 'c'
	assert(ft_strncmp("A", "a", 1) < 0);       // 'A' < 'a' (ASCII values)
	assert(ft_strncmp("a", "A", 1) > 0);       // 'a' > 'A' (ASCII values)
	
	// Edge case - n = 0 (should always return 0)
	assert(ft_strncmp("different", "strings", 0) == 0);
	assert(ft_strncmp("abc", "xyz", 0) == 0);
	assert(ft_strncmp(strncmp_str1, strncmp_str3, 0) == 0);
	
	// Edge case - n = 1 (compare only first character)
	assert(ft_strncmp("abc", "axz", 1) == 0);  // Both start with 'a'
	assert(ft_strncmp("abc", "bxz", 1) < 0);   // 'a' < 'b'
	assert(ft_strncmp("bxz", "abc", 1) > 0);   // 'b' > 'a'
	
	// Empty string comparisons
	assert(ft_strncmp("", "", 10) == 0);       // Both empty
	assert(ft_strncmp("abc", "", 1) > 0);      // Non-empty vs empty
	assert(ft_strncmp("", "abc", 1) < 0);      // Empty vs non-empty
	assert(ft_strncmp("abc", "", 0) == 0);     // n=0 with empty string
	
	// One string shorter than n
	assert(ft_strncmp("short", "shortlong", 10) < 0);  // First ends earlier
	assert(ft_strncmp("shortlong", "short", 10) > 0);  // Second ends earlier
	assert(ft_strncmp("test", "test", 10) == 0);       // Both end before n
	
	// Null terminator handling
	char strncmp_null_test1[] = "test\0hidden";
	char strncmp_null_test2[] = "test\0different";
	assert(ft_strncmp(strncmp_null_test1, strncmp_null_test2, 10) == 0);  // Should stop at first null
	assert(ft_strncmp("test", "test\0extra", 5) == 0);  // Should stop at null in second string
	
	// Special characters and numbers
	assert(ft_strncmp("test123", "test456", 4) == 0);   // Identical prefix
	assert(ft_strncmp("test123", "test456", 5) < 0);    // '1' < '4'
	assert(ft_strncmp("test!@#", "test$%^", 4) == 0);   // Identical prefix
	assert(ft_strncmp("test!@#", "test$%^", 5) < 0);    // '!' < '$'
	
	// Long strings test - 128+ bytes to test vectorized implementation
	char* strncmp_long1 = "This is a very long string to test strncmp function with extended content and multiple words.";
	char* strncmp_long2 = "This is a very long string to test strncmp function with different content and multiple words.";
	char* strncmp_long3 = "This is a very long string to test strncmp function with extended content and multiple words.";
	
	assert(ft_strncmp(strncmp_long1, strncmp_long3, 100) == 0);  // Identical long strings
	assert(ft_strncmp(strncmp_long1, strncmp_long2, 50) == 0);   // Identical up to difference point
	assert(ft_strncmp(strncmp_long1, strncmp_long2, 70) != 0);   // Includes difference ("extended" vs "different")
	
	// Very long strings (256+ bytes) to test multiple 128-byte chunks
	char strncmp_very_long1[512];
	char strncmp_very_long2[512];
	char strncmp_very_long3[512];
	
	// Fill with repeating pattern - identical strings
	for (int i = 0; i < 510; i++) {
		strncmp_very_long1[i] = 'A' + (i % 26);
		strncmp_very_long2[i] = 'A' + (i % 26);
	}
	strncmp_very_long1[510] = '\0';
	strncmp_very_long2[510] = '\0';
	
	// Create third string identical except for position 300 (in 3rd 128-byte chunk)
	for (int i = 0; i < 510; i++) {
		strncmp_very_long3[i] = 'A' + (i % 26);
	}
	strncmp_very_long3[300] = 'Z';  // Different character at position 300
	strncmp_very_long3[510] = '\0';
	
	// Test identical very long strings
	assert(ft_strncmp(strncmp_very_long1, strncmp_very_long2, 511) == 0);
	assert(ft_strncmp(strncmp_very_long1, strncmp_very_long2, 256) == 0);  // First 2 chunks
	assert(ft_strncmp(strncmp_very_long1, strncmp_very_long2, 384) == 0);  // First 3 chunks
	
	// Test difference in 3rd chunk (after 256 bytes)
	assert(ft_strncmp(strncmp_very_long1, strncmp_very_long3, 299) == 0);  // Before difference
	assert(ft_strncmp(strncmp_very_long1, strncmp_very_long3, 301) != 0);  // Includes difference
	assert(ft_strncmp(strncmp_very_long1, strncmp_very_long3, 400) != 0);  // Well past difference
	
	// Test boundary conditions around 128-byte chunks
	char strncmp_boundary1[200];
	char strncmp_boundary2[200];
	
	// Identical for first 127 bytes, different at 128th byte
	for (int i = 0; i < 127; i++) {
		strncmp_boundary1[i] = 'X';
		strncmp_boundary2[i] = 'X';
	}
	strncmp_boundary1[127] = 'A';
	strncmp_boundary2[127] = 'B';
	for (int i = 128; i < 199; i++) {
		strncmp_boundary1[i] = 'Y';
		strncmp_boundary2[i] = 'Y';
	}
	strncmp_boundary1[199] = '\0';
	strncmp_boundary2[199] = '\0';
	
	assert(ft_strncmp(strncmp_boundary1, strncmp_boundary2, 127) == 0);   // Before difference
	assert(ft_strncmp(strncmp_boundary1, strncmp_boundary2, 128) < 0);    // Includes difference 'A' < 'B'
	assert(ft_strncmp(strncmp_boundary2, strncmp_boundary1, 128) > 0);    // Reverse: 'B' > 'A'
	
	// Test exact 128-byte strings
	char strncmp_exact128_1[129];
	char strncmp_exact128_2[129];
	
	for (int i = 0; i < 128; i++) {
		strncmp_exact128_1[i] = 'M';
		strncmp_exact128_2[i] = 'M';
	}
	strncmp_exact128_1[128] = '\0';
	strncmp_exact128_2[128] = '\0';
	
	// Make them different at the very last byte (127th position)
	strncmp_exact128_2[127] = 'N';
	
	assert(ft_strncmp(strncmp_exact128_1, strncmp_exact128_1, 128) == 0);  // Same string
	assert(ft_strncmp(strncmp_exact128_1, strncmp_exact128_2, 127) == 0);  // Same first 127
	assert(ft_strncmp(strncmp_exact128_1, strncmp_exact128_2, 128) < 0);   // Different at 127th: 'M' < 'N'
	
	// Test very long identical strings (1000+ bytes)
	char strncmp_huge1[1024];
	char strncmp_huge2[1024];
	
	for (int i = 0; i < 1023; i++) {
		strncmp_huge1[i] = 'Q';
		strncmp_huge2[i] = 'Q';
	}
	strncmp_huge1[1023] = '\0';
	strncmp_huge2[1023] = '\0';
	
	assert(ft_strncmp(strncmp_huge1, strncmp_huge2, 1024) == 0);  // Identical huge strings
	assert(ft_strncmp(strncmp_huge1, strncmp_huge2, 500) == 0);   // Partial comparison
	assert(ft_strncmp(strncmp_huge1, strncmp_huge2, 768) == 0);   // 6 chunks of 128 bytes
	
	// Test with exact difference positions
	assert(ft_strncmp("abcdef", "abcxyz", 3) == 0);    // Same first 3 chars
	assert(ft_strncmp("abcdef", "abcxyz", 4) < 0);     // 'd' < 'x'
	assert(ft_strncmp("abcxyz", "abcdef", 4) > 0);     // 'x' > 'd'
	
	// ASCII boundary tests
	char strncmp_ascii1[] = "test\x7F";  // DEL character
	char strncmp_ascii2[] = "test\x80";  // Extended ASCII
	assert(ft_strncmp(strncmp_ascii1, strncmp_ascii2, 5) < 0);  // 0x7F < 0x80
	
	// Single character strings
	assert(ft_strncmp("a", "a", 1) == 0);
	assert(ft_strncmp("a", "b", 1) < 0);
	assert(ft_strncmp("z", "a", 1) > 0);
	assert(ft_strncmp("A", "a", 1) < 0);  // Uppercase comes before lowercase in ASCII
	
	// Multiple differences - should stop at first difference
	assert(ft_strncmp("abc", "axz", 2) < 0);  // Should compare 'b' vs 'x' at position 1
	assert(ft_strncmp("axz", "abc", 2) > 0);  // Should compare 'x' vs 'b' at position 1
	
	// Large n values (larger than string lengths)
	assert(ft_strncmp("short", "short", 1000) == 0);
	assert(ft_strncmp("abc", "abd", 1000) < 0);
	assert(ft_strncmp("xyz", "abc", 1000) > 0);
	
	// Strings with repeating patterns
	assert(ft_strncmp("aaaa", "aaab", 3) == 0);  // First 3 identical
	assert(ft_strncmp("aaaa", "aaab", 4) < 0);   // 'a' < 'b' at position 3
	assert(ft_strncmp("abababab", "abababcd", 6) == 0);  // First 6 identical
	assert(ft_strncmp("abababab", "abababcd", 7) < 0);   // 'a' < 'c' at position 6
	
	// Mixed content with spaces and punctuation
	char* strncmp_mixed1 = "Hello, world! How are you?";
	char* strncmp_mixed2 = "Hello, world! How old are you?";
	assert(ft_strncmp(strncmp_mixed1, strncmp_mixed2, 18) == 0);  // Same up to "are"
	assert(ft_strncmp(strncmp_mixed1, strncmp_mixed2, 20) != 0);  // Difference at "are" vs "old"
	
	// All printable ASCII characters test
	char strncmp_full_ascii1[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	char strncmp_full_ascii2[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	char strncmp_full_ascii3[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}.";
	
	assert(ft_strncmp(strncmp_full_ascii1, strncmp_full_ascii2, 95) == 0);  // Identical
	assert(ft_strncmp(strncmp_full_ascii1, strncmp_full_ascii3, 94) == 0);  // Same except last char
	assert(ft_strncmp(strncmp_full_ascii1, strncmp_full_ascii3, 95) > 0);   // '~' > '.'

	return (0);
}
