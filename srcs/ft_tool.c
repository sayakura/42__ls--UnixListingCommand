/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tool.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 16:40:33 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 16:40:34 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char			*get_path(char *path, char *filename)
{
	char	*path_;
	char	*full_path;

	path_ = NULL;
	if (ft_strcmp(path, "/") != 0)
		path_ = ft_strjoin(path, "/");
	else
		path_ = ft_strdup("/");
	full_path = ft_strjoin(path_, filename);
	if (path)
		free(path_);
	return (full_path);
}

struct stat		*get_stat(char *path, char *filename)
{
	struct stat	*sb;
	char		*full_path;

	full_path = NULL;
	if (filename != NULL)
		full_path = get_path(path, filename);
	else
		full_path = ft_strdup(path);
	sb = malloc(sizeof(struct stat));
	if (lstat(full_path, sb) == -1)
		ring_buffer_op(5, "stat: ", filename, ": ", sys_errlist[errno], "\n");
	if (full_path)
		free(full_path);
	return (sb);
}

void			acl_handler(t_dpsb *dpsb)
{
	acl_t			acl;

	acl = NULL;
	if (listxattr(dpsb->name, NULL, 0, XATTR_NOFOLLOW) > 0)
		dpsb->extended = '@';
	else
		dpsb->extended = ' ';
}

char			*replace_tilde_with_path(char *path)
{
	extern char	**environ;
	int			i;
	char		*real_path;

	i = 0;
	real_path = NULL;
	if (ft_strchr(path, '~'))
	{
		while (environ[i])
		{
			if (ft_strstr(environ[i], "HOME") == NULL)
				i++;
			else
			{
				real_path = ft_strdup(environ[i] + 5);
				break ;
			}
		}
		return (real_path == NULL ? ft_strdup(path) : real_path);
	}
	else
		return (real_path = ft_strdup(path));
}

void			*display_error_message(char *path)
{
	static char	buffer[300];
	int			i;
	char		*pointer;

	i = ft_strlen(path);
	pointer = path;
	while (path[i] != '/' && i--)
		;
	if (i > 0)
	{
		ft_strcpy(buffer, path + i + 1);
		pointer = buffer;
	}
	if (errno == 9)
		ring_buffer_op(5, "\e[1m\e[31mls: ", pointer, ": ",\
							"Not a directory", "\e[0m\n");
	else
		ring_buffer_op(5, "\e[1m\e[31mls: ", pointer, ": ",\
							sys_errlist[errno], "\e[0m\n");
	return (NULL);
}
