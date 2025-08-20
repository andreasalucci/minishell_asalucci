/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:04:15 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/21 10:04:18 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * ft_memmove - Copia un bloque de memoria manejando solapamientos.
 *
 * Esta función copia `n` bytes desde el área de memoria `src` al área `dest`.
 * A diferencia de ft_memcpy, ft_memmove asegura que la copia sea correcta
 * incluso si las áreas de memoria se solapan.
 *
 * Parámetros:
 *   - dest: Puntero al área de memoria de destino.
 *   - src: Puntero al área de memoria de origen.
 *   - n: Número de bytes a copiar.
 *
 * Retorno:
 *   - Retorna un puntero al área de memoria de destino (`dest`).
 *   - Si `dest` y `src` son NULL y `n > 0`, retorna NULL.
 *
 * Detalles:
 *   - Si `dest` está antes de `src` en memoria, la copia se realiza 
 * hacia adelante.
 *   - Si `dest` está después de `src` en memoria, la copia se realiza 
 * hacia atrás para evitar sobrescribir datos aún no copiados.
 *   - Esto hace que la función sea segura para bloques de memoria superpuestos.
 */

#include <stddef.h>

static char	*mayor_que(char *dest, const char *src, size_t i)
{
	while (i > 0)
	{
		dest[i -1] = src[i -1];
		i--;
	}
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;
	size_t		i;

	if (!dest && !src && n > 0)
		return (NULL);
	d = (char *)dest;
	s = (const char *)src;
	i = 0;
	if (d < s)
	{
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	else
	{
		i = n;
		d = mayor_que(d, s, i);
	}
	return (dest);
}
