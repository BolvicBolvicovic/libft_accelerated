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

void* ft_memcpy_old(void* s, int c, int n)
{
    extern void *ft_memcpy(void* s, int c, int n);
    return ft_memcpy(s, c, n);
}
