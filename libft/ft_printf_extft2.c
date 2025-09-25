/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_extft2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:04:07 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/15 19:04:16 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	print_hexa_fu(unsigned int nbr, size_t i, size_t *r, char *hexab)
{
	if (nbr == 0)
	{
		write(1, "0", 1);
		(*r)++;
	}
	nbr = if_negative(nbr, r);
	recursive(nbr, hexab, r);
	return (i + 2);
}

size_t	print_hexa_p_fu(unsigned long nbr, size_t i, size_t *r, char *hexab)
{
	if (nbr == 0)
	{
		write(1, "0", 1);
		(*r)++;
	}
	recursive(nbr, hexab, r);
	return (i + 2);
}

size_t	print_pointer_fu(void *a, size_t i, size_t *r)
{
	unsigned long	nbr;

	if (!a)
	{
		write(1, "(nil)", 5);
		*r += 5;
		return (i + 2);
	}
	write(1, "0x", 2);
	*r += 2;
	nbr = (unsigned long)(a);
	i = print_hexa_p_fu(nbr, i, r, "0123456789abcdef");
	return (i);
}

size_t	print_unsdec_fu(unsigned int a, size_t i, size_t *r)
{
	char			*astr;
	int				index;
	unsigned int	ua;

	ua = (unsigned int)a;
	astr = dtoa_fu(ua);
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

size_t	print_percent_fu(size_t i, size_t *r)
{
	write(1, "%", 1);
	(*r)++;
	return (i + 2);
}
/*
int main()
{
	print_hexalow_fu(25, 0, 0);
	return 0;
}
//cc ft_printf_extft2.c ft_printf_extft1.c print_ext_hexa.c
*/
