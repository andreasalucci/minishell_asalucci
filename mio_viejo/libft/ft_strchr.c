/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:04:01 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/21 16:04:04 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Función ft_strchr: Esta función busca la primera ocurrencia del carácter c 
en la cadena s. Si el carácter es encontrado, retorna un puntero a esa posición
en la cadena. Si no se encuentra, retorna NULL.*/

#include <stdlib.h>

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	while (*s != '\0')
	{
		if (*s == (char) uc)
			return ((char *)s);
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return (NULL);
}
