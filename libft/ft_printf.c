/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:23:03 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/15 19:03:47 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	normal_print_fu(const char *str, size_t i, size_t *r)
{
	write(1, &(str[i]), 1);
	(*r)++;
	i++;
	return (i);
}

size_t	third_ft_printf(const char *str, size_t i, va_list args, size_t *r)
{
	if (str[i + 1] == 'X')
	{
		i = print_hexa_fu(va_arg(args, unsigned long long),
				i, r, "0123456789ABCDEF");
	}
	else if (str[i + 1] == '%')
		i = print_percent_fu(i, r);
	else
	{
		write(1, &(str[i]), 1);
		(*r)++;
		i++;
		return (i);
	}
	return (i);
}

size_t	second_ft_printf(const char *str, size_t i, va_list args, size_t *r)
{
	if (str[i + 1] == '\0')
	{
		i = normal_print_fu(str, i, r);
		return (*r);
	}
	if (str[i + 1] == 'c')
		i = print_char_fu(va_arg(args, int), i, r);
	else if (str[i + 1] == 's')
		i = print_str_fu(va_arg(args, char *), i, r);
	else if (str[i + 1] == 'p')
		i = print_pointer_fu(va_arg(args, void *), i, r);
	else if (str[i + 1] == 'd')
		i = print_dec_fu(va_arg(args, long), i, r);
	else if (str[i + 1] == 'i')
		i = print_int_fu(va_arg(args, int), i, r);
	else if (str[i + 1] == 'u')
		i = print_unsdec_fu(va_arg(args, unsigned int), i, r);
	else if (str[i + 1] == 'x')
	{
		i = print_hexa_fu(va_arg(args, unsigned int), i, r,
				"0123456789abcdef");
	}
	else
		i = third_ft_printf(str, i, args, r);
	return (i);
}

