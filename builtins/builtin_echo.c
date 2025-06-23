#include "../minishell.h"

int is_option_n(const char *str)
{
    int i = 0;

    if (str[i] != '-')
        return (0);
    i++;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int builtin_echo(int argc, char **argv)
{
    int i = 1;
    int newline = 1;

    while (i < argc && is_option_n(argv[i]))
    {
        newline = 0;
        i++;
    }
    while (i < argc)
    {
        printf("%s", argv[i]);
        if (i + 1 < argc)
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}