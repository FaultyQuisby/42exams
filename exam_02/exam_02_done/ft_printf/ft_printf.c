/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarras <micarras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:25:05 by micarras          #+#    #+#             */
/*   Updated: 2020/02/20 12:10:59 by micarras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <inttypes.h>
#define FT_DARRAY_INITIAL_SIZE 16
#define FT_DARRAY_FACTOR 2

typedef struct          s_darray
{
    void                *data;
    size_t              size;
    size_t              max;
    size_t              type_size;
}                       t_darray;

typedef struct          s_modifier
{
    char                conversion;
    int                 size;
    int                 mindex;
    char                minus;
    int                 precision;
}                       t_modifier;

size_t
    ft_strlen(char *str)
{
    size_t  i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

void
    *ft_memcpy(void *dst, const void *src, size_t len)
{
    unsigned char *dst_p;
    unsigned char *src_p;

    dst_p = (unsigned char *)dst;
    src_p = (unsigned char *)src;
    while (len-- > 0)
        *dst_p++ = *src_p++;
    return (dst);
}

void
    *ft_memmove(void *dst, const void *src, size_t len)
{
    char        *dst_p;
    const char  *src_p;

    dst_p = (char *)dst;
    src_p = (char *)src;
    if (src_p > dst_p)
    {
        while (len--)
            dst_p[len] = src_p[len];
    }
    else
        ft_memcpy(dst, src, len);
    return (dst);
}

char
    *ft_calloc(size_t nmemb, size_t size)
{
    char        *ptr;
    size_t      i;

    i = 0;
    if (nmemb == 0 || size == 0)
        if (!(ptr = malloc(1)))
            return (NULL);
    if (!(ptr = malloc(nmemb * size)))
        return (NULL);
    while (i < (nmemb * size))
    {
        ptr[i] = 0;
        i++;
    }
    return (ptr);
}

void
    ft_darray_reappend(t_darray *self, size_t beginning, size_t end)
{
    void            *tmp;
    const size_t    portion_len = end - beginning;
    const size_t    type_size = self->type_size;

    tmp = ft_calloc(portion_len, type_size);
    ft_memcpy(tmp, self->data + beginning * type_size, portion_len * type_size);
    ft_memmove(self->data + beginning * type_size, self->data + end * type_size,
            (self->size - end) * type_size);
    ft_memcpy(self->data + (self->size - portion_len) * type_size,
            tmp, portion_len * type_size);
    free(tmp);
    return ;
}

int
    ft_darray_resize(t_darray *self, size_t new_size)
{
    void            *tmp;
    const size_t    type_size = self->type_size;

    if(!(tmp = malloc(new_size * type_size)))
        return (1);
    if (self->size > new_size)
        self->size = new_size;
    ft_memcpy(tmp, self->data, self->size * type_size);
    free(self->data);
    self->data = tmp;
    self->max = new_size;
    return (0);
}

int
    ft_darray_append(t_darray *self, void *data, size_t data_len)
{
    size_t          new_size;
    const size_t    total_size = self->size + data_len;
    const size_t    type_size = self->type_size;

    if (total_size > self->max)
    {
        new_size = (self->max > 0 ? self->max : FT_DARRAY_INITIAL_SIZE);
        while (new_size < total_size)
            new_size *= FT_DARRAY_FACTOR;
        ft_darray_resize(self, new_size);
    }
    ft_memcpy(self->data + self->size * type_size, data, data_len * type_size);
    self->size += data_len;
    return (0);
}

int
    ft_darray_trim(t_darray *self)
{
    ft_darray_append(self, "\0", 1);
    if (self->max > self->size)
        return (ft_darray_resize(self, self->size));
    return (0);

}

int
    ft_itoa_base(t_darray *d, int number, int base)
{
    int                 res;
    const char *const   base_str = "0123456789abcdef";

    res = 1;
    if (number <= -base || base <= number)
        res += ft_itoa_base(d, number / base, base);
    ft_darray_append(d, (void *)(base_str + ((number % base) < 0 ?
                -(number % base) : number % base)), 1);
    return (res);
}

int
    ft_uitoa_base(t_darray *d, uintmax_t number, int base)
{
    int                 res;
    const char *const   base_str = "0123456789abcdef";

    res = 1;
    if ((uintmax_t)base <= number)
        res += ft_itoa_base(d, (intmax_t)(number / base), base);
    ft_darray_append(d, (void *)base_str + ((size_t)(number % base)), 1);
    return (res);
}

int
    pf_convert_spec_s(t_modifier *m, t_darray *d, va_list ap)
{
    char    *arg;
    int     res;

    if (!(arg = va_arg(ap, char *)))
        arg = "(null)";
    res = (m->precision >= 0 ?
            ((ft_strlen(arg) < (size_t)m->precision) ?
            ft_strlen(arg) : (size_t)m->precision) : ft_strlen(arg));
    m->precision = -1;
    ft_darray_append(d, (void *)arg, res);
    return (res);
}

int
    pf_convert_spec_d(t_modifier *m , t_darray *d, va_list ap)
{
    intmax_t    arg;
    int         res;

    if ((arg = va_arg(ap, int)) < 0)
        ft_darray_append(d, "-", 1);
    if (arg == 0 && m->precision == 0)
        return (0);
    res = ft_itoa_base(d, arg, 10);
    return (res);
}

int
    pf_convert_spec_x(t_modifier *m, t_darray *d, va_list ap)
{
    uintmax_t   arg;
    int         res;

    arg = va_arg(ap, unsigned int);
    if (arg == 0 && m->precision == 0)
        return (0);
    res = ft_uitoa_base(d, arg, 16);
    return (res);
}

int
    pf_print(t_modifier *m, t_darray *d, va_list ap)
{
    int         i;
    const char  *c = "sdx";
    void *const t[] = { &pf_convert_spec_s,
                        &pf_convert_spec_d,
                        &pf_convert_spec_x };

    i = 0;
    while (c[i] && m->conversion != c[i])
        i++;
    if (c[i])
        return (((int (*)())t[i])(m, d, ap));
    ft_darray_append(d, &m->conversion, 1);
    m->precision = -1;
    return (1);
}

int
    pf_precision(t_modifier *m, t_darray *d, va_list ap)
{
    size_t      after;
    int         width;
    int         len;

    if ((width = pf_print(m, d, ap)) == -1)
        return (-1);
    after = d->size;
    len = width;
    if (len < m->precision && (m->conversion == 'd' || m->conversion == 'x'))
    {
        while (len < m->precision && ++len)
            ft_darray_append(d, "0", 1);
        ft_darray_reappend(d, after - width, after);
    }
    if (m->size < 0 && m->precision == -1)
    {
        m->size = (m->size < 0 ? -(m->size) : (m->size));
        m->minus = 1;
    }
    return (len);
}

int
    pf_convert(t_modifier *m, t_darray *d, va_list ap)
{
    size_t      before;
    size_t      after;
    int         width;
    int         len;

    before = d->size;
    width = pf_precision(m, d, ap);
    after = d->size;
    len = after - before;
    if (len <= m->size && width != -1)
        while (len < m->size && ++len)
            ft_darray_append(d, " ", 1);
    ft_darray_reappend(d, before, after);
    return (width);
}

char
    *pf_next_specifier(const char *s, t_darray *d)
{
    char        *p;

    p = (char *)s;
    while (*p && *p != '%')
        p++;
    if (p != s)
        ft_darray_append(d, (void *)s, p - s);
    return (p);
}

const char
    *pf_match(const char *s, t_modifier *m)
{
    *m = (t_modifier){0, 0, 0, 0, -1};
    while(*(++s))
    {
        if (*s == '.')
        {
            m->precision = 0;
            s++;
            while (*s >= '0' && *s <= '9')
                m->precision = 10 * (m->precision) + *s++ - '0';
            s--;
        }
        else if (*s >= '0' && *s <= '9')
        {
            m->size = 0;
            while (*s >= '0' && *s <= '9')
                m->size = 10 * (m->size) + *s++ - '0';
            s--;
        }
        else if ((m->conversion = *s))
            return (s + 1);
    }
    return (s);
}

int
    ft_printf(const char *fmt, ...)
{
    va_list     ap;
    va_list     dap;
    char        *s;
    char        *ret;
    int         byte_count;
    t_modifier  m;
    t_darray    d;

    s = (char *)fmt;
    va_start(ap, fmt);
    va_copy(dap, ap);
    d = (t_darray){ NULL, 0, 0, sizeof(char) };
    while (*s)
    {
        if (*s == '%')
        {
            s = (char *)pf_match(s, &m);
            if (m.conversion && pf_convert(&m, &d, dap) == -1)
            {
                ft_darray_resize(&d, m.mindex);
                ft_darray_trim(&d);
                ret = d.data;
                return (-1);
            }
            m.mindex = d.size;
        }
        s = pf_next_specifier(s, &d);
    }
    ft_darray_trim(&d);
    ret = d.data;
    if ((byte_count = d.size - 1) == -1)
        write(1, ret, ft_strlen(ret));
    else
        write(1, ret, byte_count);
    free (ret);
    return (byte_count);
}
