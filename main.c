/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:56:30 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 02:52:30 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



static void msg_error(void)
{
    char msg[] = "Usage:\n./codexion n_of_coders t_to_burnout t_to_compile t_to_debug t_to_refactor n_of_compiles_required dongle_cooldown scheduler(fifo|edf)\n";
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

    
    

    
    return 0;
}




