/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displays.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 03:13:00 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 03:13:52 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// void log_status(t_sim *sim, int coder_id, t_status st)
// {
//     long t;
//     int stopped;

//     pthread_mutex_lock(&sim->stop_lock);
//     stopped = sim->stop;
//     pthread_mutex_unlock(&sim->stop_lock);

//     if (stopped && st != ST_BURNED_OUT)
//         return;

//     pthread_mutex_lock(&sim->log_lock);
//     t = now_ms() - sim->start_ms;
//     printf("%ld %d %s\n", t, coder_id, msg_of(st));
//     pthread_mutex_unlock(&sim->log_lock);
// }



