/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarras <micarras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 12:07:48 by micarras          #+#    #+#             */
/*   Updated: 2020/02/03 12:24:58 by micarras         ###   ########.fr       */
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
        while (str1[i])
        {
            j = 0;
            while (str2[j])
            {
                if (str1[i] == str2[j])
                {
                    write(1, &str1[i], 1);
                    break ;
                }
                j++;
            }
            i++;
        }
        write(1, &bn, 1);
    }
    else
        write(1, &bn, 1);
}
