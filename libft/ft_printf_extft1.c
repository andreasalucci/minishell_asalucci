/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_extft1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:03:58 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/15 19:04:00 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_char_fu(char a, size_t i, size_t *r)
{
	a = (char)a;
	write(1, &a, 1);
	(*r)++;
	return (i + 2);
}

size_t	print_str_fu(char *a, size_t i, size_t *r)
{
	int	index;

	if (!a)
	{
		write(1, "(null)", 6);
		*r += 6;
		return (i + 2);
	}
	index = 0;
	while (a[index] != '\0')
	{
		write(1, &a[index], 1);
		(*r)++;
		index++;
	}
	return (i + 2);
}

size_t	print_int_fu(int a, size_t i, size_t *r)
{
	char	*astr;
	int		index;

	astr = ft_itoa(a);
	index = 0;
	while (astr[index] != '\0')
	{
		write(1, &astr[index], 1);
		(*r)++;
		index++;
	}
	free(astr);
	return (i + 2);
}

size_t	print_dec_fu(long a, size_t i, size_t *r)
{
	i = print_int_fu(a, i, r);
	return (i);
}
