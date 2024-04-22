/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/22 17:28:47 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_delim(char *c)
{
	return (*c == ' ' || *c == '\t' || *c == '<' || *c == '>' || *c == '|'
		|| *c == '(' || *c == ')' || *c == '\0' || !ft_strncmp(c, "&&", 2));
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t');
}

int	loop_quote(char *c)
{
	int		i;
	int		is_closed;
	char	quote;

	quote = *c;
	(c)++;
	i = 1;
	is_closed = 0;
	while (*c)
	{
		i++;
		if (*c == quote && !is_closed)
			is_closed = 1;
		else if ((*c == '\'' || *c == '"') && is_closed)
		{
			is_closed = 0;
			quote = *c;
		}
		(c)++;
		if (ft_isspace(*c) && is_closed)
			return (i);
	}
	return (i);
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
	else if (!ft_strncmp(ops, "&&", 2))
		return (T_AND);
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
	while (!is_delim((*input) + l))
	{
		if ((*input)[l] == '"' || (*input)[l] == '\'')
			l += loop_quote(*input);
		else
			l++;
	}
	if (l == 0)
		return (NULL);
	token = ft_dlsnew(ft_strndup(*input, l), T_ARG);
	*input += l;
	return (token);
}

void free_tokens(t_dls *tokens)
{
	t_dls *temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->content);
		free(temp);
	}
}

void free_ast(t_ast *ast)
{
	if (ast->type == T_PIPE)
	{
		int i = 0;
		while (ast->children[i])
		{
			free_ast(ast->children[i]);
			i++;
		}
	}
	free_tokens(ast->tokens);
	free(ast);
}

t_dls	*parse_token(char *input)
{
	t_dls	*tokens;
	t_dls	*token;

	tokens = NULL;
	ft_skipspaces(&input);
	while (*input)
	{
		if (get_ops_type(input))
			token = tokenize_operation(&input);
		else
			token = tokenize_param(&input);
		ft_dlsadd_back(&tokens, token);
		if(!tokens)
		{
			free_tokens(tokens);
			return (NULL);
		}
		if (token->type == T_PIPE && token->prev && token->prev->type >= T_INPUT)
			break;
		ft_skipspaces(&input);
	}
	if(tokens && (token->type == T_PIPE || token->type >= T_INPUT))
	{
		ft_putstr_fd("Syntax Error\n", 2);
		free_tokens(tokens);
		tokens = NULL;
	}
	else if(tokens && (token->type >= T_COL && token->type <= T_AND))
	{
		ft_putstr_fd("Not supported\n", 2);
		free_tokens(tokens);
		tokens = NULL;
	}
	return (tokens);
}

char	**parse_to_arg(t_dls *tokens)
{
	t_dls	*tmp;
	char	**av;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp)
	{
		if (tmp->type == T_ARG && tmp->content[0] != '\0')
			i++;
		tmp = tmp->next;
	}
	if (i == 0)
		return (NULL);
	av = (char **)malloc((i + 1) * sizeof(char *));
	if (!av)
		return (0);
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == T_ARG && tmp->content[0] != '\0')
		{
			av[i] = ft_strdup(tmp->content);
			i++;
		}
		tmp = tmp->next;
	}
	av[i] = NULL;
	return (av);
}

// char *get_dollar(char *str, char **envp)
// {
// 	int		i;
// 	int		j;
// 	char	*var;
// 	int		varlen;

// 	j = 1;
// 	while (str[j] && !ft_isspace(str[j]) && !is_delim(&str[j]))
// 		j++;
// 	var = ft_strndup(str + 1, j - 1);
// 	varlen = ft_strlen(var);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (!ft_strncmp(envp[i], var, varlen))
// 		{
// 			free(var);
// 			return (ft_strdup(envp[i] + varlen + 1));
// 		}
// 		i++;
// 	}
// 	free(var);
// 	return (str);
// }

