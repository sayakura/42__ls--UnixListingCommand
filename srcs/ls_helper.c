/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 16:46:07 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 16:46:08 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			handle_arg_file(t_dpsb **dpsb, char *path, char **av,
t_array_list *file_dir[2])
{
	struct stat	sb;

	path = replace_tilde_with_path(*av);
	if (lstat(path, &sb) != -1)
	{
		if (!S_ISDIR(sb.st_mode))
		{
			*dpsb = ft_memalloc(sizeof(t_dpsb));
			(*dpsb)->name = path;
			(*dpsb)->sb = get_stat(path, NULL);
			acl_handler(*dpsb);
			if (S_ISLNK(sb.st_mode))
				(*dpsb)->name = get_sym_name(&path, *av);
			ARRAY_LIST(push)(file_dir[File], *dpsb);
		}
		else
			ARRAY_LIST(push)(file_dir[Dir], path);
	}
	else
		display_error_message(path);
}

t_array_list	*fucking_norm(t_array_list *al, char *path)
{
	display_error_message((char *)path);
	return (al);
}
