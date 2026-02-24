/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:56:30 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 21:28:28 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



static void msg_error(void)
{
    char msg[] = "Usage:\n./codexion  CODERS  T_burnout  T_compile  T_debug  T_refactor  N_compiles  cooldown  scheduler_type--> (fifo|edf)\n";
    write(2, &msg, sizeof(msg));
}









int main(int argc, char **argv)
{
    t_simulation simulation;
    int i;
    

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
    
    

    
    return 0;
}




