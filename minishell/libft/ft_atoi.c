/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:35:06 by asalucci          #+#    #+#             */
/*   Updated: 2024/05/29 17:35:10 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long int	number(const char *nb_string)
{
	int	p;
	int	a;
	int	len;

	len = 0;
	while (nb_string[len] <= '9' && nb_string[len] >= '0')
		len++;
	a = 0;
	p = 1;
	while (len > 0)
	{
		len--;
		a = a + (nb_string[len] - '0') * p;
		p = p * 10;
	}
	return (a);
}

int	ft_atoi(const char *nptr)
{
	int			i;
	const char	*nb_string;
	int			sign;

	i = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	if (nptr[i] <= '9' || nptr[i] >= '0')
	{
		nb_string = &nptr[i];
		return (number(nb_string) * sign);
	}
	return (0);
}
