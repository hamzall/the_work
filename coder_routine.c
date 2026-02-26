/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 02:51:11 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/26 14:03:47 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"




    
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