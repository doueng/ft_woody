/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_num_to_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dengstra <dengstra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/07 18:06:19 by dengstra          #+#    #+#             */
/*   Updated: 2017/09/07 18:07:52 by dengstra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*num_to_string(int num)
{
	int		n;
	int		len;
	char	*res;

	n = num;
	len = 1;
	while (n /= 10)
		len++;
	res = ft_strnew(len);
	while (len--)
	{
		res[len] = (num % 10) + '0';
		num /= 10;
	}
	return (res);
}
