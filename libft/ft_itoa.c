/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:36 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 19:20:30 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_digit_count(int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	fill_str(char *str, long num, size_t len)
{
	str[len] = '\0';
	len--;
	if (num == 0)
	{
		str[0] = '0';
		return ;
	}
	while (num != 0)
	{
		str[len] = (num % 10) + '0';
		len--;
		num /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;
	long	num;
	int		neg;

	len = get_digit_count(n);
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	num = n;
	neg = n < 0;
	if (neg)
		num = -num;
	fill_str(str, num, len);
	if (neg)
		str[0] = '-';
	return (str);
}
