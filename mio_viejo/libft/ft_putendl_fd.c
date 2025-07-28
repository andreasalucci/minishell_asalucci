/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:48:16 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:48:23 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/**
 * ft_putendl_fd - Envía la cadena 's' al file descriptor dado, 
 * seguido de un salto de línea.
 * @s: La cadena que se va a enviar.
 * @fd: El file descriptor sobre el que escribir.
 *
 * Descripción:
 * Esta función utiliza la función write para escribir la cadena 's' en el
 * file descriptor 'fd', seguida de un salto de línea ('\n'). Es útil para
 * enviar cadenas con una nueva línea al final a archivos o dispositivos
 * específicos, incluyendo stdout, stderr, o archivos abiertos.
 *
 * Parámetros:
 * - s: La cadena que se va a enviar. No debe ser NULL.
 * - fd: El file descriptor sobre el que se va a escribir. Puede ser la salida
 *   estándar (1), la salida de error estándar (2), o un file descriptor 
 * de un archivo.
 *
 * Valor devuelto:
 * No retorna ningún valor.
 */
void	ft_putendl_fd(char *s, int fd)
{
	if (s == NULL || fd < 0)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
	write(fd, "\n", 1);
}
