/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:17:28 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/25 18:07:44 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*temp;
	size_t	start;
	size_t	stop;

	start = 0;
	stop = ft_strlen(s1);
	if (!(s1 && set))
		return (NULL);
	while (ft_strchr(set, s1[start]) && s1[start] != '\0')
		start++;
	while (ft_strchr(set, s1[stop - 1]) && stop > start)
		stop--;
	temp = ft_substr(s1, start, stop - start);
	return (temp);
}
