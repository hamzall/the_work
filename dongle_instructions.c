/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_instructions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:42:30 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/27 14:35:08 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int get_arrival(t_simulation *sim)
{
    int res;

    pthread_mutex_lock(&sim->arr_lock);
    res = sim->arr_counter++;
    pthread_mutex_unlock(&sim->arr_lock);

    return res;
}


static long get_deadline(t_coder *coder)
{
    long res;

    pthread_mutex_lock(&coder->lock);
    res = coder->simulation->time_burnout + coder->last_compile;
    pthread_mutex_unlock(&coder->lock);

    return res;
}


static int is_ready(t_req request, t_dongle *dongle)
{
    t_req root;

    if (get_root(&dongle->the_heap, &root) == 0)
        return 0;
    
    if (root.arrival == request.arrival && root.coder_id == request.coder_id && root.deadline == request.deadline)
    {
        if (dongle->held_by == 0 && dongle->cooldown_to <= get_current_time())
            return 1;
        else
            return 0;
    }
    else
        return 0;
}


int take_dongle(t_coder *coder, t_dongle *dongle)
{
    t_simulation *sim;
    sim = coder->simulation;

    t_req request;

    request.arrival = get_arrival(sim);
    request.coder_id = coder->id;
    request.deadline = get_deadline(coder);


    pthread_mutex_lock(&dongle->mutex);
    if (heap_push(&dongle->the_heap, request, sim) == 0)
    {
        pthread_mutex_unlock(&dongle->mutex);
        return 0;
    }

    while (get_stop_value(sim) == 0)
    {
        if (is_ready(request, dongle))
        {
            dongle->held_by = coder->id;
            heap_pop(sim, &dongle->the_heap, &request);
            pthread_mutex_unlock(&dongle->mutex);
            log_status(sim, coder->id, TAKE_DONGLE);
            return 1;
        }
        else if (dongle->cooldown_to > get_current_time())
        {
            long wait_ms = dongle->cooldown_to - get_current_time();
			struct timeval tv;
			struct timespec ts;
			long add_ns;

			gettimeofday(&tv, NULL);
			ts.tv_sec = tv.tv_sec;
			ts.tv_nsec = tv.tv_usec * 1000L;

			ts.tv_sec += wait_ms / 1000;
			add_ns = (wait_ms % 1000) * 1000000L;
			ts.tv_nsec += add_ns;

			if (ts.tv_nsec >= 1000000000L)
			{
				ts.tv_sec += 1;
				ts.tv_nsec -= 1000000000L;
			}
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
        }
        else
        {
            pthread_cond_wait(&dongle->cond, &dongle->mutex);
        }
    }
    heap_remove_req(sim, &dongle->the_heap, request);
    pthread_mutex_unlock(&dongle->mutex);
    
    return 0;
}
