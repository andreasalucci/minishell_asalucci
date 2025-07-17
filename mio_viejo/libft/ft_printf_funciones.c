/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_funciones.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <inbauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 09:29:33 by inbauman          #+#    #+#             */
/*   Updated: 2025/02/19 15:25:44 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*int imprimir_decimales(int decimales)
{
    int i;
    char    *nbr;

    i = 0;
    if (decimales < 0)
        decimales = -decimales;
    while(decimales)
    {
        nbr[i] = (decimales % 10) + '0';
        decimales /= 10;
        i++;
    }
    i = 1;
    while(i >= 0)
    {
        write(1, &nbr[i], 1);
        i--;
    }
    return (2);
}*/
/*int control_decimal(double num)
{
    int entero;
    int caracteres;
    int decimales;
    double numero_control;
    
    caracteres = 0;
    entero = num;
    numero_control = num - entero;
    if(numero_control != 0.0)
        {
            num *= 100;
            decimales = num;
            caracteres += ft_putnbr(entero);
            write(1, ".", 1);
            caracteres += imprimir_decimales(decimales);
            return (caracteres);
        }
    
    caracteres += ft_putnbr(entero);
    return(caracteres);
}*/
int	ft_printf_numeros_dos_cifras(long long num)
{
	char	nbr[11];
	int		i;
	int		caracteres;

	caracteres = 0;
	i = 0;
	while (num != 0)
	{
		nbr[i] = (num % 10) + '0';
		num = num / 10;
		i++;
	}
	nbr[i] = '\0';
	caracteres = i;
	while (--i >= 0)
	{
		write(1, &nbr[i], 1);
	}
	return (caracteres);
}

int	ft_printf_putnbr(long long num)
{
	char	nbr;
	int		caracteres;

	caracteres = 0;
	if (num == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (num < 0)
	{
		write(1, "-", 1);
		num = -num;
		caracteres++;
	}
	if (num >= 0 && num < 10)
	{
		nbr = num + '0';
		write(1, &nbr, 1);
		caracteres++;
		return (caracteres);
	}
	if (num >= 10)
		caracteres += ft_printf_numeros_dos_cifras(num);
	return (caracteres);
}

int	ft_printf_numero_hex(unsigned int num, char c)
{
	char	*base_hex;
	int		caracteres;

	caracteres = 0;
	if (num == 0)
		return (write(1, "0", 1));
	if (c == 'x')
		base_hex = "0123456789abcdef";
	else
		base_hex = "0123456789ABCDEF";
	if (num >= 16)
		caracteres += ft_printf_numero_hex(num / 16, c);
	num %= 16;
	caracteres += write(1, &base_hex[num], 1);
	return (caracteres);
}

int	ft_printf_putstr(char *s)
{
	int	caracteres;

	caracteres = 0;
	if (s == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (*s)
	{
		write(1, s, 1);
		caracteres++;
		s++;
	}
	return (caracteres);
}

int	ft_printf_puthex(unsigned long long p)
{
	char	*base_hex;
	int		caracteres;

	base_hex = "0123456789abcdef";
	caracteres = 0;
	if (p == 0)
		return (write(1, "(nil)", 5));
	if (p >= 16)
		caracteres += ft_printf_puthex(p / 16);
	if (caracteres == 0)
		caracteres += write(1, "0x", 2);
	p %= 16;
	caracteres += write(1, &base_hex[p], 1);
	return (caracteres);
}
