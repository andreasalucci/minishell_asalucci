/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:14:16 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/27 09:14:18 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*res_s;

	res_s = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!res_s)
		return (NULL);
	res_s[ft_strlen(s)] = '\0';
	i = 0;
	while (s[i] != '\0')
	{
		res_s[i] = f(i, s[i]);
		i++;
	}
	return (res_s);
}
