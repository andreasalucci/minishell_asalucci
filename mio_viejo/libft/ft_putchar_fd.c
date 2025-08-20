/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:43:40 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:43:48 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
/**
 * ft_putchar_fd - Envía el carácter 'c' al file descriptor especificado.
 * @c: El carácter a enviar.
 * @fd: El file descriptor sobre el que escribir.
 *
 * Descripción:
 * Esta función utiliza la función write para escribir el carácter 'c'
 * en el file descriptor 'fd'. Es útil para enviar caracteres a archivos
 * o dispositivos específicos, incluyendo stdout, stderr, o archivos abiertos.
 */

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
