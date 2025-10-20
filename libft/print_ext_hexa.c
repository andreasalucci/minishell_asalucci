/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ext_hexa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:04:44 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/15 19:04:45 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	if_negative(unsigned int nbr, size_t *r)
{
	if (nbr < 0)
	{
		write(1, "-", 1);
		(*r)++;
		nbr = -nbr;
	}
	return (nbr);
}

void	recursive(unsigned long nbr, char *hexab, size_t *r)
{
	unsigned long	digit;

	if (nbr > 0)
	{
		digit = nbr % 16;
		recursive(nbr / 16, hexab, r);
		write(1, &hexab[digit], 1);
		(*r)++;
	}
}

void	recursive_neg_min(unsigned long nbr, char *hexab, size_t *r)
{
	unsigned long	digit;

	if (nbr > 0)
	{
		digit = nbr % 16;
		recursive(nbr / 16, hexab, r);
		write(1, &hexab[digit], 1);
		(*r)++;
	}
}
/*

int main()
{
	write(1, "\n\n\n", 3);
	ft_putnbr_base(8, "01"); write(1, "\n", 1);
	ft_putnbr_base(7, "0123"); write(1, "\n", 1);
	ft_putnbr_base(16, "24"); write(1, "\n", 1);
	ft_putnbr_base(16, "2424"); write(1, "\n", 1);
	ft_putnbr_base(-2147483648, "0123456789ABCDEF"); write(1, "\n", 1);
	ft_putnbr_base(2147483647, "0123456789ABCDEF"); write(1, "\n", 1);
	ft_putnbr_base(-42, "0123456789ABCDEF"); write(1, "\n", 1);
	ft_putnbr_base(-42, "0123+56789ABCDEF"); write(1, "\n\n\n", 3);
}*/
