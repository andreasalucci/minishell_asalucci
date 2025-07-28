/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:43:15 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:45:36 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

static int	signo(const char *nptr, int i)
{
	if (nptr[i] == '-')
		return (-1);
	else
		return (1);
}

static int	calculas_numero(const char *nptr, int i)
{
	int		numero;

	numero = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		numero *= 10;
		numero += nptr[i] - '0';
		i++;
	}
	return (numero);
}

int	ft_atoi(const char *nptr)
{
	int		numero;
	int		negativo;
	int		i;

	i = 0;
	numero = 0;
	negativo = 0;
	if (nptr[i] == '\0')
		return (0);
	while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\n'
		|| nptr[i] == '\r' || nptr[i] == '\v' || nptr[i] == '\f')
		i++;
	if (!(nptr[i] == '-' || nptr[i] == '+' || (nptr[i] >= '0'
				&& nptr[i] <= '9')))
		return (0);
	negativo = signo(nptr, i);
	if (nptr[i] == '-' || nptr[i] == '+')
		i++;
	if (!(nptr[i] >= '0' && nptr[i] <= '9'))
		return (0);
	numero = calculas_numero(nptr, i);
	return (numero * negativo);
}
