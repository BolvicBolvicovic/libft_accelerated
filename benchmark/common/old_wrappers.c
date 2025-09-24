/* ************************************************************************** */
/*                                                                            */
/*                            OLD LIBFT WRAPPERS                             */
/*                                                                            */
/* ************************************************************************** */

// These are wrapper functions that will be linked with the old libft library
// They provide access to old implementations with different names to avoid conflicts

int ft_strlen_old(const char* str)
{
    extern int ft_strlen(const char* str);
    return ft_strlen(str);
}

void* ft_memset_old(void* s, int c, int n)
{
    extern void* ft_memset(void *s, int c, int n);
    return ft_memset(s, c, n);
}

void* ft_memcpy_old(void* dst, char* src, int n)
{
    extern void *ft_memcpy(void* dst, char* src, int n);
    return ft_memcpy(dst, src, n);
}

char* ft_strchr_old(const char* dst, char c)
{
    extern char* ft_strchr(const char* dst, char c);
    return ft_strchr(dst, c);
}
