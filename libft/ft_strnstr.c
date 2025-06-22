/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:27:31 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/22 19:35:36 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*cbig;
	char	*clittle;

	cbig = (char *)big;
	clittle = (char *)little;
	i = 0;
	j = 0;
	if (clittle[0] == '\0')
		return (cbig);
	while (cbig[i] != '\0' && i < len)
	{
		while (cbig[i + j] == clittle[j] && clittle[j] != '\0')
		{
			if (i + j >= len)
				return (NULL);
			j++;
		}
		if (clittle[j] == '\0')
			return (&(cbig[i]));
		i++;
		j = 0;
	}
	return (NULL);
}
