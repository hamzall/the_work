/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_instructions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:42:30 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/26 17:39:43 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int take_ticket(t_simulation *sim)
{
    int res;
    
    pthread_mutex_lock(&sim->arr_lock);
    res = sim->arr_counter++;
    pthread_mutex_unlock(&sim->arr_lock);
    return res;
}


static int is_read(t_dongle *dongle, t_req request, long current_time)
{
    t_req *root;
    
    if (get_root(&dongle->the_heap, root) == 0)
        return 0;

    if (root->arrival == request.arrival && root->coder_id == request.coder_id && root->deadline == request.deadline)
    {
        if (dongle->held_by == 0 && dongle->cooldown_to < current_time)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}


static int coder_deadline(t_coder *coder)
{
    long deadline;

    pthread_mutex_lock(&coder->lock);
    deadline = coder->simulation->time_burnout + coder->last_compile;
    pthread_mutex_unlock(&coder->lock);
    return deadline;
}


int take_dongle(t_coder *coder, t_dongle *dongle)
{
    t_req req;

    req.arrival = take_ticket(coder->simulation);
    req.coder_id = coder->id;
    req.deadline = coder_deadline(coder);

    
    
}

