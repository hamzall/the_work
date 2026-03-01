/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 22:58:30 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/27 14:38:52 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <limits.h>

typedef enum e_type
{
    CODEX_FIFO = 0,
    CODEX_EDF = 1
} t_type;

typedef enum e_status
{
	TAKE_DONGLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}	t_status;

typedef struct s_req
{
	int		coder_id;
	long	arrival;
	long	deadline;
}	t_req;

typedef struct s_heap
{
	t_req	*arr;
	int		current_size;
	int		max_size;
}	t_heap;

struct s_simulation;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;

	int				held_by;
	long			cooldown_to;

	t_heap			the_heap;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;

	pthread_mutex_t	lock;
	long			last_compile;
	int				compiling_counter;

	t_dongle		*left;
	t_dongle		*right;

	struct s_simulation	*simulation;
}	t_coder;

typedef struct s_simulation
{
	int				num_coders;
	long			time_burnout;
	long			time_compile;
	long			time_debug;
	long			time_refactor;
	int				total_compiles;
	long			cooldown;
	t_type			the_type;

	long			start_ms;

	pthread_mutex_t	stop_lock;
	int				is_stoped;

	pthread_mutex_t	log_lock;

	pthread_mutex_t	arr_lock;
	long			arr_counter;

	t_dongle		*dongles;
	t_coder			*coders;

	pthread_t		monitor;

	// is all threads ready to start
	pthread_mutex_t	ready_lock;
	pthread_cond_t	ready_cond;
	int				ready_count;

}	t_simulation;




long get_current_time(void);
int parsing(t_simulation *simulation, int argc, char **argv);
int create_heap(t_heap *heap, int capa);
void free_heap(t_heap *heap);
void	sleep_by_ms(long ms);
int	initialize_data(t_simulation *s);
void set_stop_value(t_simulation *sim, int value);
void wake_up_all_threads(t_simulation *sim);
void cleanup_all_data(t_simulation *simulation);
void *coder_routine(void *argm);
int get_root(t_heap *heap, t_req *element_to_fill);
int heap_push(t_heap *heap, t_req req, t_simulation *sim);
int get_stop_value(t_simulation *sim);
int heap_pop(t_simulation *sim, t_heap *heap, t_req *element_to_fill);
void log_status(t_simulation *sim, int coder_id, t_status st);
void	heap_remove_req(t_simulation *sim, t_heap *heap, t_req req);
int take_dongle(t_coder *coder, t_dongle *dongle);



#endif
