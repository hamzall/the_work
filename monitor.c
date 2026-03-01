/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 09:42:48 by hel-achh          #+#    #+#             */
/*   Updated: 2026/03/01 11:07:35 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// typedef struct s_coder
// {
// 	int				id;
// 	pthread_t		thread;

// 	pthread_mutex_t	lock;
// 	long			last_compile;
// 	int				compiling_counter;

// 	t_dongle		*left;
// 	t_dongle		*right;

// 	struct s_simulation	*simulation;
// }	t_coder;


static long get_time_last_compile(t_coder *coder)
{
    long time;
    
    pthread_mutex_lock(&coder->lock);
    time = coder->last_compile;
    pthread_mutex_unlock(&coder->lock);

    return time;
}


void *monitor_routine(void *arg)
{






    return NULL;
}