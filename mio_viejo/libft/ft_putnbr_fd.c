/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:48:59 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 18:17:20 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
 * ft_putnbr_fd - Envía el número 'n' al file descriptor especificado. 
 * @n: El número que se va a enviar. 
 * @fd: El file descriptor sobre el que escribir. * 
 * 
 * Descripción: 
 * Esta función envía el número entero 'n' al file descriptor 'fd' usando la 
 * función write. Es capaz de manejar números negativos y el valor mínimo de 
 * un entero (-2147483648). Utiliza una función auxiliar para manejar los 
 * dígitos del número. 
 */
static void	numeros_dos_cifras(int n, int fd)
{
	char	nbr[11];
	int		i;

	i = 0;
	while (n != 0)
	{
		nbr[i] = (n % 10) + '0';
		n = n / 10;
		i++;
	}
	while (--i >= 0)
	{
		write(fd, &nbr[i], 1);
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	char	nbr;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 0 && n < 10)
	{
		nbr = n + '0';
		write(fd, &nbr, 1);
	}
	if (n >= 10)
		numeros_dos_cifras(n, fd);
}
