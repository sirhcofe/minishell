#include "../../minishell.h"

t_token *create_token(char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        exit(1);
    value = ft_trim_paren(value);
    new_token->left = NULL;
    new_token->right = NULL;
    new_token->value = ft_strdup(value);
    new_token->operator = get_operator_type(value); // redundant?
    new_token->cmdlst = generate_pipe_list(value, new_token);
    new_token->exit_status = 0;
    return (new_token);
}

int find_lowest_precedence(char **tokens, int start, int end, int *lowest_precedence)
{
    int	i;
    int	parens;
    int	precedence;
    int	lowest_precedence_index;

    i = start;
    parens = 0;
    lowest_precedence_index = -1;
    *lowest_precedence = INT_MAX;
    while (i <= end)
    {
        if (is_left_paren(tokens[i]))
            parens++;
        else if (is_right_paren(tokens[i]))
            parens--;
        else if (parens == 0 && is_operator(tokens[i]))
        {
            precedence = (ft_strcmp(tokens[i], "&&") == 0) ? 1 : 0;
            if (precedence <= *lowest_precedence)
            {
                lowest_precedence_index = i;
                *lowest_precedence = precedence;
            }
        }
        i++;
    }
    return (lowest_precedence_index);
}

int is_balanced(char **tokens, int start, int end)
{
    int parens;

    if (!is_left_paren(tokens[start]) || !is_right_paren(tokens[end]))
        return (0);
    parens = 0;
    for (int i = start; i <= end; i++)
    {
        if (is_left_paren(tokens[i]))
            parens++;
        else if (is_right_paren(tokens[i]))
            parens--;
        if (parens < 0)
            return (0);
    }
    return (parens == 0);
}

t_token *build_token_tree(char **tokens, int start, int end)
{
    int		lowest_precedence_index;
    int		lowest_precedence;
    t_token	*new_token;

    if (start > end)
        return (NULL);
    lowest_precedence_index = find_lowest_precedence(tokens, start, end, &lowest_precedence);
    if (lowest_precedence_index != -1)
    {
        new_token = create_token(tokens[lowest_precedence_index]);
        new_token->left = build_token_tree(tokens, start, lowest_precedence_index - 1);
        new_token->right = build_token_tree(tokens, lowest_precedence_index + 1, end);
        return (new_token);
    }
    else if (is_balanced(tokens, start, end))
        return (build_token_tree(tokens, start + 1, end - 1));
    else
        return (create_token(tokens[start]));
}