char **process_av(t_dls *tokens, t_minishell **mnsh)
{
	t_dls	*tmp;
	char	**av;
	char	*dollar_var;

	tmp = tokens;
	while (tmp)
	{
		if (ft_strchr(tmp->content, '\'') || ft_strchr(tmp->content, '\"')
			|| envar_exist(tmp->content))
		{
			dollar_var = parse_string(tmp->content, mnsh);
			free(tmp->content);
			tmp->content = dollar_var;
		}
		tmp = tmp->next;
	}
	av = parse_to_arg(tokens);
	return (av);
}


int	count_pipe(t_dls *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_dls	*copy_prev(t_dls **tokens)
{
	t_dls	*temp;

	temp = (*tokens)->prev;
	if ((*tokens)->next)
	{
		*tokens = (*tokens)->next;
		free((*tokens)->prev->content);
		free((*tokens)->prev);
		(*tokens)->prev = NULL;
		temp->next = NULL;
	}
	else
	{
		temp = *tokens;
		*tokens = NULL;
	}
	while (temp->prev)
		temp = temp->prev;
	return (temp);
}

t_dls	*copy_next(t_dls **tokens)
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
		if (tokens->type >= T_INPUT && tokens->type <= T_APPEND)
			return (tokens->type);
		tokens = tokens->next;
	}
	return (T_CMD);
}

int delim_check(char *hline, char*delim)
{
	while (*hline)
	{
		if (*delim == '\'' || *delim == '"')
			delim++;
		if (*delim == *hline)
		{
			hline++;
			delim++;
		}
		else
		{
			return (0);
		}
	}
	while (*delim)
	{
		if (*delim != '\'' && *delim != '"')
			return (0);
	}
	return (1);

}

void	interupt_handler(int signum)
{
	if (signum == SIGINT)
	{
		//ft_putstr_fd("\n", 1);
		//rl_on_new_line();
		sigint_handler(signum);
		g_sig_received = 1;
		exit(130);
	}
}

void	check_heredoc(t_dls *tokens)
{
	char *delim;
	char *hline;
	int	fd[2];
	int pid;

	while(tokens)
	{
		if (tokens->type == T_HEREDOC)
		{
			if (!tokens->next)
			{
				ft_putstr_fd("Syntax Error",2);
				exit(2);
			}
			delim = tokens->next->content;
			pipe(fd);
			pid = fork();
			if (!pid)
			{
				signal(SIGINT, interupt_handler);
				while (delim)
				{
					hline = readline("> ");
					// if (!hline)
					// 	break;
					if (delim_check(hline, delim))
						break;
					ft_putstr_fd(hline, fd[1]);
					ft_putstr_fd("\n", fd[1]);
					free(hline);
				}
				close(fd[1]);
				close(fd[0]);
				exit(0);
			}
			close(fd[1]);
			tokens->heredoc = fd[0];
			waitpid(pid, NULL, 0);
		}
		tokens = tokens->next;
	}
	if (g_sig_received)
	{}
}

t_ast	**populate_children(t_dls *tokens, int count)
{
	t_ast	**children;
	int		i;

	i = 0;
	children = (t_ast **)malloc(sizeof(t_ast *) * (count + 2));
	while (tokens)
	{
		if (tokens->type == T_PIPE || tokens->next == NULL)
		{
			children[i] = (t_ast *)malloc(sizeof(t_ast));
			children[i]->tokens = copy_prev(&tokens);
			check_heredoc(children[i]->tokens);
			children[i]->type = T_CMD;
			i++;
		}
		else
			tokens = tokens->next;
	}
	children[i] = NULL;
	return (children);
}

void	init_ast(t_ast *ast)
{
	ast->type = T_INVALID;
	ast->tokens = NULL;
	ast->children = NULL;
}

t_ast	*parse_ast(t_dls *tokens)
{
	t_ast	*ast;
	int		count;

	ast = (t_ast *)malloc(sizeof(t_ast));
	init_ast(ast);
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
		check_heredoc(tokens);
		ast->type = T_CMD;// check_redirect(tokens);
	}
	return (ast);
}

char	*get_filename(t_dls *tokens)
{
	while (tokens->next || (tokens->next && is_redir(tokens->next->content)))
	{
		tokens = tokens->next;
	}
	return (tokens->content);
}

void empty_string(t_dls *tokens)
{
	if (!tokens)
		return ;
	char *str=tokens->content;
	while (*str)
	{
		*str = '\0';
		str++;
	}
	tokens->type = T_EMPTY;
}

