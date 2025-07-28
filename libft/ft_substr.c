/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:56:02 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/05 15:56:05 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*ss;

	if (start >= ft_strlen(s) || len == 0)
		ss = (char *)malloc(sizeof(char));
	else if (start + len >= ft_strlen(s))
		ss = (char *)malloc(sizeof(char) * (ft_strlen(s) - start) + 1);
	else
		ss = (char *)malloc(sizeof(char) * (len + 1));
	if (!ss)
		return (NULL);
	if (start >= ft_strlen(s) || len == 0)
	{
		ss[0] = '\0';
		return (ss);
	}
	i = 0;
	while (s[start] && s[start + i] && i < len)
	{
		ss[i] = s[start + i];
		i++;
	}
	ss[i] = '\0';
	return (ss);
}
