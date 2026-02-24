/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:47:17 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 21:32:33 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


long get_current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

void sleep_by_ms(long ms)
{
    long start;

    start = get_current_time();

    while (get_current_time() - start < ms)
    {
        usleep(75);
    }
}
