/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displays.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 03:13:00 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/25 13:59:05 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static char *get_a_status(t_status status)
{
    if (status == TAKE_DONGLE)
        return "has taken a dongle";
    if (status == COMPILING)
        return "is compiling";
    if (status == DEBUGGING)
        return "is debugging";
    if (status == REFACTORING)
        return "is refactoring";
    if (status == BURNED_OUT)
        return "burned out";
    return "Unknown Status";
}


int get_stop_value(t_simulation *sim)
{
    int value;
    
    pthread_mutex_lock(&sim->stop_lock);
    value = sim->is_stoped;
    pthread_mutex_unlock(&sim->stop_lock);
    return value;
}


void set_stop_value(t_simulation *sim, int value)
{
    pthread_mutex_lock(&sim->stop_lock);
    sim->is_stoped = value;
    pthread_mutex_unlock(&sim->stop_lock);
}


void wake_up_all_threads(t_simulation *sim)
{
    int i;

    i = 0;
    while (i < sim->num_coders)
    {
        pthread_mutex_lock(&sim->dongles[i].mutex);
        pthread_cond_broadcast(&sim->dongles[i].cond);
        pthread_mutex_unlock(&sim->dongles[i++].mutex);
    }
}


void log_status(t_simulation *sim, int coder_id, t_status st)
{
    long t;
    int res;

    if (get_stop_value(sim) && st != BURNED_OUT)
        return;

    
    // res = get_stop_value(sim);
    pthread_mutex_lock(&sim->log_lock);

    if (get_stop_value(sim) && st != BURNED_OUT)
    {
        pthread_mutex_unlock(&sim->log_lock);
        return;
    }

    t = get_current_time() - sim->start_ms;
    printf("%ld %d %s\n", t, coder_id, get_a_status(st));

    pthread_mutex_unlock(&sim->log_lock);
}
