/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:28:45 by dphang            #+#    #+#             */
/*   Updated: 2024/04/24 15:19:18 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# include "../lib/libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/ioctl.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

typedef struct s_envp
{
	char			*content;
	struct s_envp	*next;
}					t_envp;

typedef enum s_type
{
	T_INVALID = 0,
	T_EMPTY,
	T_CMD,
	T_ARG,
	T_COL,
	T_OB,
	T_CB,
	T_OR,
	T_AND,
	T_PIPE,
	T_INPUT,
	T_HEREDOC,
	T_TRUNC,
	T_APPEND
}					t_type;

typedef struct s_dls
{
	char			*content;
	t_type			type;
	int				heredoc;
	struct s_dls	*prev;
	struct s_dls	*next;
}					t_dls;

typedef struct s_ast
{
	t_type			type;
	struct s_dls	*tokens;
	struct s_ast	**children;
}					t_ast;

typedef struct s_minishell
{
	t_envp			*envp;
	int				exit_sig;
	unsigned char	exit_code;
	int				is_child;
	t_ast			*ast;
	int				*opipe;
	int				io[2];
}					t_minishell;

typedef struct s_parsestr
{
	int				i;
	int				j;
	int				sgl_quote;
	int				dbl_quote;
	char			*temp;
}					t_parsestr;

extern int			g_sig_received;

t_dls				*parse_token(char *input, t_minishell **mnsh);
t_dls				*ft_dlsnew(char *content, t_type type);
void				ft_dlsadd_back(t_dls **lst, t_dls *new);
int					execute(t_dls *tokens, char **envp);
t_ast				*parse_ast(t_dls *tokens, t_minishell **mnsh);
int					execute_ast(t_minishell **mnst, int *opipe);
//	=========================   built-ins   ===================================
//	builtins_utils
int					ft_strcmp(const char *s1, const char *s2);
int					is_redir(char *str);
//  cd
int					cd(char **cmd, t_minishell **mnsh);
//	echo
int					echo(char **cmd);
//	env
int					env(char **cmd, t_minishell *mnsh);
//  exit
int					mnsh_exit(char **cmd, t_minishell **mnsh);
//  export_sort
void				sort_print(t_envp *envp);
//  export_utils
t_envp				*envp_exist(char *envp, t_minishell **mnsh);
void				add_exp(char *envp, t_minishell **mnsh);
char				*enclose_val(char *envp);
t_envp				*dup_envp(t_envp *envp);
int					is_validenvar(char *envp);
//  export
int					export(char **cmd, t_minishell **mnsh);
//	pwd
int					pwd(void);
//  unset
int					unset(char **cmd, t_minishell **mnsh);
//  =========================   execution   ===================================
//  excu
int					excu(char **cmd, t_minishell **mnsh);
int					is_builtins(char **cmd);
//	excu_cmd_utils
int					is_executable(char *cmd);
int					envp_size(t_envp *envp);
char				**dup_envparr(t_envp *envp);
//  excu_cmd
char				**get_path(t_envp *envp);
void				excu_cmd(char **cmd, t_minishell **mnsh);
//  =========================   initialization   ==============================
t_envp				*newenvp(char *var);
void				init_mnsh(char **envp, t_minishell **mnsh);
//  =========================   signals   =====================================
void				sigint_handler(int sig);
void				eof_handler(t_minishell **mnsh);
//  =========================   free   ========================================
void				free_envp(t_envp **envp);
void				free_all(t_minishell **mnsh);
t_dls				*free_tokens(t_dls *tokens);
void				free_ast(t_ast *ast);
void				free_arr(char ***arr);
//  =========================   parsing   =====================================
//	parse_dollar
int					envar_exist(char *str);
int					is_envar(char *str);
char				*expand_dollar(char *str, t_minishell **mnsh);
void				apnd_expsn(char *str, t_parsestr *pstr, t_minishell **mnsh);
char				*enval(char *var, t_envp *envp);
//	parse_quotes
void				parse_sglquote(t_parsestr *pstr, char *str);
void				parse_dblquote(t_parsestr *pstr, char *str,
						t_minishell **mnsh);
//	parse_string
void				process_dollar(char *str, t_parsestr *pstr,
						t_minishell **mnsh);
char				*parse_string(char *str, t_minishell **mnsh);
// parser utils
int					is_delim(char *c);
int					ft_isspace(char c);
int					loop_quote(char *c);
int					ft_isoperation(char *str);
void				ft_skipspaces(char **str);
// token
t_type				get_ops_type(char *ops);
t_dls				*tokenize_operation(char **input);
t_dls				*tokenize_param(char **input, t_minishell **mnsh,
						t_dls *token);
// pipe
t_ast				**populate_children(t_dls *tokens, int count,
						t_minishell **mnsh);
void				check_heredoc(t_dls *tokens, t_minishell **mnsh);
int					count_pipe(t_dls *tokens);
int					delim_check(char *hline, char *delim);
// exe token
int					execute_tokens(t_dls *tokens, t_minishell **mnsh);
// parsing
char				**process_av(t_dls *tokens, t_minishell **mnsh);
char				**parse_to_arg(t_dls *tokens);
// token utils
t_dls				*parse_token(char *input, t_minishell **mnsh);
char				**parse_to_arg(t_dls *tokens);

#endif
