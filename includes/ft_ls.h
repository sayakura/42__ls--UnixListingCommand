/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 18:01:38 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 18:01:39 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "../libft/libft.h"
# include "array_list.h"
# include "stdio.h"
# include <dirent.h>
# include <errno.h>
# include <stdio.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <stdarg.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/xattr.h>
# include <sys/acl.h>

# define MINORBITS			20
# define MINORMASK			((1U << MINORBITS) - 1)
# define MAJOR(dev)			((unsigned int) ((dev) >> (MINORBITS + 4)))
# define MINOR(dev)			((unsigned int) ((dev) & MINORMASK))

# define BUFFER_SIZE 424242
# define SWAP(t,a,b) ( (t) = (a), (a) = (b), (b) = (t) )

enum				e_mark{
	File,
	Dir
};

typedef struct		s_flags{
	char	l: 2;
	char	re: 2;
	char	a: 2;
	char	r: 2;
	char	t: 2;
	char	g: 2;
	char	f: 2;
	char	u: 2;
	char	d: 2;
	char	header: 2;
}					t_flags;

typedef struct		s_dpsb{
	struct stat		*sb;
	char			extended;
	char			*name;
}					t_dpsb;

typedef struct		s_buffer
{
	char			*buffer;
	int				len;
}					t_buffer;

typedef int	(*t_cmper)(t_flags *, void *, void *);

typedef struct		s_qs{
	t_flags		*flag;
	t_cmper		cmp;
}					t_qs;

char				g_ring_buffer[BUFFER_SIZE];
int					g_ring_buffer_pointer;
void				dpsb_destroyer(void **pointer);
int					dpsb_t_cmper(t_flags *flag_info, void *data, void *data2);
void				dpsb_sort(t_array_list *al, t_flags *flag_info, int (*cmp)
(t_flags *, void *, void *));
void				dpsb_print(t_array_list *al, t_flags *flag_info);
char				*lsperms(int mode, char extended);
void				ring_buffer_op(int ac, ...);
char				*get_sym_name(char **sym_name, char *path);
void				print_name(t_dpsb *dpsb);
void				free_name_only(void *pointer);
int					ft_flag_bonus(char flag, t_flags *flag_info);
void				ft_flag_dispatcher(const char *av, t_flags *flag_info);
char				*handle_time_format(time_t tmp);
int					ft_save_flags_info(int ac, char ***av, t_flags *flag_info);
void				*display_error_message(char *path);
char				*replace_tilde_with_path(char *path);
void				acl_handler(t_dpsb *dpsb);
char				*get_path(char *path, char *filename);
struct stat			*get_stat(char *path, char *filename);
void				handle_arg_file(t_dpsb **dpsb, char *path, char **av,
t_array_list *file_dir[2]);
t_array_list		*fucking_norm(t_array_list *al, char *path);
#endif
