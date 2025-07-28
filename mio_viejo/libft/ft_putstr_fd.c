/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:46:48 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:46:54 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/**
 * ft_putstr_fd - Envía la cadena 's' al file descriptor especificado.
 * @s: La cadena que se va a enviar.
 * @fd: El file descriptor sobre el que escribir.
 *
 * Descripción:
 * Esta función utiliza la función write para escribir la cadena 's' en el
 * file descriptor 'fd'. Es útil para enviar cadenas a archivos o dispositivos
 * específicos, incluyendo stdout, stderr, o archivos abiertos.
 *
 * Parámetros:
 * - s: La cadena que se va a enviar. No debe ser NULL.
 * - fd: El file descriptor sobre el que se va a escribir. Puede ser la salida
 *   estándar (1), la salida de error estándar (2), o un file descriptor de 
 * un archivo.
 *
 * Valor devuelto:
 * No retorna ningún valor.
 */
void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL || fd < 0)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}
