/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <inbauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 09:29:04 by inbauman          #+#    #+#             */
/*   Updated: 2025/02/19 15:25:26 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_printf_control(char copia, va_list args)
{
	int	caracteres;

	caracteres = 0;
	if (copia == 'c')
		caracteres = ft_putchar(va_arg(args, int));
	else if (copia == 's')
		caracteres = ft_printf_putstr(va_arg(args, char *));
	else if (copia == 'p')
		caracteres += ft_printf_puthex(va_arg(args, unsigned long long));
	else if (copia == 'i' || copia == 'd')
		caracteres += ft_printf_putnbr(va_arg(args, int));
	else if (copia == 'u')
		caracteres += ft_printf_putnbr(va_arg(args, unsigned int));
	else if (copia == 'x' || copia == 'X')
		caracteres += ft_printf_numero_hex(va_arg(args, unsigned int), copia);
	else if (copia == '%')
		caracteres += ft_putchar('%');
	else
		caracteres += ft_putchar(copia);
	return (caracteres);
}

int	ft_printf(char const *linea, ...)
{
	va_list		args;
	const char	*copia;
	int			caracteres;

	va_start(args, linea);
	copia = linea;
	caracteres = 0;
	if (!linea)
		return (-1);
	while (*copia)
	{
		if (*copia == '%')
		{
			copia++;
			caracteres += ft_printf_control(*copia, args);
		}
		else
		{
			write(1, copia, 1);
			caracteres++;
		}
		copia++;
	}
	return (caracteres);
}

// int main()
// {
// 	//char nbr = '%';
// 	//char *puntero = nbr;
// 	int caracteres;
// 	int caracteres2;
// 	caracteres = ft_printf(NULL);
// 	caracteres2 = printf(NULL);
// 	//printf("ft_printf ---> %i\nprintf---> %i", caracteres, caracteres2);
// }
