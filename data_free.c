/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:22:55 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 02:04:53 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void cleanup_all_data(t_simulation *simulation)
{
    if (!simulation)
        return;
    pthread_mutex_destroy(&simulation->stop_lock);
    pthread_mutex_destroy(&simulation->log_lock);
    pthread_mutex_destroy(&simulation->arr_lock);
    pthread_mutex_destroy(&simulation->ready_lock);
    pthread_cond_destroy(&simulation->ready_cond);
    int i;

    i = 0;
    while (i < simulation->num_coders)
    {
        pthread_mutex_destroy(&simulation->dongles[i].mutex);
        pthread_cond_destroy(&simulation->dongles[i].cond);
        free_heap(&simulation->dongles[i++].the_heap);
    }
    free(simulation->dongles);
    simulation->dongles = NULL;
    i = 0;
    while (i < simulation->num_coders)
        pthread_mutex_destroy(&simulation->coders[i++].lock);

    free(simulation->coders);
    simulation->coders = NULL;
}
