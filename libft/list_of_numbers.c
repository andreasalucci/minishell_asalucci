/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:19:55 by asalucci          #+#    #+#             */
/*   Updated: 2025/04/02 13:19:57 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_list_ordered(t_node *a)
{
	while (a && a->next)
	{
		if (a->index != (a->next)->index - 1)
			return (0);
		a = a->next;
	}
	return (1);
}

int	f_is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	return (str[i] == '\0');
}

int	check_duplicates(int *arr, int size)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (j < size)
		{
			if (arr[i] == arr[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	process_args(int argc, char **argv, int **initial_arr,
	long long int **ini_arr_l)
{
	int	i;

	*ini_arr_l = (long long int *)malloc(sizeof(long long int) * (argc - 1));
	if (!*ini_arr_l)
		return (0);
	*initial_arr = (int *)malloc(sizeof(int) * (argc - 1));
	if (!*initial_arr)
	{
		free(ini_arr_l);
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!f_is_number(argv[i]))
			return (0);
		(*ini_arr_l)[i - 1] = ft_atolli(argv[i]);
		if ((*ini_arr_l)[i - 1] > 2147483647 ||
			(*ini_arr_l)[i - 1] < -2147483648)
			return (0);
		(*initial_arr)[i - 1] = (int)((*ini_arr_l)[i - 1]);
		i++;
	}
	return (1);
}
