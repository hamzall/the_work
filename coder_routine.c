/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 02:51:11 by hel-achh          #+#    #+#             */
/*   Updated: 2026/03/01 21:23:44 by hel-achh         ###   ########.fr       */
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



	// int				id;
	// pthread_t		thread;

	// pthread_mutex_t	lock;
	// long			last_compile;
	// int				compiling_counter;

	// t_dongle		*left;
	// t_dongle		*right;

	// struct s_simulation	*simulation;


    
// 	pthread_mutex_t	ready_lock;
// 	pthread_cond_t	ready_cond;
// 	int				ready_count;



    
void *coder_routine(void *argm)
{
    t_dongle *d1, d2;
    t_simulation *sim;
    t_coder *coder;

    coder = (t_coder*)argm;
    sim = coder->simulation;

    pthread_mutex_lock(&sim->arr_lock);
    sim->arr_counter++;
    pthread_cond_broadcast(&sim->ready_cond);
    pthread_mutex_unlock(&sim->arr_lock);

    
    if (sim->num_coders == 1)
    {
        if (take_dongle(coder, coder->left) == 0)
        {
            sleep(2);
            return NULL;
        }
        else{
            sleep(2);
            return NULL;
        }
    }
    
    while (get_stop_value(sim) == 0)
    {
        if (are_all_complete_compiles_time(sim))
        {
            set_stop_value(sim, 1);
            break;
        }
        
        fill_dongles(coder, &d1, &d2);
        
        if (!take_dongle(coder, d1))
        {
            if (get_stop_value(sim))
            {
                
            }
        }

        
    }
    

    
    
    
    
    return NULL;
}
