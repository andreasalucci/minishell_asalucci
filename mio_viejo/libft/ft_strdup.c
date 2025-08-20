/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 12:14:21 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/07 12:14:26 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
 * La función ft_strdup recibe una cadena de caracteres 's' y devuelve una 
 * nueva cadena que es una copia exacta de 's'. La memoria para la nueva
 * cadena se reserva dinámicamente con malloc. Si la reserva de memoria falla,
 * se retorna NULL. Se utiliza ft_strlcpy para copiar la cadena de manera 
 * segura, incluyendo el carácter de terminación '\0'.
 */

#include <stddef.h>
#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*dest;

	i = ft_strlen(s);
	dest = malloc(i +1);
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, s, i +1);
	return (dest);
}
