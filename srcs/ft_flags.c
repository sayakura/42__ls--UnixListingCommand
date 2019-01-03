/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 17:58:19 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 17:58:20 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_flag_bonus(char flag, t_flags *flag_info)
{
	if (flag == 'g')
		flag_info->g = 1;
	else if (flag == 'f')
	{
		flag_info->f = 1;
		flag_info->a = 1;
	}
	else if (flag == 'u')
		flag_info->u = 1;
	else if (flag == 'd')
		flag_info->d = 1;
	else
		return (0);
	return (1);
}

void	ft_flag_dispatcher(const char *av, t_flags *flag_info)
{
	int		i;

	i = 0;
	while (av[++i] != '\0')
	{
		if (av[i] == 'l')
			flag_info->l = 1;
		else if (av[i] == 'R')
			flag_info->re = 1;
		else if (av[i] == 'a')
			flag_info->a = 1;
		else if (av[i] == 'r')
			flag_info->r = 1;
		else if (av[i] == 't')
			flag_info->t = 1;
		else if (ft_flag_bonus(av[i], flag_info))
			;
		else
		{
			ft_putstr("ls: illegal option -- ");
			ft_putchar(av[i]);
			ft_putstr("\nusage: ls [-lRartgf] [file ...]\n");
			exit(EXIT_FAILURE);
		}
	}
}

int		ft_save_flags_info(int ac, char ***av, t_flags *flag_info)
{
	int	i;
	int	status;

	i = 1;
	status = 1;
	while (i < ac)
	{
		if (ft_strchr((*av)[i], '-') != NULL)
		{
			ft_flag_dispatcher((*av)[i], flag_info);
			status = 2;
		}
		else
			break ;
		i++;
	}
	(*av) += i;
	return (status);
}
