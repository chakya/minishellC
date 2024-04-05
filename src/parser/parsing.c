/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/05 16:47:24 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_delim(char *c)
{
	return (*c == ' ' || *c == '\t' || *c == '<' || *c == '>' || *c == '|'
		|| *c == '(' || *c == ')' || *c == '\0' || !ft_strncmp(c, "&&", 2));
}

int	loop_quote(char *c)
{
	int		i;
	char	quote;

	quote = *c;
	(c)++;
	i = 1;
	while (*c)
	{
		i++;
		if (*c == quote)
			return (i);
		(c)++;
	}
	return (0);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t');
}

int	ft_isoperation(char *str)
{
	return (*str == '(' || *str == ')' || *str == '|' || !ft_strncmp(str, "&&",
			2) || !ft_strncmp(str, "||", 2) || *str == '<' || *str == '>'
		|| *str == ';');
}

void	ft_skipspaces(char **str)
{
	while (ft_isspace(**str))
		(*str)++;
}

t_type	get_ops_type(char *ops)
{
	if (!ft_strncmp(ops, "&&", 2))
		return (T_AND);
	else if (!ft_strncmp(ops, "||", 2))
		return (T_OR);
	else if (!ft_strncmp(ops, ">>", 2))
		return (T_DR);
	else if (!ft_strncmp(ops, "<<", 2))
		return (T_DL);
	else if (*ops == '(')
		return (T_OPEN_B);
	else if (*ops == ')')
		return (T_CLOSE_B);
	else if (*ops == '|')
		return (T_PIPE);
	else if (*ops == '<')
		return (T_L);
	else if (*ops == '>')
		return (T_R);
	else if (*ops == ';')
		return (T_COL);
	else
		return (T_INVALID);
}

char	*ft_strndup(const char *s, int len)
{
	int		i;
	char	*dup;

	i = 0;
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (((void *)0));
	while (s[i] && i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

t_dls	*tokenize_operation(char **input)
{
	t_dls	*token;

	if (!ft_strncmp(*input, "&&", 2) || !ft_strncmp(*input, "||", 2)
		|| !ft_strncmp(*input, ">>", 2) || !ft_strncmp(*input, "<<", 2))
	{
		token = ft_dlsnew(ft_strndup(*input, 2), get_ops_type(*input));
		(*input)++;
	}
	else
		token = ft_dlsnew(ft_strndup(*input, 1), get_ops_type(*input));
	(*input)++;
	return (token);
}

t_dls	*tokenize_param(char **input)
{
	t_dls	*token;
	int		l;

	l = 0;
	if (**input == '"' || **input == '\'')
	{
		l = loop_quote(*input);
		if (l <= 0)
			printf("quote not closed");
	}
	else
	{
		while (!is_delim((*input) + l))
			l++;
	}
	token = ft_dlsnew(ft_strndup(*input, l), T_ARG);
	*input += l;
	return (token);
}

t_dls	*parse_token(char *input)
{
	t_dls	*tokens;
	t_dls	*token;

	tokens = NULL;
	while (*input)
	{
		ft_skipspaces(&input);
		if (get_ops_type(input))
			token = tokenize_operation(&input);
		else
			token = tokenize_param(&input);
		ft_dlsadd_back(&tokens, token);
	}
	return (tokens);
}

int run_builtin(char **av, char **envp)
{
	int exit_status;

	if (!ft_strncmp(av[0], "echo", 5))
		exit_status = echo(av);
	// else if (!ft_strncmp(av[0], "cd"))
	// 	cd(av[1]);
	else if (!ft_strncmp(av[0], "env", 4))
		exit_status = env(av, envp);
	else if (!ft_strncmp(av[0], "pwd", 4))
		pwd();
	// else if (!ft_strncmp(av[0], "export"))
	// 	export(av);
	// else if (!ft_strncmp(av[0], "unset"))
	// 	unset(av);
	return (exit_status);
}

char **parse_to_arg(t_dls *tokens)
{
	t_dls	*tmp;
	char	**av;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp)
	{
		if (tmp->type == T_ARG)
			i++;
		tmp = tmp->next;
	}
	av = (char **)malloc((i + 1) * sizeof(char *));
	if (!av)
		return (0);
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			av[i] = ft_strdup(tmp->content);
			i++;
		}
		tmp = tmp->next;
	}
	av[i] = NULL;
	return (av);
}

char *get_dollar(char *str, char **envp)
{
	int		i;
	int		j;
	char	*var;
	int		varlen;

	j = 1;
	while (str[j] && !ft_isspace(str[j]) && !is_delim(&str[j]))
		j++;
	var = ft_strndup(str + 1, j - 1);
	varlen = ft_strlen(var);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, varlen))
		{
			free(var);
			return (ft_strdup(envp[i] + varlen + 1));
		}
		i++;
	}
	free(var);
	return (str);
}

char **process_av(t_dls *tokens, char **envp)
{
	t_dls	*tmp;
	char	**av;
	char	*dollar_var;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp)
	{
		if (tmp->content[0] == '$')
		{
			dollar_var = get_dollar(tmp->content, envp);
			free(tmp->content);
			tmp->content = dollar_var;
		}
		//else if for * wild card
		tmp = tmp->next;
	}
	av = parse_to_arg(tokens);
	return (av);
}

int	execute(t_dls *tokens, char **envp)
{
	char	**av;

	av = process_av(tokens, envp);
	return run_builtin(av, envp);
}
