/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 02:51:11 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/25 22:34:37 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// // is all threads ready to start


// 	pthread_mutex_t	ready_lock;
// 	pthread_cond_t	ready_cond;
// 	int				ready_count;


    
void *coder_routine(void *argm)
{
    t_simulation *simm;
    t_coder *coder;

    coder = (t_coder*)argm;
    simm = coder->simulation;


    
    pthread_mutex_lock(&simm->ready_lock);
    simm->ready_count++;
    pthread_cond_broadcast(&simm->ready_cond);
    pthread_mutex_unlock(&simm->ready_lock);


    


    



    






    
    return NULL;
}