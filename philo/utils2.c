/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 13:44:54 by bchanaa           #+#    #+#             */
/*   Updated: 2024/08/20 13:44:55 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	valid_config(t_context *ctx)
{
	return (ctx->philo_count > 0 && ctx->tt_die > 0 \
			&& ctx->tt_eat > 0 && ctx->tt_sleep > 0);
}

int	valid_number(char *str)
{
	int	i;
	int	sign;

	while (*str == 32)
		str++;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	if (*str == 0)
		return (0);
	i = 0;
	while (*str == '0')
		str++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (0);
	}
	if (ft_strlen(str) != 10)
		return (ft_strlen(str) < 10);
	if (sign == -1 && ft_strncmp(str, "2147483648", 10) > 0)
		return (0);
	return (sign == -1 || ft_strncmp(str, "2147483647", 10) <= 0);
}

int	valid_arguments(int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (0);
	i = 1;
	while (i < ac)
	{
		if (!valid_number(av[i]))
			return (0);
		i++;
	}
	return (1);
}
