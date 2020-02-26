/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   union.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarras <micarras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 12:25:59 by micarras          #+#    #+#             */
/*   Updated: 2020/02/03 12:45:04 by micarras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

size_t
    ft_strlen(char *str)
{
    size_t  i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char *
    ft_remdu(char *str)
{
    size_t  i;
    size_t  j;
    size_t  k;

    i = 0;
    while (i < ft_strlen(str))
    {
        j = i + 1;
        while (str[j])
        {
            if (str[j] == str[i])
            {
                k = j;
                while (str[k])
                {
                    str[k] = str[k + 1];
                    k++;
                }
            }
            j++;
        }
        i++;
    }
    return (str);
}

int
    main(int argc, char **argv)
{
    const char  bn = '\n';
    char        *str1;
    char        *str2;
    size_t      i;
    size_t      j;

    if (argc == 3)
    {
        str1 = ft_remdu(argv[1]);
        str2 = ft_remdu(argv[2]);
        i = 0;
        write(1, str1, ft_strlen(str1));
        while (str2[i])
        {
            j = 0;
            while (str1[j])
            {
                if (str2[i] == str1[j])
                    break;
                j++;
            }
            if (j == ft_strlen(str1))
                write(1, &str2[i], 1);
            i++;
        }
        write(1, &bn, 1);
    }
    else
        write(1, &bn, 1);
}
