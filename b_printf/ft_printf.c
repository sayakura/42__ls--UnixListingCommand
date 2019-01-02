/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_headers.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 23:42:10 by qpeng             #+#    #+#             */
/*   Updated: 2018/09/24 18:23:29 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

const char *g_ubase = "0123456789ABCDEF";
const char *g_lbase = "0123456789abcdef";

size_t	ft_strlen(const char *str)
{
	size_t	index;

	index = 0;
	while (str[index] != '\0')
		index++;
	return (index);
}

void	ft_putbuffer(char *buffer, int c, char const *str)
{
	size_t	index;
	size_t	len;

	len = 0;
	index = 0;
	while (buffer[len] != '\0')
		len++;
	if (c != -1)
		buffer[len++] = (unsigned char)c;
	else
	{
		if (str == NULL)
		{
			ft_putbuffer(buffer, -1, "(null)");
			return ;
		}
		while (str[index] != '\0')
		{
			buffer[len] = str[index];
			len++;
			index++;
		}
	}
	buffer[len] = '\0';
}

char	*ft_lltoa(long long num, int base, signed char isupper)
{
	char		*res;
	size_t		size;
	char		neg;
	long long	temp;

	neg = (base == 10 && num < 0) ? 1 : 0;
	size = (num < 0) ? 3 : 2;
	temp = num;
	while ((temp /= base) != 0)
		size++;
	if (!(res = (char *)malloc(sizeof(char) * size)))
		return (NULL);
	res[--size] = '\0';
	res[--size] = (isupper == 1) ? g_ubase[ABS(num % base)] :\
									g_lbase[ABS(num % base)];
	while (num /= base)
		res[--size] = (isupper == 1) ? g_ubase[ABS(num % base)] :\
									g_lbase[ABS(num % base)];
	if (base == 10 && neg == 1)
		res[--size] = '-';
	return (res);
}

void	ft_putargs(unsigned char sym, va_list args, char *buffer)
{
	if (sym == 's')
		ft_putbuffer(buffer, -1, (va_arg(args, const char*)));
	else if (sym == 'c')
		ft_putbuffer(buffer, va_arg(args, int), 0);
	else if (sym == 'd')
		ft_putbuffer(buffer, -1, ft_lltoa(va_arg(args, int), 10, 0));
	else if (sym == 'i')
		ft_putbuffer(buffer, -1, ft_lltoa(va_arg(args, int), 10, 0));
	else if (sym == 'p')
	{
		ft_putbuffer(buffer, -1, "0x");
		ft_putbuffer(buffer, -1, ft_lltoa(va_arg(args, unsigned\
		long), 16, 0));
	}
	else if (sym == 'o')
		ft_putbuffer(buffer, -1, ft_lltoa(va_arg(args, unsigned int), 8, 0));
	else if (sym == 'u')
		ft_putbuffer(buffer, -1, ft_lltoa(va_arg(args, unsigned int), 10, 0));
	else if (sym == 'x')
		ft_putbuffer(buffer, -1, ft_lltoa(va_arg(args, unsigned int), 16, 0));
	else if (sym == '%')
		ft_putbuffer(buffer, '%', 0);
}

int		b_printf(const char *restrict format, ...)
{
	va_list	args;
	size_t	index;
	char	buffer[1000000];

	index = 0;
	va_start(args, format);
	while (index < 1000000)
		buffer[index++] = '\0';
	index = -1;
	while (format[++index] != '\0')
	{
		if (format[index] != '%')
			ft_putbuffer(buffer, format[index], 0);
		else
		{
			while (format[++index] == ' ')
				;
			if (format[index] == '\0')
				break ;
			else
				ft_putargs(format[index], args, buffer);
		}
	}
	va_end(args);
	return (write(1, buffer, ft_strlen(buffer)));
}
