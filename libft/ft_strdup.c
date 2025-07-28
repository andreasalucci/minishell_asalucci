/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:11:20 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/23 14:43:25 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*ds;
	char	*cs;

	cs = (char *)s;
	i = 0;
	while (cs[i] != '\0')
		i++;
	ds = (char *)malloc((sizeof (char)) * (i + 1));
	if (!ds)
		return (NULL);
	i = 0;
	while (cs[i] != '\0')
	{
		ds[i] = cs[i];
		i++;
	}
	ds[i] = '\0';
	return ((char *)ds);
}
/*
int main()
{
	char str[] = "abelsoft";
	char *btr;

	btr = ft_strdup(str);
	free(btr);
	return 0;
}*/
