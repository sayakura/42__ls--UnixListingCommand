/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 16:09:35 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 16:09:36 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ring_buffer_op(int ac, ...)
{
	size_t		len;
	int			i;
	va_list		av;
	char		*temp;

	va_start(av, ac);
	i = 0;
	while (i < ac)
	{
		temp = va_arg(av, char *);
		len = ft_strlen(temp);
		if (len + g_ring_buffer_pointer >= BUFFER_SIZE - 1)
		{
			write(1, g_ring_buffer, g_ring_buffer_pointer);
			g_ring_buffer_pointer = 0;
		}
		ft_strcpy(g_ring_buffer + g_ring_buffer_pointer, temp);
		g_ring_buffer_pointer += len;
		i++;
	}
}

void	dpsb_print_l(t_dpsb *dpsb, t_flags *flag_info)
{
	struct passwd	*pwd;
	struct group	*grp;
	char			*temp;

	ring_buffer_op(2, lsperms(dpsb->sb->st_mode, dpsb->extended), " ");
	ring_buffer_op(2, ft_itoa(dpsb->sb->st_nlink), " ");
	if (flag_info->g != 1 && (pwd = getpwuid(dpsb->sb->st_uid)) != NULL)
		ring_buffer_op(2, pwd->pw_name, " ");
	if ((grp = getgrgid(dpsb->sb->st_gid)) != NULL)
		ring_buffer_op(2, grp->gr_name, " ");
	if (!S_ISCHR(dpsb->sb->st_mode) && !S_ISBLK(dpsb->sb->st_mode))
		ring_buffer_op(2, ft_itoa(dpsb->sb->st_size), " ");
	else
		ring_buffer_op(4, ft_itoa(MAJOR(dpsb->sb->st_rdev)), ", ", \
		ft_itoa(MINOR(dpsb->sb->st_rdev)), " ");
	ring_buffer_op(2, (temp = handle_time_format(dpsb->sb->st_mtime)), " ");
	free(temp);
	print_name(dpsb);
}

void	dpsb_print2(t_array_list *al, t_flags *flag_info,\
blkcnt_t total, int hiddens)
{
	int			i;
	char		*d_name;

	if (flag_info->l == 1 && al->len - hiddens > 0)
		ring_buffer_op(3, "total ", ft_itoa(total), "\n");
	i = 0;
	while (i < al->len)
	{
		d_name = ((t_dpsb *)(al->arr[i]))->name;
		if (flag_info->a == 0 && d_name[0] == '.' && ++i)
			continue ;
		else if (flag_info->l == 0)
			print_name(al->arr[i]);
		else
			dpsb_print_l((t_dpsb *)(al->arr[i]), flag_info);
		i++;
	}
}

void	dpsb_print(t_array_list *al, t_flags *flag_info)
{
	int			i;
	blkcnt_t	total;
	int			hiddens;

	hiddens = 0;
	i = 0;
	total = 0;
	while (flag_info->l == 1 && i < al->len)
	{
		if (((t_dpsb *)(al->arr[i]))->name[0] == '.')
		{
			++hiddens;
			if (flag_info->a == 0 && ++i)
				continue ;
		}
		total += ((t_dpsb *)(al->arr[i]))->sb->st_blocks;
		i++;
	}
	dpsb_print2(al, flag_info, total, hiddens);
}
