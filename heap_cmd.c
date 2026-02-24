/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-achh <hel-achh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 22:29:23 by hel-achh          #+#    #+#             */
/*   Updated: 2026/02/24 02:59:38 by hel-achh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// mybe should use mutex for heap
// resizing

#include "codexion.h"

int create_heap(t_heap *heap, int capa)
{
    if (capa <= 0)
        return 0;
    heap->arr = malloc(sizeof(t_req) * capa);
    if (!heap->arr)
        return 0;
    heap->max_size = capa;
    heap->current_size = 0;
    return 1;
}

static void ft_swap(t_req *r1, t_req *r2)
{
    t_req temp;

    temp = *r1;
    *r1 = *r2;
    *r2 = temp;
}

static int compare_fifo(t_req a, t_req b)
{
    if (a.arrival != b.arrival)
        return a.arrival < b.arrival;
    else
        return a.deadline < b.deadline;
}

static int compare_edf(t_req a, t_req b)
{
    if (a.deadline != b.deadline)
        return a.deadline < b.deadline;
    else
        return a.arrival < b.arrival;
}

static int is_less(t_req a, t_req b, t_simulation *sim)
{
    if (sim->the_type == CODEX_EDF)
        return compare_edf(a, b);
    if (sim->the_type == CODEX_FIFO)
        return compare_fifo(a, b);
    return compare_fifo(a, b);
}

static void up(t_heap *heap, int index, t_simulation *sim)
{
    int parent = 0;

    while (index > 0)
    {
        parent = (index - 1) / 2;

        if (is_less(heap->arr[index], heap->arr[parent], sim))
            ft_swap(&heap->arr[index], &heap->arr[parent]);
        else
            break;
        index = parent;
    }
}

int heap_push(t_heap *heap, t_req req, t_simulation *sim)
{
    if (heap->max_size == heap->current_size)
    {
        printf("the heap is full\n");
        return 0;
    }

    heap->arr[heap->current_size++] = req;
    up(heap, heap->current_size - 1, sim);
    return 1;
}


int get_root(t_heap *heap, t_req *element_to_fill)
{
    if (heap->current_size <= 0)
        return 0;
    *element_to_fill = heap->arr[0];
    return 1;
}



static void down(t_heap *heap, t_simulation *sim, int i)
{
    int min_by_index = i;
    int left = i * 2 + 1;
    int right = i * 2 + 2;

    while (1)
    {
        min_by_index = i;
        left = i * 2 + 1;
        right = i * 2 + 2;

        if (left < heap->current_size && is_less(heap->arr[left], heap->arr[min_by_index], sim))
            min_by_index = left;
        if (right < heap->current_size && is_less(heap->arr[right], heap->arr[min_by_index], sim))
            min_by_index = right;
        if (min_by_index == i)
            break;
        ft_swap(&heap->arr[min_by_index], &heap->arr[i]);
        i = min_by_index;
    }
}



void free_heap(t_heap *heap)
{
    if (heap->arr)
        free(heap->arr);
    heap->arr = NULL;
    heap->current_size = 0;
    heap->max_size = 0;
}


int heap_pop(t_simulation *sim, t_heap *heap, t_req *element_to_fill)
{
    if (heap->current_size <= 0)
        return 0;
    
    *element_to_fill = heap->arr[0];
	heap->current_size--;
    
	if (heap->current_size > 0)
	{
		heap->arr[0] = heap->arr[heap->current_size];
		down(heap, sim, 0);
	}
	return (1);   
}


void	heap_remove_req(t_simulation *sim, t_heap *heap, t_req req)
{
	int		i;

    i = 0;
    while (i < heap->current_size)
    {
        if (heap->arr[i].coder_id == req.coder_id && heap->arr[i].arrival == req.arrival)
        {
            heap->current_size--;
            if (i < heap->current_size)
            {
                heap->arr[i] = heap->arr[heap->current_size];
				down(heap, sim, i);
				up(heap, i, sim);
            }
            return;
        }
        i++;
    }
}
