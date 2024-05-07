/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:19:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/07 21:43:59 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	valid_config(t_context *ctx)
{
	return (ctx->philo_count > 0 && ctx->tt_die > 0
			&& ctx->tt_eat >= 0 && ctx->tt_sleep >= 0);
}

int	init_context(int ac, char **av, t_context *ctx)
{
	ctx->philo_count = ft_atoi(av[1]);
	ctx->tt_die = ft_atoi(av[2]);
	ctx->tt_eat = ft_atoi(av[3]);
	ctx->tt_sleep =	ft_atoi(av[4]);
	if (ac == 6)
		ctx->max_eat = ft_atoi(av[5]);
	else
		ctx->max_eat = -1;
	return (valid_config(ctx));
}
