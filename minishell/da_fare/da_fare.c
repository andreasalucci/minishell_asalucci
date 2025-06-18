int builtin_cd(t_env **env, char **args)

void builtin_env(t_env *env)


Quando vorrai gestire anche:

export VAR=value

errori tipo export 123INVALID

export VAR (senza =)

... dovrai aggiungere:

parsing dell’argomento

funzione is_valid_identifier

divisione key=value