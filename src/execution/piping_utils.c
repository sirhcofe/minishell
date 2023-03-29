/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zwong <zwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:22:35 by chenlee           #+#    #+#             */
/*   Updated: 2023/03/29 13:39:18 by zwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * Function creates a backup of current pipefd which will be used for the next
 * function, if there is an existing backup, function closes them, then close
 * the current pipefd.
 * 
 * @param index The current command list
 * @param n_cmds Total number of commands
 * @param pipefd The file 
*/
void	ft_close_pipe(int index, int n_cmds, int pipefd[2][2])
{
	if (index != 0)
		close(pipefd[1][0]);
	if (index != n_cmds - 1)
		pipefd[1][0] = pipefd[0][0];
	close(pipefd[0][1]);
	if (index == n_cmds - 1)
		close(pipefd[0][0]);
}

char	*get_readline(char *rdr_str)
{
	char	*temp_one;
	char	*temp_two;
	char	*str;

	str = NULL;
	while (1)
	{
		temp_one = readline("> ");
		if (ft_strncmp(temp_one, rdr_str, ft_strlen(rdr_str)) == 0)
			break ;
		temp_two = ft_strjoin(temp_one, "\n");
		free(temp_one);
		temp_one = ft_strjoin(str, temp_two);
		if (str != NULL)
			free(str);
		free(temp_two);
		str = ft_strdup(temp_one);
		free(temp_one);
	}
	return (str);
}

int	do_heredoc(char *cmd, t_rdrinfo info)
{
	char	*gnl;
	int		pipefd[2];
	
	if (pipe(pipefd) == -1)
		exit(error(cmd, "pipe failed"));
	gnl = get_readline(info.rdr_str);
	write(pipefd[1], gnl, ft_strlen(gnl));
	close(pipefd[1]);
	return (pipefd[0]);
}

void	ft_open(char *cmd, t_rdrinfo info, int *fd_in, int *fd_out)
{
	if ((info.rdr_type == IN || info.rdr_type == HEREDOC))
	{
		if (*(fd_in) != -42)
			close(*(fd_in));
		if (info.rdr_type == IN)
			*(fd_in) = open(info.rdr_str, O_RDONLY);
		else if (info.rdr_type == HEREDOC)
			*(fd_in) = do_heredoc(cmd, info);
		if (*(fd_in) == -1)
			exit(error(cmd, "open error"));
	}
	else if ((info.rdr_type == OUT || info.rdr_type == APPEND))
	{
		if (*(fd_out) != -42)
			close(*(fd_out));
		if (info.rdr_type == OUT)
			*(fd_out) = open(info.rdr_str, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		else if (info.rdr_type == APPEND)
			*(fd_out) = open(info.rdr_str, O_CREAT | O_APPEND);
		if (*(fd_out) == -1)
			exit(error(cmd, "open error"));
	}
}
