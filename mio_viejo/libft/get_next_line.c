/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <inbauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:19:31 by inbauman          #+#    #+#             */
/*   Updated: 2025/01/16 17:47:56 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line2(char **linea, int bytes_leidos)
{
	char	*linea_retorno;
	char	*temp;
	char	*coincidencia;

	if (bytes_leidos < 0 || (bytes_leidos == 0 && !*linea))
		return (NULL);
	if (bytes_leidos == 0 && **linea == '\0')
	{
		free(*linea);
		*linea = NULL;
		return (NULL);
	}
	coincidencia = ft_strchr(*linea, '\n');
	if (coincidencia)
	{
		linea_retorno = ft_substr(*linea, 0, coincidencia - *linea +1);
		temp = ft_strdup(coincidencia +1);
		free(*linea);
		*linea = temp;
		return (linea_retorno);
	}
	linea_retorno = ft_strdup(*linea);
	free(*linea);
	*linea = NULL;
	return (linea_retorno);
}

char	*get_next_line(int fd)
{
	char		bufer[BUFFER_SIZE +1];
	int			bytes_leidos;
	static char	*linea[MAX_FD];
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bytes_leidos = read(fd, bufer, BUFFER_SIZE);
	while (bytes_leidos > 0)
	{
		if (!linea[fd])
			linea[fd] = ft_strdup("");
		bufer[bytes_leidos] = '\0';
		temp = ft_strjoin(linea[fd], bufer);
		free(linea[fd]);
		linea[fd] = temp;
		if (ft_strchr(temp, '\n'))
			break ;
		bytes_leidos = read(fd, bufer, BUFFER_SIZE);
	}
	return (get_next_line2(&linea[fd], bytes_leidos));
}
/*int main()
{
	int i = 0;
	const char *archivo = "pruebas.txt";
	int fd = open(archivo, O_RDONLY);
	if (fd == -1)
	{
		perror("Error al abrir el archivo");
		return 1;
	}
	char *buffer = get_next_line(fd);
	while (buffer != NULL)
	{
		
		printf("Main %i ----> %s", i +1, buffer);
		i++;
		free(buffer);
		buffer = get_next_line(fd);
		//if (i == 4)
		//{
		//	close(fd);
		//	return 0;
		//}
	}
	close(fd);
	return 0;
}*/
/*int main() {
     int i = 0;
     const char *archivo = "pruebas.txt";
     const char *archivo2 = "pruebas2.txt";
     int fd = open(archivo, O_RDONLY);
     int fd2 = open(archivo2, O_RDONLY);

     if (fd == -1) {
         perror("Error al abrir el archivo pruebas.txt");
         return 1;
     }
     if (fd2 == -1) {
         perror("Error al abrir el archivo pruebas2.txt");
         close(fd); // Cierra el primer archivo si falla el segundo
         return 1;
     }

     char *buffer1 = NULL;
     char *buffer2 = NULL;
    
     while (1) {
         buffer1 = get_next_line(fd);
         buffer2 = get_next_line(fd2);
       
         if (buffer1 == NULL && buffer2 == NULL)
             break; // Ambos archivos han sido leídos completamente
        
         if (buffer1) {
             printf("Texto 1 %i ----> %s", i + 1, buffer1);
             free(buffer1);
         }
        
         if (buffer2) {
             printf("texto 2 %i ----> %s", i + 1, buffer2);
             free(buffer2);
         }
        
         i++;
     }

     close(fd);
     close(fd2);

     return 0;
}*/
