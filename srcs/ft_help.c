/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_help.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 16:11:01 by qpeng             #+#    #+#             */
/*   Updated: 2019/01/02 16:11:03 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	dpsb_destroyer(void **pointer)
{
	t_dpsb	*dpsb;

	dpsb = (t_dpsb *)(*pointer);
	if (dpsb->name)
		free(dpsb->name);
	if (dpsb->sb)
		free(dpsb->sb);
	if (*pointer)
		free(*pointer);
}

int		dpsb_t_cmper(t_flags *flag_info, void *data, void *data2)
{
	t_dpsb	*dpsb;
	t_dpsb	*dpsb2;
	int		ret;

	dpsb = (t_dpsb *)data;
	dpsb2 = (t_dpsb *)data2;
	if (flag_info->t)
	{
		if (flag_info->u == 1)
			ret = (dpsb->sb)->st_atimespec.tv_sec - \
							(dpsb2->sb)->st_atimespec.tv_sec;
		else
			ret = (dpsb->sb)->st_mtimespec.tv_sec - \
							(dpsb2->sb)->st_mtimespec.tv_sec;
		if (ret != 0)
			return ((flag_info->r) ? ret : -ret);
	}
	ret = (ft_strcmp(dpsb->name, dpsb2->name));
	return ((flag_info->r) ? -ret : ret);
}

void	dpsb_sort_g(t_dpsb **arr, int len, char *target, int to)
{
	void	*temp;
	int		i;
	int		index;

	i = -1;
	while (++i < len)
		if (ft_strcmp(arr[i]->name, target) == 0)
			index = i;
	temp = arr[index];
	while (index > to)
	{
		arr[index] = arr[index - 1];
		index--;
	}
	arr[to] = temp;
}

void	quicksort(void **arr, int first, int last, t_qs *s)
{
	int		i;
	int		j;
	int		pivot;
	void	*temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;
		while (i < j)
		{
			while (s->cmp(s->flag, arr[i], arr[pivot]) <= 0 && i < last)
				i++;
			while (s->cmp(s->flag, arr[j], arr[pivot]) > 0)
				j--;
			if (i < j)
				SWAP(temp, arr[i], arr[j]);
		}
		SWAP(temp, arr[pivot], arr[j]);
		quicksort(arr, first, j - 1, s);
		quicksort(arr, j + 1, last, s);
	}
}

void	dpsb_sort(t_array_list *al, t_flags *flag_info, t_cmper cmp)
{
	int		i;
	t_qs	qs;

	qs = (t_qs){.flag = flag_info, .cmp = cmp};
	i = 0;
	if (flag_info->f == 1)
	{
		dpsb_sort_g((t_dpsb **)al->arr, al->len, ".", 0);
		dpsb_sort_g((t_dpsb **)al->arr, al->len, "..", 1);
		return ;
	}
	if (al)
		quicksort(al->arr, 0, al->len - 1, &qs);
}
