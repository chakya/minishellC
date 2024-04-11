/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/11 16:11:17 by cwijaya          ###   ########.fr       */
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
	if (!ft_strncmp(ops, "||", 2))
		return (T_OR);
	else if (!ft_strncmp(ops, ">>", 2))
		return (T_APPEND);
	else if (!ft_strncmp(ops, "<<", 2))
		return (T_HEREDOC);
	else if (*ops == '(')
		return (T_OB);
	else if (*ops == ')')
		return (T_CB);
	else if (*ops == '|')
		return (T_PIPE);
	else if (*ops == '<')
		return (T_INPUT);
	else if (*ops == '>')
		return (T_TRUNC);
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

int count_pipe(t_dls *tokens)
{
	int count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			count++;
		tokens = tokens->next;
	}

	return count;
}

t_dls *copy_prev(t_dls **tokens)
{
	t_dls *temp;

	temp = (*tokens)->prev;

	if ((*tokens)->next)
	{
		*tokens = (*tokens)->next;
		free((*tokens)->prev);
		(*tokens)->prev = NULL;
		temp->next = NULL;
	}
	else
		*tokens = NULL;
	while (temp->prev)
		temp = temp->prev;
	return temp;
}

t_dls *copy_next(t_dls **tokens)
{
	*tokens = (*tokens)->next;
	free((*tokens)->prev);
	(*tokens)->prev = NULL;
	return (*tokens);
}

t_type	check_redirect(t_dls *tokens)
{
	while (tokens)
	{
		if (tokens->type >= T_TRUNC)
			return (tokens->type);
		tokens = tokens->next;
	}
	return T_CMD;
}

t_ast **populate_children(t_dls *tokens, int count)
{
	t_ast **children;
	int i;

	i = 0;
	children = (t_ast **)malloc(sizeof(t_ast *) * (count + 2));
	while (tokens)
	{
		if (tokens->type == T_PIPE || tokens->next == NULL)
		{
			children[i] = (t_ast *)malloc(sizeof(t_ast));
			children[i]->tokens = copy_prev(&tokens);
			children[i]->type = check_redirect(children[i]->tokens);
			i++;
		}
		else
			tokens = tokens->next;
	}
	children[i] = NULL;
	return children;
}

t_ast *parse_ast(t_dls *tokens)
{
	t_ast *ast;
	int	count;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	count = count_pipe(tokens);
	if (count > 0)
	{
		ast->type = T_PIPE;
		ast->children = populate_children(tokens, count);
	}
	else
	{
		ast->tokens = tokens;
		ast->type = check_redirect(tokens);
	}
	return ast;
}

int execute_pipe(t_ast **children, t_minishell **mnst, char **envp, int first)
{
	int fd[2];
	int id;

	if (!*children)
		return 1;
	if (pipe(fd) == -1){
		printf("err");
		return (1);
	}
	id = fork();
	if (id == 0){
		if (children[1])
			dup2(fd[1], STDOUT_FILENO);
		if (!first)
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execute_ast(*children, mnst, envp);
		exit(1);
	}
	else
	{
		execute_pipe(&children[1], mnst, envp, 0);
		close(fd[0]);
		close(fd[1]);
		waitpid(id, NULL, 0);
	}
	return (0);
}

int	execute_tokens(t_dls *tokens, t_minishell **mnst, char **envp)
{
	char	**av;

	av = process_av(tokens, envp);
	if (!av)
		return (1);
	excu(av, mnst);
	return (0);
}

int execute_ast(t_ast *ast, t_minishell **mnst, char** envp)
{
	if (!ast)
		return 0;
	if (ast->type == T_PIPE)
		execute_pipe(ast->children, mnst, envp, 1);
	if (ast->type == T_CMD)
		execute_tokens(ast->tokens, mnst, envp);
	return 1;
}
