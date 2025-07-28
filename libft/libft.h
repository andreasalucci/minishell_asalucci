/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:32:56 by asalucci          #+#    #+#             */
/*   Updated: 2025/05/22 11:15:58 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>

# define BUFFER_SIZE 30

size_t			ft_strlen(const char *s);
int				ft_strlenc(char *str, char c);
void			*ft_memset(void *s, int c, size_t n);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *s);
char			*ft_substr(char const *s, unsigned int start, size_t len);
int				ft_atoi(const char *nptr);
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
char			*ft_itoa(int n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			ft_putchar_fd(char c, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int nb, int fd);
void			ft_putstr_fd(char *str, int fd);
char			**ft_split(char const *s, char c);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
char			*ft_strjoin(char const *s1, char const *s2);
size_t			ft_strlcat(char *dst, const char *src, size_t size);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_strrchr(const char *s, int c);
char			*ft_strtrim(char const *s1, char const *set);
int				ft_tolower(int c);
int				ft_toupper(int c);
char			*cut_string(char *s, char cut_from_char);
char			*f_constrn(char *constr);
char			*get_next_line(int fd);
int				ft_strcmp(const char *s1, const char *s2);
char 			*ft_strcpy(char *dest, const char *src);

char			*ft_itoa(int n);
size_t			print_char_fu(char a, size_t i, size_t *r);
size_t			print_str_fu(char *a, size_t i, size_t *r);
size_t			print_pointer_fu(void *a, size_t i, size_t *r);
size_t			print_dec_fu(long a, size_t i, size_t *r);
size_t			print_int_fu(int a, size_t i, size_t *r);
char			*dtoa_fu(unsigned int n);
size_t			print_unsdec_fu(unsigned int a, size_t i, size_t *r);
unsigned int	if_negative(unsigned int nbr, size_t *r);
void			recursive(unsigned long nbr, char *hexab,
					size_t *r);
void			recursive_neg_min(unsigned long nbr, char *hexab,
					size_t *r);
size_t			print_hexa_fu(unsigned int nbr, size_t i,
					size_t *r, char *hexab);
size_t			print_hexa_p_fu(unsigned long nbr, size_t i, size_t *r,
					char *hexab);
size_t			print_percent_fu(size_t i, size_t *r);
void			write_null_fu(void);
int				ft_printf(const char *str, ...);

typedef struct s_node
{
	int				data;
	int				index;
	struct s_node	*next;
}	t_node;

typedef struct s_str_node
{
	char				*line;
	struct s_str_node	*next;
}	t_str_node;

void			swap_int(int *a, int *b);
void			*ft_memcpy(void *dest, const void *src, size_t n);
long long int	number(const char *nb_string);
long long int	ft_atolli(const char *nptr);

t_node			*create_node(int data, int index);
void			free_list(t_node *head);
t_node			*array_to_list(int *arr, int size);
int				*f_bubble_sort(int *arr, int size);
t_node			*f_corresponding_index(t_node *a, t_node *sorted_list);

int				f_is_number(char *str);
int				is_list_ordered(t_node *a);
int				check_duplicates(int *arr, int size);
int				process_args(int argc, char **argv, int **initial_arr,
					long long int **ini_arr_l);

void			f_pa(t_node **a, t_node **b);
void			f_pb(t_node **a, t_node **b);
void			f_rotate(t_node **stack);
void			f_ra(t_node **a);
void			f_rb(t_node **b);

void			process_bit(t_node **a, t_node **b, int i, int argc);
int				algorithm(t_node **a, t_node **b, int argc);
void			f_just_three(t_node *a, int plus);
void			f_just_four(t_node **a, t_node **b, int plus);
void			f_just_five(t_node **a, t_node **b);

int				f_in_the_limits(char *number);

void			free_str_list(t_str_node *head);

int 			str_is_numeric(const char *str);

#endif
