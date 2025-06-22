/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atolli.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:17:27 by asalucci          #+#    #+#             */
/*   Updated: 2025/05/22 11:17:29 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long long int	number_atolli(const char *nb_string)
{
	long long int	p;
	long long int	a;
	int				len;

	len = 0;
	while (nb_string[len] <= '9' && nb_string[len] >= '0')
		len++;
	a = 0;
	p = 1;
	while (len > 0)
	{
		len--;
		a = a + (nb_string[len] - '0') * p;
		p = p * 10;
	}
	return (a);
}

long long int	ft_atolli(const char *nptr)
{
	int			i;
	const char	*nb_string;
	int			sign;

	i = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	if (nptr[i] <= '9' || nptr[i] >= '0')
	{
		nb_string = &nptr[i];
		return (number_atolli(nb_string) * sign);
	}
	return (0);
}
