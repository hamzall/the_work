/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 02:51:11 by hel-achh          #+#    #+#             */
/*   Updated: 2026/03/01 07:08:04 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int are_all_complete_compiles_time(t_simulation *sim)
{
    int i;

    i = 0;
    while (i < sim->num_coders)
    {
        pthread_mutex_lock(&sim->coders[i].lock);
        if (sim->coders[i].compiling_counter < sim->total_compiles)
        {
            pthread_mutex_unlock(&sim->coders[i].lock);
            return 0;
        }
        pthread_mutex_unlock(&sim->coders[i].lock);
        i++;
    }
    return 1;
}


static void increase_compiles(t_coder *coder)
{
    pthread_mutex_lock(&coder->lock);
    coder->compiling_counter++;
    pthread_mutex_unlock(&coder->lock);
}


void fill_dongles(t_coder *coder, t_dongle **dongle1, t_dongle **dongle2)
{
    if (coder->right < coder->left)
    {
        *dongle1 = coder->right;
        *dongle2 = coder->left;
    }
    else
    {
        *dongle2 = coder->right;
        *dongle1 = coder->left;
    }
}




void *coder_routine(void *argm)
{
    t_simulation *simm;
    t_coder *coder;

    t_dongle *d1;
    t_dongle *d2;

    coder = (t_coder*)argm;
    simm = coder->simulation;


    pthread_mutex_lock(&simm->ready_lock);
    simm->ready_count++;
    pthread_cond_broadcast(&simm->ready_cond);
    pthread_mutex_unlock(&simm->ready_lock);


    if (simm->num_coders == 1)
    {
        take_dongle(coder, coder->left);
        while (get_stop_value(simm) == 0)
        {
            sleep(1);
        }
        return NULL;
    }


    while (get_stop_value(simm) == 0)
    {
        if (are_all_complete_compiles_time(simm) == 1)
        {
            set_stop_value(simm, 1);
            break;
        }

        fill_dongles(coder, &d1, &d2);

        if (take_dongle(coder, d1) == 0)
        {
            
        }
    }


    



    






    
    return NULL;
}