#include "../minishell.h"

int builtin_pwd(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        return (1);
    }
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}
