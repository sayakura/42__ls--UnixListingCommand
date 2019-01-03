/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_l.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 16:06:04 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 16:06:06 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	filetypeletter(int mode)
{
	char	c;

	if (S_ISREG(mode))
		c = '-';
	else if (S_ISDIR(mode))
		c = 'd';
	else if (S_ISBLK(mode))
		c = 'b';
	else if (S_ISCHR(mode))
		c = 'c';
	else if (S_ISFIFO(mode))
		c = 'p';
	else if (S_ISLNK(mode))
		c = 'l';
	else if (S_ISSOCK(mode))
		c = 's';
	else
		c = '?';
	return (c);
}

char		*lsperms(int mode, char extended)
{
	static const char	*rwx[] = {"---", "--x", "-w-", "-wx",
	"r--", "r-x", "rw-", "rwx"};
	static char			bits[12];

	bits[0] = filetypeletter(mode);
	ft_strcpy(&bits[1], rwx[(mode >> 6) & 7]);
	ft_strcpy(&bits[4], rwx[(mode >> 3) & 7]);
	ft_strcpy(&bits[7], rwx[(mode & 7)]);
	if (mode & S_ISUID)
		bits[3] = (mode & S_IXUSR) ? 's' : 'S';
	if (mode & S_ISGID)
		bits[6] = (mode & S_IXGRP) ? 's' : 'l';
	if (mode & S_ISVTX)
		bits[9] = (mode & S_IXOTH) ? 't' : 'T';
	bits[10] = extended;
	bits[11] = '\0';
	return (bits);
}

void		print_name(t_dpsb *dpsb)
{
	if (S_ISFIFO(dpsb->sb->st_mode))
		ring_buffer_op(3, "\e[33m", dpsb->name, "\e[0m\n");
	else if (S_ISBLK(dpsb->sb->st_mode))
		ring_buffer_op(3, "\e[34m\e[46m", dpsb->name, "\e[0m\n");
	else if (S_ISCHR(dpsb->sb->st_mode))
		ring_buffer_op(3, "\e[34m\e[43m", dpsb->name, "\e[0m\n");
	else if (S_ISLNK(dpsb->sb->st_mode))
	{
		ring_buffer_op(3, "\e[35m", dpsb->name, "\e[0m\n");
	}
	else if (S_ISDIR(dpsb->sb->st_mode))
		ring_buffer_op(3, "\e[36m\e[1m", dpsb->name, "\e[0m\n");
	else if (S_IEXEC & dpsb->sb->st_mode)
		ring_buffer_op(3, "\e[31m", dpsb->name, "\e[0m\n");
	else
		ring_buffer_op(2, dpsb->name, "\n");
}

char		*get_sym_name(char **sym_name, char *path)
{
	char	*name;
	char	*temp;

	name = ft_strnew(500);
	ft_strcpy(name, *sym_name);
	ft_strdel(sym_name);
	ft_strcat(name, " -> ");
	temp = ft_strnew(300);
	readlink(path, temp, 50);
	ft_strcat(name, temp);
	free(temp);
	return (name);
}

char		*handle_time_format(time_t tmp)
{
	char	*str;
	char	*ret;
	int		dst;
	int		index;
	double	timediff;

	index = 0;
	dst = 0;
	str = ctime(&tmp);
	str += 4;
	timediff = time(NULL) - tmp;
	ret = ft_strnew(40);
	ft_strncpy(ret, str, 7);
	if ((timediff > 15778800) || (timediff < -15778800))
		ft_strncpy(ret + 7, ctime(&tmp) + 20, 4);
	else
		ft_strncpy(ret + 7, ctime(&tmp) + 10, 6);
	return (ret);
}
