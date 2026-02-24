/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 11:49:21 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 02:59:16 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int is_digit(char c)
{
    return (c >= 48 && c <= 57);
}


static int is_all_digit(char *str)
{
    if (!str || !*str)
        return 0;
    
    int i = 0;
    if (str[i] == '+')
        i++;
    if (is_digit(str[i]) == 0)
        return 0;
    
    while (str[i])
    {
        if (is_digit(str[i]) == 0)
        {
            return 0;
        }
        i++;
    }
    return 1;
}


static long ft_atol(char *str)
{
    long res;
    int i;

    if (is_all_digit(str) == 0)
        return 0;

    res = 0;
    i = 0;

    if (str[i] == '+')
        i++;

    while (str[i])
	{
		if (res > (LONG_MAX - (str[i] - 48)) / 10)
			return (-1);
		res = res * 10 + (str[i] - 48);
		i++;
	}

	return (res);
}



static int get_the_type(t_type *to_fill, char *str_type)
{
    if (strcmp(str_type, "fifo") == 0)
    {
        *to_fill = CODEX_FIFO;
        return 1;
    }
    if (strcmp(str_type, "edf") == 0)
    {
        *to_fill = CODEX_EDF;
        return 1;
    }
    return 0;
}



int parsing(t_simulation *simulation, int argc, char **argv)
{
    if (argc != 9)
        return 0;
    
    simulation->num_coders = ft_atol(argv[1]);
    simulation->time_burnout = ft_atol(argv[2]);
    simulation->time_compile = ft_atol(argv[3]);
    simulation->time_debug = ft_atol(argv[4]);
    simulation->time_refactor = ft_atol(argv[5]);
    simulation->total_compiles = ft_atol(argv[6]);
    simulation->cooldown = ft_atol(argv[7]);

    if (get_the_type(&simulation->the_type, argv[8]) == 0)
        return 0;

    if (simulation->num_coders <= 0 || simulation->time_burnout <= 0 || simulation->time_compile <= 0
        || simulation->time_debug <= 0 || simulation->time_refactor <= 0 || simulation->total_compiles <= 0
        || simulation->cooldown < 0)
        {
            return 0;
        }
    
    return 1;
}

