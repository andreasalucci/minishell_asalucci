/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utilities.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:20:51 by asalucci          #+#    #+#             */
/*   Updated: 2025/04/02 13:20:54 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_node	*create_node(int data, int index)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->data = data;
	new_node->index = index;
	new_node->next = NULL;
	return (new_node);
}

void	free_list(t_node *head)
{
	t_node	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

t_node	*array_to_list(int *arr, int size)
{
	int		i;
	t_node	*head;
	t_node	*current;

	i = 1;
	head = create_node(arr[0], 0);
	if (!head)
		return (NULL);
	current = head;
	while (i < size)
	{
		current->next = create_node(arr[i], i);
		if (!current->next)
			return (free_list(head), NULL);
		current = current->next;
		i++;
	}
	return (head);
}

int	*f_bubble_sort(int *arr, int size)
{
	int	i;
	int	j;
	int	swap;

	i = 0;
	swap = 1;
	while (swap && i < size)
	{
		swap = 0;
		j = 1;
		while (j < size)
		{
			if (arr[j] < arr[j - 1])
			{
				swap_int(&arr[j], &arr[j - 1]);
				swap = 1;
			}
			j++;
		}
		i++;
	}
	return (arr);
}

t_node	*f_corresponding_index(t_node *a, t_node *sorted_list)
{
	t_node	*current_il;
	t_node	*current_sl;

	current_il = a;
	while (current_il)
	{
		current_sl = sorted_list;
		while (current_sl)
		{
			if (current_il->data == current_sl->data)
				current_il->index = current_sl->index;
			current_sl = current_sl->next;
		}
		current_il = current_il->next;
	}
	return (a);
}
