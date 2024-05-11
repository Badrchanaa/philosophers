/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:19:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/10 20:15:22 by bchanaa          ###   ########.fr       */
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
		ctx->max_meals = ft_atoi(av[5]);
	else
		ctx->max_meals = -1;
	if (!valid_config(ctx))
		return (0);
	ctx->forks = malloc(sizeof(int) * (ctx->philo_count));
	if (!ctx->forks)
		return (0);
	ctx->mutexes = malloc(sizeof(pthread_mutex_t) * (ctx->philo_count));
	if (!ctx->mutexes)
		return (free(ctx->forks), 0);
	return (1);
}

int	init_mutexes(pthread_mutex_t *mutexes, int size)
{
	int	i;

	if (!mutexes)
		return (0);
	i = 0;
	while (i < size)
	{
		if (pthread_mutex_init(mutexes + i, NULL))
			break;
		i++;
	}
	if (i < size)
	{
		destroy_mutexes(mutexes, i);
		return (0);
	}
	return (1);
}

void	destroy_mutexes(pthread_mutex_t *mutexes, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(mutexes + i);
		i++;
	}
}
