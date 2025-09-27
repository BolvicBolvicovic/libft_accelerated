#include "../libft.h"

extern int* __errno_location(void);

__always_inline ssize_t
ft_write(int fd, const void* buf, size_t count)
{
	ssize_t ret;

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
	asm volatile (
		"syscall"			// Notes:
		: "=a" (ret)                    // Output: rax -> ret
		: "a" (1),                      // Input: write syscall number in rax
		  "D" (fd),                     // Input: fd in rdi  
		  "S" (buf),                    // Input: buf in rsi
		  "d" (count)                   // Input: count in rdx
		: "rcx", "r11", "memory"        // Clobbered by syscall:
						// RCX: Used internally by syscall to store return address
						// R11: Gets the RFLAGS register value
	);
#elif defined(__aarch64__) || defined(_M_ARM64)
    asm volatile (
        "svc #0"
        : "=r" (ret)
        : "r" (fd), "r" (buf), "r" (count), "r" (64)
        : "memory"
    );
#endif

	if (ret < 0)
	{
		*__errno_location() = -ret;
		return -1;
	}
	
	return ret;
}

inline void
ft_printline(int fd, char* s)
{
	ft_write(fd, s, ft_strlen(s));
}