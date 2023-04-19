#include "../../minishell.h"

/**
 * Function gets the value of the environment variable.
 * 
 * @param envp The environment variable array
 * @param key The key with value of interest
 * @return The value of the key
*/
char	*get_envp_value(char **envp, char *key)
{
	int		i;
	char	*value;

	i = 0;
	while (envp[i])
	{
		value = ft_strnstr(envp[i], key, ft_strlen(key));
		if (value)
		{
			value += ft_strlen(key) + 1;
			break ;
		}
		i++;
	}
	return (value);
}

/**
 * Function creates a duplicate from the environment variable parsed from main,
 * to prevent directly modifying the keys and values of the parent shell.
 * 
 * @param envp the environment variable of the parent shell
 * @return Returns a copy of environment variable
*/
char	**dup_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i] != NULL)
		i++;
	new_envp = (char **)ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (envp[i] != NULL)
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[i] = 0;
	return(new_envp);
}

char	*expand_env_dollar(t_vars *vars, char *str)
{
	int		i;
	int		j;
	char	*new_str;
	char	*expanded;
	char	quote_t;

	quote_t = 0;
	new_str = NULL;
	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		if (quote_t && quote_t == str[i])
			quote_t = 0;
		else if (!quote_t && ft_isquote(str[i]))
			quote_t = str[i];
		else if (str[i] == '$' && quote_t != '\'' && str[i + 1] != ' ')
		{
			if (str[++i] == '?')
			{
				new_str = ft_strjoin(new_str, ft_itoa(vars->last_errno));
				j = ft_strlen(ft_itoa(vars->last_errno));
			}
			else
			{
				while (ft_isalnum(str[j + i]))
					j++;
				expanded = get_envp_value(vars->envp, ft_substr(str, i, j));
				if (expanded)
					new_str = ft_strjoin(new_str, expanded);
				i += j - 1;
			}
		}
		else if (str[i] == '*' && !quote_t)
		{
			free(new_str);
			new_str = expand_wildcard(str);
			break ;
		}
		if (!j)
			new_str = ft_strjoin(new_str, ft_substr(str, i, 1));
		i++;
	}
	free(str);
	return (new_str);
}