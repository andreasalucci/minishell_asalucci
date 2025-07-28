#include "../minishell.h"

void    free_quotes(char *str1, char *str2, char *str3)
{
    free(str1);
    free(str2);
    free(str3);
}