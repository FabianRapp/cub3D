/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 23:53:26 by frapp             #+#    #+#             */
/*   Updated: 2024/02/24 21:32:06 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c <= 90 && c >= 65)
		c += 32;
	return (c);
}

void	ft_strtolower(char *str)
{
	if (!str)
		return ;
	while (*str)
	{
		*str = ft_tolower(*str);
		str++;
	}
}
