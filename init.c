/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 14:29:33 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 02:51:31 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_global_mutexes(t_simulation *s)
{
	pthread_mutex_destroy(&s->stop_lock);
	pthread_mutex_destroy(&s->log_lock);
	pthread_mutex_destroy(&s->arr_lock);
	pthread_mutex_destroy(&s->ready_lock);
}

int	init_global_mutexes(t_simulation *s)
{
	if (pthread_mutex_init(&s->stop_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&s->log_lock, NULL) != 0)
		return (pthread_mutex_destroy(&s->stop_lock), 0);
	if (pthread_mutex_init(&s->arr_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&s->stop_lock);
		pthread_mutex_destroy(&s->log_lock);
		return (0);
	}
	if (pthread_mutex_init(&s->ready_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&s->stop_lock);
		pthread_mutex_destroy(&s->log_lock);
		pthread_mutex_destroy(&s->arr_lock);
		return (0);
	}
	return (1);
}

static void	init_dongle_defaults(t_dongle *d)
{
	d->held_by = 0;
	d->cooldown_to = 0;
	d->the_heap.arr = NULL;
	d->the_heap.current_size = 0;
	d->the_heap.max_size = 0;
}

static void	destroy_dongles_upto(t_simulation *s, int last_ok)
{
	int	i;

    if (!s->dongles)
        return;


    if (last_ok >= 0)
    {
	    i = 0;
	    while (i <= last_ok)
	    {
		    pthread_mutex_destroy(&s->dongles[i].mutex);
		    pthread_cond_destroy(&s->dongles[i].cond);
		    free_heap(&s->dongles[i].the_heap);
		    i++;
	    }
    }
	free(s->dongles);
	s->dongles = NULL;
}

static int	init_one_dongle(t_simulation *s, int i)
{
	if (pthread_mutex_init(&s->dongles[i].mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&s->dongles[i].cond, NULL) != 0)
		return (pthread_mutex_destroy(&s->dongles[i].mutex), 0);
	if (create_heap(&s->dongles[i].the_heap, 4) == 0)
	{
		pthread_cond_destroy(&s->dongles[i].cond);
		pthread_mutex_destroy(&s->dongles[i].mutex);
		return (0);
	}
	return (1);
}

int	init_dongles(t_simulation *s)
{
	int	i;

	s->dongles = (t_dongle *)malloc(sizeof(t_dongle) * s->num_coders);
	if (!s->dongles)
		return (0);
	i = 0;
	while (i < s->num_coders)
		init_dongle_defaults(&s->dongles[i++]);
	i = 0;
	while (i < s->num_coders)
	{
		if (!init_one_dongle(s, i))
			return ((i > 0) ? destroy_dongles_upto(s, i - 1) : (free(s->dongles),
				s->dongles = NULL), 0);
		i++;
	}
	return (1);
}

static void	destroy_coders_upto(t_simulation *s, int last_ok)
{
	int	i;

    if (!s->coders)
        return;

    if (last_ok >= 0)
    {
	    i = 0;
	    while (i <= last_ok)
		    pthread_mutex_destroy(&s->coders[i++].lock);
    }
	free(s->coders);
	s->coders = NULL;
}

static void	fill_coder(t_simulation *s, int i)
{
	s->coders[i].id = i + 1;
	s->coders[i].simulation = s;
	s->coders[i].compiling_counter = 0;
	s->coders[i].last_compile = 0;
	s->coders[i].left = &s->dongles[i];
    if (s->num_coders == 1)
		s->coders[i].right = NULL;
	else
		s->coders[i].right = &s->dongles[(i + 1) % s->num_coders];
}

int	init_coders(t_simulation *s)
{
	int	i;

	s->coders = (t_coder *)malloc(sizeof(t_coder) * s->num_coders);
	if (!s->coders)
		return (0);
	i = 0;
	while (i < s->num_coders)
	{
		fill_coder(s, i);
		if (pthread_mutex_init(&s->coders[i].lock, NULL) != 0)
        {
            if (i > 0)
                destroy_coders_upto(s, i - 1);
            else
            {
                free(s->coders);
                s->coders = NULL;
            }
            return 0;
        }
		i++;
	}
	return (1);
}

int	initialize_data(t_simulation *s)
{
	s->is_stoped = 0;
	s->arr_counter = 0;
	s->ready_count = 0;
	if (!init_global_mutexes(s))
		return (0);
	if (pthread_cond_init(&s->ready_cond, NULL) != 0)
		return (destroy_global_mutexes(s), 0);
	if (!init_dongles(s))
		return (pthread_cond_destroy(&s->ready_cond), destroy_global_mutexes(s), 0);
	if (!init_coders(s))
	{
		destroy_dongles_upto(s, s->num_coders - 1);
		pthread_cond_destroy(&s->ready_cond);
		destroy_global_mutexes(s);
		return (0);
	}
	return (1);
}

