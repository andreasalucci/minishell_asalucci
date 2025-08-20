/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:39:31 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:39:41 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
/**
 * ft_striteri - Aplica una función a cada carácter de una cadena
 * @s: La cadena que se va a iterar
 * @f: La función a aplicar a cada carácter de la cadena
 * 
 * Descripción:
 * Esta función itera sobre cada carácter de la cadena 's'. A cada carácter,
 * se le aplica la función 'f', la cual toma dos parámetros: el índice del
 * carácter dentro de la cadena y un puntero al propio carácter, permitiendo
 * así modificarlo si es necesario.
 * 
 * Parámetros:
 * - s: La cadena sobre la que se va a iterar. No debe ser NULL.
 * - f: La función que se va a aplicar a cada carácter. No debe ser NULL.
 * 
 * Retorno:
 * No retorna ningún valor.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*#include <stdio.h>
#include <string.h>
#include "libft.h"

void test_function(unsigned int i, char *c)
{
    *c = *c + i;
}

void run_test2(char *input, char *expected)
{
    char buffer[100];
    ft_strlcpy(buffer, input, sizeof(input));
    ft_striteri(buffer, test_function);
    if (ft_strcmp(buffer, expected) == 0)
        printf("Test passed\n");
    else
        printf("Test failed: expected \"%s\", got \"%s\"\n", expected, buffer);
}

int main()
{
    run_test2("abc", "ace");
    run_test2("123", "135");
    run_test2("", "");
    run_test2("A", "A");

    return 0;
}*/
//gcc -Wall -Wextra -Werror ft_striteri.c ft_strlcpy.c ft_strcmp.c
