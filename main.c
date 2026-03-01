/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:56:30 by hel-achh          #+#    #+#             */
/*   Updated: 2026/03/01 14:27:52 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



static void msg_error(void)
{
    char msg[] = "Usage:\n./codexion  CODERS  T_burnout\
  T_compile  T_debug  T_refactor  N_compiles  cooldown  scheduler_type--> : (fifo OR edf)\n";
    write(2, &msg, sizeof(msg));
}









int main(int argc, char **argv)
{
    t_simulation simulation;
    int i;
    int j;
    

    if (parsing(&simulation, argc, argv) == 0)
    {
        msg_error();
        return 1;
    }

    if (initialize_data(&simulation) == 0)
    {
        printf("error in init\n");
        return 1;
    }

    simulation.start_ms = get_current_time();

    i = 0;
    while (i < simulation.num_coders)
    {
        simulation.coders[i].last_compile = simulation.start_ms;
        i++;
    }


    i = 0;
    while (i < simulation.num_coders)
    {
        if (pthread_create(&simulation.coders[i].thread, NULL, coder_routine, &simulation.coders[i]) != 0)
        {
            set_stop_value(&simulation, 1);
            wake_up_all_threads(&simulation);
            
            j = 0;
            while (j < i)
                pthread_join(simulation.coders[j++].thread, NULL);

            cleanup_all_data(&simulation);
            return 1;
        }
        i++;
    }

    // all this wait for the fairness and the best timing for a void race or....
    pthread_mutex_lock(&simulation.ready_lock);
    while (simulation.ready_count < simulation.num_coders)
        pthread_cond_wait(&simulation.ready_cond, &simulation.ready_lock);
    pthread_mutex_unlock(&simulation.ready_lock);


    if (pthread_create(&simulation.monitor, NULL, monitor_routine, &simulation) != 0)
    {
        set_stop_value(&simulation, 1);
        wake_up_all_threads(&simulation);
        
        i = 0;
        while (i < simulation.num_coders)
            pthread_join(simulation.coders[i++].thread, NULL);

        cleanup_all_data(&simulation);
        return 1;
    }

    i = 0;
    while (i < simulation.num_coders)
        pthread_join(simulation.coders[i++].thread, NULL);

    pthread_join(simulation.monitor, NULL);


    cleanup_all_data(&simulation);
    return 0;
}