size_t	fourth_ft_printf(const char *str, va_list args, size_t *r)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '%' && str[i] != '\0')
			i = normal_print_fu(str, i, r);
		if (str[i] == '\0')
			return (*r);
		if (str[i] == '%' && str[i + 1] == '\0')
			return (-1);
		i = second_ft_printf(str, i, args, r);
	}
	return (*r);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	size_t	*r;
	size_t	rr;

	r = calloc(1, sizeof(size_t));
	va_start(args, str);
	if (str[0] == '\0')
	{
		free(r);
		return (0);
	}
	rr = fourth_ft_printf(str, args, r);
	free(r);
	va_end(args);
	return (rr);
}
/*


int main() {

int p;
int f;

f = ft_printf("F %p %p \n", 
-9223372036854775809ULL, 9223372036854775807);
ft_printf("F %i\n", f);

p = printf("P %p %p \n", 
-9223372036854775809ULL, 9223372036854775807);
printf("P %i\n", p);

//free(p);
//free(f);

printf("\n\n\n\n\n\n\n\n");


//cd 0_HolyGraph/CC_0perativa/ft_printf
//cc ft_printf.c libftprintf.a
//cc ft_printf.c ft_printf_extft1.c ft_printf_extft2.c
 ft_itoa.c dtoa_fu.c print_ext_hexa.c






printf("P%i\n", printf(""));
ft_printf("F%i\n", ft_printf(""));		printf("\n\n");




//	printf("P%i\n", printf(1)); // deve segmentare
//	ft_printf("F%i\n", ft_printf(1));		printf("\n\n");




printf("P%i\n", printf("Pbcdef\n"));
ft_printf("F%i\n", ft_printf("Fbcdef\n"));		printf("\n\n");




// printf("P%i\n", printf("Pbcdef\n", 's', 't'));
// ft_printf("F%i\n", ft_printf("Fbcdef\n", 's', 't'));	printf("\n\n");




printf("P%i\n", printf("Pbc%cde%cf\n", 's', 't'));
ft_printf("F%i\n", ft_printf("Fbc%cde%cf\n", 's', 't'));	printf("\n\n");




// printf("P%i\n", printf("Pbc%cde%cf\n", 's'));
// ft_printf("F%i\n", ft_printf("Fbc%cde%cf\n", 's'));		printf("\n\n");



printf("P%i\n", printf("Pbcdef%c", 's'));
printf("\n");
ft_printf("F%i\n", ft_printf("Fbcdef%c", 's'));		printf("\n\n");
printf("\n");




printf("P%i\n", printf("Pbcdef%c\n", 's'));
ft_printf("F%i\n", ft_printf("Fbcdef%c\n", 's'));		printf("\n\n");




// printf("P%i\n", printf("Pbcdef%", 's'));
// printf("\n");
// ft_printf("F%i\n", ft_printf("Fbcdef%", 's'));		printf("\n\n");
// printf("\n");




printf("P%i\n", printf("Pbc%cefg%\n", 's'));
ft_printf("F%i\n", ft_printf("Fbc%cefg%\n", 's'));		printf("\n\n");




printf("P%i\n", printf("Pbc%cde%kf\n", 's'));
ft_printf("F%i\n", ft_printf("Fbc%cde%kf\n", 's'));		printf("\n\n");




printf("P%i\n", printf("Pbc%cde%kf\n", 42));
ft_printf("F%i\n", ft_printf("Fbc%cde%kf\n", 42));		printf("\n\n");




// printf("P%i\n", printf("Pbc %c def %c\n", 'C'));
// ft_printf("F%i\n", ft_printf("Fbc %c def %c\n", 'C'));
		printf("\n\n\n\n\n");











printf("P%i\n", printf("Pbc%sde%cf\n", " cbP ", 't'));
ft_printf("F%i\n", ft_printf("Fbc%sde%cf\n", " cbP ", 't'));	printf("\n\n");




// printf("P%i\n", printf("Pbc%cde%cf\n", 's'));
// ft_printf("F%i\n", ft_printf("Fbc%cde%cf\n", 's'));		printf("\n\n");



// printf("P%i\n", printf("Pbc%cde%cf\n", 's', " cbP "));
// ft_printf("F%i\n", ft_printf("Fbc%cde%cf\n", 's', " cbP "));	printf("\n\n");




printf("P%i\n", printf("Pbc%sdef%s", "Axiom", "Cb\nbp"));
printf("\n");
ft_printf("F%i\n", ft_printf("Fbc%sdef%s", "Axiom", "Cb\nbp"));	printf("\n\n");
printf("\n");




printf("P%i\n", printf("Pbc%sefg%\n", "Xeleffe1"));
ft_printf("F%i\n", ft_printf("Fbc%sefg%\n", "Xeleffe1"));		printf("\n\n");



// char *X;
// printf("P%i\n", printf("Pbcdef %s %s\n", "Xeleffe", X));
// ft_printf("F%i\n", ft_printf("Fbcdef %s %s\n", "Xeleffe", X));
	printf("\n\n");



// printf("P%i\n", printf("Pbcdef %s %s\n", "Xeleffe"));
// ft_printf("F%i\n", ft_printf("Fbcdef %s %s\n", "Xeleffe"));
		printf("\n\n");



// printf("P%i\n", printf("Pbcdef %s %s\n", "Xeleffe1", "Xeleffe2", "Xeleffe3"));
// ft_printf("%i\n", 
// 	ft_printf("Fbcdef %s %s\n", "Xeleffe1", "Xeleffe2", "Xeleffe3"));
// printf("\n\n\n\n\n\n");











printf("P%i\n", printf("Pbcdef%%\n"));
ft_printf("F%i\n", ft_printf("Fbcdef%%\n"));		printf("\n\n");

printf("P%i\n", printf("Pbcdef%%"));
ft_printf("F%i\n", ft_printf("Fbcdef%%"));		printf("\n\n");
printf("P%i\n", printf("%%Pbcdef\n"));
ft_printf("F%i\n", ft_printf("%%Fbcdef\n"));		printf("\n\n");

printf("P%i\n", printf("Pbcdef%%\n", "Xeleffe1"));
ft_printf("F%i\n", ft_printf("Fbcdef%%\n", "Xeleffe1"));		printf("\n\n");

printf("P%i\n", printf("Pbcdef%%", "Xeleffe1"));
printf("\n");
ft_printf("F%i\n", ft_printf("Fbcdef%%", "Xeleffe1"));		printf("\n\n");
printf("\n");

printf("P%i\n", printf("%%%%cdef\n"));
ft_printf("F%i\n", ft_printf("%%%%cdef\n"));		printf("\n\n");

printf("P%i\n", printf("%%%%cdef\n", 1, 2));
ft_printf("F%i\n", ft_printf("%%%%cdef\n", 1, 2));		printf("\n\n");

printf("P%i\n", printf("%%%%%%%"));
ft_printf("F%i\n", ft_printf("%%%%%%%"));		printf("\n\n");

printf("P%i\n", printf("%%%%%%%", 1, 2));
ft_printf("F%i\n", ft_printf("%%%%%%%", 1, 2));		printf("\n\n");
	
printf("P%i\n", printf("%%%%%%", 1, 2));
ft_printf("F%i\n", ft_printf("%%%%%%", 1, 2));		printf("\n\n\n\n\n\n");







int A[1];
int B[1];
int C[1];
int D[1];

*A = 1;
*B = 2;
*C = 3;
*D = 4;

printf("   P%i\n", printf("P %p\n", A));
ft_printf("   F%i\n", ft_printf("F %p\n", A));		printf("\n\n");

printf("   P%i\n", printf("P %p", A));
printf("\n");
ft_printf("   F%i\n", ft_printf("F %p", A));		printf("\n\n");
printf("\n");

printf("   P%i\n", printf("%p", A));
printf("\n");
ft_printf("   F%i\n", ft_printf("%p", A));		printf("\n\n");
printf("\n");

// printf("   P%i\n", printf("%p Pbc %p def %p", A));
// printf("\n");
// ft_printf("   F%i\n", ft_printf("%p Fbc %p def %p", A));		printf("\n\n");
// printf("\n");

// printf("   P%i\n", printf("%p Pbc %p def %p", A, B));
// printf("\n");
// ft_printf("   F%i\n", ft_printf("%p Fbc %p def %p", A, B));
		printf("\n\n");
// printf("\n");

// printf("   P%i\n", printf("%p Pbc %p def %p", A, B, C, D));
// printf("\n");
// ft_printf("   F%i\n", ft_printf("%p Fbc %p def %p", A, B, C, D));
// printf("\n\n\n\n\n\n");
// printf("\n");









printf("   P%i\n", printf("P %X\n", -2045));
ft_printf("   F%i\n", ft_printf("F %X\n", -2045));		printf("\n\n\n\n\n\n");





printf("   P%i\n", printf("P %d\n", -2045424289000));
ft_printf("   F%i\n", ft_printf("F %d\n", -2045424289000));
printf("\n\n\n\n\n\n");





printf("   P%i\n", printf("P %u\n", -2045424289000));
ft_printf("   F%i\n", ft_printf("F %u\n", -2045424289000));		printf("\n\n");

printf("   P%i\n", printf("P %u\n", -20454242890));
ft_printf("   F%i\n", ft_printf("F %u\n", -20454242890));		printf("\n\n");

printf("   P%i\n", printf("P %u\n", 20454242890));
ft_printf("   F%i\n", ft_printf("F %u\n", 20454242890)); printf("\n\n\n\n\n\n");







	printf("\n\n\n\n\n\n\n\n");
	return 0;
}*/