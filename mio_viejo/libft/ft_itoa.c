/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:32:44 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 18:12:03 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
/* FT_ITOA convertira un numero INT en una cadena de CHAR es el contrario de
 * FT_ATOI*/
static int	contar_numeros(int n);

static char	*numero_max(void)
{
	char	*str;

	str = malloc(sizeof(char) * 12);
	if (!str)
		return (NULL);
	str[0] = '-';
	str[1] = '2';
	str[2] = '1';
	str[3] = '4';
	str[4] = '7';
	str[5] = '4';
	str[6] = '8';
	str[7] = '3';
	str[8] = '6';
	str[9] = '4';
	str[10] = '8';
	str[11] = '\0';
	return (str);
}

static char	*copiar_numeros(int n, int signo)
{
	char	*str;
	int		j;
	int		cifras;

	cifras = contar_numeros(n) + signo;
	j = cifras -1;
	str = malloc((sizeof(char) * cifras) + 1);
	if (!str)
		return (NULL);
	str[cifras] = '\0';
	if (signo == 1)
	{
		str[0] = '-';
		n = -n;
	}
	while (n != 0 && j >= signo)
	{
		str[j] = (n % 10) + '0';
		n /= 10;
		j--;
	}
	return (str);
}

static char	*una_cifra(int n, int signo)
{
	char	*str;

	if (signo == 0)
	{
		str = malloc(sizeof(char) * 2);
		if (!str)
			return (NULL);
		str[0] = n + '0';
		str[1] = '\0';
	}
	else
	{
		str = malloc(sizeof(char) * 3);
		if (!str)
			return (NULL);
		str[0] = '-';
		str[1] = -n + '0';
		str[2] = '\0';
	}
	return (str);
}

static int	contar_numeros(int n)
{
	int	numeros;

	numeros = 0;
	while (n != 0)
	{
		n /= 10;
		numeros++;
	}
	return (numeros);
}

char	*ft_itoa(int n)
{
	int		signo;
	char	*str;

	signo = 0;
	if (n < 0)
		signo = 1;
	if (n == -2147483648)
		return (numero_max());
	if ((n >= 0 && n < 10) || (n < 0 && n > -10))
	{
		str = una_cifra(n, signo);
		return (str);
	}
	else
	{
		str = copiar_numeros(n, signo);
		return (str);
	}
}
