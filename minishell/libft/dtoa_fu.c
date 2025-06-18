/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dtoa_fu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:13:19 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/15 19:03:26 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*unumberfill_fu(unsigned int n, char *number, int len)
{
	char	digit;
	int		s;
	int		i;

	i = len - 1;
	s = 0;
	if (n == 0)
		number[0] = '0';
	while (i >= s)
	{
		digit = '0' + (n % 10);
		number[i] = digit;
		i--;
		n /= 10;
	}
	return (number);
}

char	*dtoa_fu(unsigned int n)
{
	char	*number;
	int		i;
	int		len;

	i = 1;
	len = 1;
	while ((n / i) >= 10)
	{
		i *= 10;
		len++;
	}
	number = (char *)malloc(sizeof(char) * (len + 1));
	if (!number)
		return (NULL);
	number = unumberfill_fu(n, number, len);
	number[len] = '\0';
	return (number);
}
