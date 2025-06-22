/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:13:19 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/15 19:03:36 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*numberfill_fu(int n, char *number, int len)
{
	char	digit;
	int		s;
	int		i;

	i = len - 1;
	s = 0;
	if (n == 0)
		number[0] = '0';
	if (n < 0)
	{
		s = 1;
		number[0] = '-';
	}
	while (i >= s)
	{
		if (n >= 0)
			digit = '0' + (n % 10);
		else
			digit = '0' - (n % 10);
		number[i] = digit;
		i--;
		n /= 10;
	}
	return (number);
}

char	*ft_itoa(int n)
{
	char	*number;
	int		i;
	int		len;

	i = 1;
	len = 1;
	while ((n / i) >= 10 || (n / i) <= -10)
	{
		i *= 10;
		len++;
	}
	if (n < 0)
		len++;
	number = (char *)malloc(sizeof(char) * (len + 1));
	if (!number)
		return (NULL);
	number = numberfill_fu(n, number, len);
	number[len] = '\0';
	return (number);
}
