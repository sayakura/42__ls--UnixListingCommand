/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 17:52:41 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 17:52:42 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_array_list	*ft_save_dir(char *path)
{
	DIR				*dir;
	struct dirent	*dp;
	t_array_list	*al;
	t_dpsb			*dpsb;
	char			*temp;

	ARRAY_LIST(init)(&al);
	if ((dir = opendir(path)) == NULL)
		return (fucking_norm(al, path));
	while ((dp = readdir(dir)) != NULL)
	{
		dpsb = malloc(sizeof(t_dpsb));
		dpsb->sb = get_stat(path, dp->d_name);
		dpsb->name = ft_strdup(dp->d_name);
		acl_handler(dpsb);
		if (S_ISLNK(dpsb->sb->st_mode))
		{
			temp = get_path(path, dpsb->name);
			dpsb->name = get_sym_name(&dpsb->name, temp);
			free(temp);
		}
		ARRAY_LIST(push)(al, dpsb);
	}
	closedir(dir);
	return (al);
}

void			ft_recursive(t_flags *flag_info, char *path)
{
	t_array_list	*al;
	int				i;
	char			*full_path;

	i = 0;
	if (flag_info->header == 1)
		ring_buffer_op(2, path, ":\n");
	flag_info->header = 1;
	al = ft_save_dir(path);
	dpsb_sort(al, flag_info, &dpsb_t_cmper);
	dpsb_print(al, flag_info);
	while (i < al->len && flag_info->re == 1)
	{
		if (((t_dpsb *)(al->arr[i]))->name[0] != '.'
		&& S_ISDIR((((t_dpsb *)(al->arr[i]))->sb)->st_mode))
		{
			full_path = get_path(path, ((t_dpsb *)(al->arr[i]))->name);
			ring_buffer_op(1, "\n");
			ft_recursive(flag_info, full_path);
			free(full_path);
		}
		i++;
	}
	ARRAY_LIST(destroy)(&al, &dpsb_destroyer);
}

void			recursive_dirs(t_array_list *dirs, t_flags *flag_info)
{
	int				i;
	t_dpsb			*dpsb;
	t_array_list	*dir_sorted;

	i = -1;
	ARRAY_LIST(init)(&dir_sorted);
	while (++i < dirs->len)
	{
		dpsb = ft_memalloc(sizeof(t_dpsb));
		dpsb->name = ft_strdup(dirs->arr[i]);
		dpsb->sb = get_stat(dpsb->name, NULL);
		acl_handler(dpsb);
		ARRAY_LIST(push)(dir_sorted, dpsb);
	}
	dpsb_sort(dir_sorted, flag_info, &dpsb_t_cmper);
	i = 0;
	while (i < dir_sorted->len)
	{
		ft_recursive(flag_info, ((t_dpsb *)dir_sorted->arr[i])->name);
		i++;
		if (i < dir_sorted->len)
			ring_buffer_op(1, "\n");
	}
	ARRAY_LIST(destroy)(&dirs, &ft_memdel);
	ARRAY_LIST(destroy)(&dir_sorted, &dpsb_destroyer);
}

void			ls_av_op(t_flags *flag_info, char **av)
{
	char			*path;
	t_dpsb			*dpsb;
	t_array_list	*file_dir[2];

	path = NULL;
	ARRAY_LIST(init)(&file_dir[File]);
	ARRAY_LIST(init)(&file_dir[Dir]);
	while (*av)
	{
		handle_arg_file(&dpsb, path, av, file_dir);
		av++;
	}
	if (file_dir[File]->len > File)
	{
		dpsb_sort(file_dir[File], flag_info, &dpsb_t_cmper);
		dpsb_print(file_dir[File], flag_info);
		if (file_dir[Dir]->len > 0)
			ring_buffer_op(1, "\n");
		flag_info->header = 1;
	}
	ARRAY_LIST(destroy)(&file_dir[File], &dpsb_destroyer);
	if (file_dir[Dir]->len > 0)
		recursive_dirs(file_dir[Dir], flag_info);
	else
		ARRAY_LIST(destroy)(&file_dir[Dir], &ft_memdel);
}

int				main(int ac, char **av)
{
	t_flags			flag_info;
	int				status;
	t_array_list	*al;

	al = NULL;
	ft_bzero(&flag_info, sizeof(t_flags));
	if ((status = ft_save_flags_info(ac, &av, &flag_info)) == 0)
		exit(EXIT_FAILURE);
	else if (*av == NULL)
		ft_recursive(&flag_info, ".");
	else
		ls_av_op(&flag_info, av);
	write(1, g_ring_buffer, g_ring_buffer_pointer);
	exit(EXIT_SUCCESS);
}
