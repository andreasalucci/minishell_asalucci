/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:46:17 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:46:21 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"
/*
Tu función ft_strmapi aplica una función f a cada carácter de 
una cadena s, creando una nueva cadena con los resultados.

Proceso:

Verificación de Entrada: Comprueba que la cadena s no sea NULL. 
Si s es NULL, la función retorna NULL.

Calcular la Longitud de s: Determina la longitud de la cadena s 
usando ft_strlen.

Reserva de Memoria: Usa malloc para reservar memoria suficiente 
para la nueva cadena, incluyendo espacio para el carácter nulo (\0).

Aplicar f a Cada Carácter: Recorre cada carácter de la cadena s 
con un bucle for, aplicando la función f y almacenando el 
resultado en la nueva cadena.

Añadir el Carácter Nulo: Asegúrate de que la nueva cadena termina 
con un carácter nulo (\0).

Retornar la Nueva Cadena: Retorna la nueva cadena que contiene 
los resultados de aplicar f a cada carácter de s.
*/
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	size_t			largo_str;
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return (NULL);
	largo_str = ft_strlen(s);
	str = (char *)malloc((sizeof(char) * largo_str) + 1);
	if (!str)
		return (NULL);
	while (i < largo_str)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[largo_str] = '\0';
	return (str);
}
