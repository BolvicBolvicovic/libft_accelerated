/* ************************************************************************** */
/*                                                                            */
/*                       OLD LIBFT IMPLEMENTATIONS                           */
/*                                                                            */
/* ************************************************************************** */

// This file contains the actual old implementations with renamed functions
// to avoid symbol conflicts with the new optimized versions

#include "benchmark_common.h"

// Old strlen implementation - prevent optimization with volatile
size_t ft_strlen_old(const char *s)
{
    volatile size_t len = 0;  // volatile prevents optimization
    volatile const char *ptr = s;  // volatile prevents optimization
    
    while (*ptr) {
        len++;
        ptr++;
    }
    return len;
}

// Old memset implementation - prevent optimization
void *ft_memset_old(void *s, int c, size_t n)
{
    volatile unsigned char *str = (unsigned char *)s;  // volatile prevents optimization
    volatile size_t i = 0;

    while (i < n)
    {
        str[i] = (unsigned char)c;
        i++;
    }
    return s;
}

// Old memcpy implementation - prevent optimization
void *ft_memcpy_old(void *dst, const void *src, size_t n)
{
    volatile unsigned char *d;
    volatile const unsigned char *s;
    volatile size_t i;

    if (!dst && !src)
        return NULL;
    
    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    i = 0;
    while (i < n)
    {
        d[i] = s[i];
        i++;
    }
    return dst;
}

// Old strchr implementation
char *ft_strchr_old(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return ((char *)s);
        s++;
    }
    if ((char)c == '\0')
        return ((char *)s);
    return (NULL);
}

// Old strncmp implementation
int ft_strncmp_old(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && (s1[i] || s2[i]))
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return (0);
}

// Old memchr implementation
void *ft_memchr_old(const void *s, int c, size_t n)
{
    const char *str;
    size_t i;

    str = (const char *)s;
    i = 0;
    while (i < n)
    {
        if (str[i] == (char)c)
            return ((void *)(str + i));
        i++;
    }
    return (NULL);
}