void rem2(t_dls *tokens)
{
	empty_string(tokens);
	if (tokens->next)
		empty_string(tokens->next);
	else
	{
		printf("Invalid syntax");
	}
}

int	proc_redir(t_dls *tokens, t_minishell *mnsh)
{
	int		fd[2] = {-1, -1};

	while (tokens)
	{
		if (tokens->type == T_INPUT)
		{
			fd[0] = open(tokens->next->content, O_RDONLY);
			if (fd[0] < 0)
			{
				ft_putstr_fd(" No such file or directory\n", 2);
				return (1);
			}
			rem2(tokens);
		}
		else if (tokens->type == T_HEREDOC)
		{
			fd[0] = tokens->heredoc;
			rem2(tokens);
		}
		else if (tokens->type == T_TRUNC)
		{
			fd[1] = open(tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd[1] < 0)
			{
				ft_putstr_fd(" Permission denied\n", 2);
				return (1);
			}
			rem2(tokens);
		}
		else if (tokens->type == T_APPEND)
		{
			fd[1] = open(tokens->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd[1] < 0)
			{
				ft_putstr_fd(" Permission denied\n", 2);
				return (1);
			}
			rem2(tokens);
		}
		tokens = tokens->next;
	}

	mnsh->io[0] = dup(STDIN_FILENO);
	mnsh->io[1] = dup(STDOUT_FILENO);
	if (fd[0] > 2)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	else if (fd[1] > 2)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	return (0);
}

void free_av(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		free(av[i]);
		i++;
	}
	free(av);
}

int	execute_tokens(t_dls *tokens, t_minishell **mnsh)
{
	char	**av;

	if (proc_redir(tokens, *mnsh))
		return (1);
	av = process_av(tokens, mnsh);
	if (!av)
		return (0);
	(*mnsh)->exit_code = excu(av, mnsh);
	free_av(av);
	return (0);
}

int	execute_pipe(t_ast **children, int *opipe, t_minishell **mnsh)
{
	int	fd[2] = {0, 0};
	int	id;

	if (!*children)
		return (1);
	if (children[1])
		pipe(fd);
	id = fork();
	if (id == 0)
	{
		if (children[1])
			dup2(fd[1], STDOUT_FILENO);
		if (opipe)
			dup2(opipe[0], STDIN_FILENO);
		if (fd[0])
		{
			close(fd[0]);
			close(fd[1]);
		}
		if (opipe)
		{
			close(opipe[0]);
			close(opipe[1]);
		}
		execute_tokens((*children)->tokens, mnsh);
		exit(1);
	}
	else
	{
		if (opipe)
		{
			close(opipe[0]);
			close(opipe[1]);
		}
		execute_pipe(&children[1], fd, mnsh);
		waitpid(id, NULL, 0);
	}
	return (0);
}

void handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sig_received = 1;
		ft_putstr_fd("\n", 1);
		exit(0);
	}
}

int	execute_ast(t_minishell **mnsh, int *opipe)
{
	int	id;
	int	exit_status;

	if (!(*mnsh)->ast)
		return (0);
	if ((*mnsh)->ast->type == T_PIPE)
		execute_pipe((*mnsh)->ast->children, opipe, mnsh);
	else if ((*mnsh)->ast->type == T_CMD)
	{
		if (is_builtins(&(*mnsh)->ast->tokens->content))
		{
			execute_tokens((*mnsh)->ast->tokens, mnsh);
			dup2((*mnsh)->io[0], STDIN_FILENO);
			dup2((*mnsh)->io[1], STDOUT_FILENO);
		}
		else
		{
			id = fork();
			if (id == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				if (execute_tokens((*mnsh)->ast->tokens, mnsh))
					exit(1);
				exit(0);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				waitpid(id, &exit_status, 0);
				if (WIFEXITED(exit_status))
					(*mnsh)->exit_code = WEXITSTATUS(exit_status);
				signal(SIGINT, &sigint_handler);
			}
		}
	}
	free_ast((*mnsh)->ast);
	(*mnsh)->ast = NULL;
	return (1);
}
