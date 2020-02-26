/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarras <micarras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:49:22 by micarras          #+#    #+#             */
/*   Updated: 2020/02/19 18:34:19 by micarras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t
    ft_strlen(const char *str)
{
    size_t  i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char
    *ft_strcpy(char *dst, const char *src)
{
    char        *dst_p;
    char        *src_p;
    size_t      len;

    if (!dst || !src)
        return (NULL);
    len = ft_strlen(src);
    dst_p = (char *)dst;
    src_p = (char *)src;
    while(len--)
        *dst_p++ = *src_p++;
    *dst_p = '\0';
    return (dst);
}

char
    *ft_strdup(const char *str, size_t len)
{
    char        *dst;

    if (!str)
        return (NULL);
    if (!(dst = (char *)malloc(len + 1)))
        return (NULL);
    ft_strcpy(dst, str);
    return (dst);
}

char
    *ft_strjoin(char *dst, const char *src)
{
    size_t      dst_len;
    size_t      src_len;
    char        *tmp;

    dst_len = ft_strlen(dst);
    src_len = ft_strlen(src);
    tmp = ft_strdup(dst, dst_len);
    free(dst);
    if (!(dst = (char *)malloc(dst_len + src_len + 1)))
        return (NULL);
    ft_strcpy(dst, tmp);
    free(tmp);
    ft_strcpy(dst + dst_len, src);
    return (dst);
}

int
    get_next_line(char **line)
{
    static char buff[1];
    int         ret;

    if (!line)
        return (-1);
    *line = ft_strdup("", 0);
    while ((ret = read(0, buff, 1)) > 0)
    {
        if (buff[0] == '\n')
            return (1);
        if (!(*line = ft_strjoin(*line, buff)))
            return (-1);
    }
    return (ret);
}
