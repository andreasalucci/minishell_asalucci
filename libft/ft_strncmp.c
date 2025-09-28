/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:27:31 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/21 11:35:05 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				i;
	unsigned char	o;

	o = 0;
	if ((int)n < 0)
	{
		if (ft_strlen(s1) >= ft_strlen(s2))
			n = ft_strlen(s1);
		else
			n = ft_strlen(s2);
	}
	i = 0;
	while (i < (int)n)
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (o);
		if (s1[i] > s2[i])
			return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
		else if (s1[i] < s2[i])
			return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
		i++;
	}
	return (o);
}
