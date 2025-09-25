/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:17:28 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/25 15:26:59 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*js;
	size_t	i;
	size_t	j;

	js = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!js)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		js[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		js[i + j] = s2[j];
		j++;
	}
	js[i + j] = '\0';
	return (js);
}
