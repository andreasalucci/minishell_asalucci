/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:23:53 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:42:52 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
la función strncmp según el manual de C (man). La función estándar ft_strncmp 
compara los primeros n caracteres de dos cadenas y tiene en cuenta un parámetro 
adicional para especificar el número de caracteres a comparar.
*/
#include <stddef.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	const unsigned char	*us1;
	const unsigned char	*us2;

	us1 = (const unsigned char *)s1;
	us2 = (const unsigned char *)s2;
	i = 0;
	while (i < n && us1[i] != '\0' && us2[i] != '\0')
	{
		if (us1[i] != us2[i])
			return (us1[i] - us2[i]);
		i++;
	}
	if (i < n)
	{
		if (us2[i] != '\0')
			return (-us2[i]);
		if (us1[i] != '\0')
			return (us1[i]);
	}
	return (0);
}
/*int main()
{
	printf("%i", ft_strncmp("test\200", "test\0", 6));
	printf("\n%i\n", strncmp("test\200", "test\0", 6));
}*/
