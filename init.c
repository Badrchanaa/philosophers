/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:19:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/15 19:40:16 by bchanaa          ###   ########.fr       */
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
	ctx->kill_all = 0;
	if (ac == 6)
		ctx->max_meals = ft_atoi(av[5]);
	else
		ctx->max_meals = -1;
	if (!valid_config(ctx))
		return (0);
	ctx->forks = malloc(sizeof(pthread_mutex_t) * (ctx->philo_count));
	if (!ctx->forks)
		return (0);
	if (pthread_mutex_init(&ctx->kill_lock, NULL))
		return (free(ctx->forks), 0);
	return (1);
}

void	destroy_philo(t_philo *philo)
{
	pthread_mutex_destroy(&philo->time_lock);
	pthread_mutex_destroy(&philo->count_lock);
}

int	new_philo(t_context *ctx, t_philo *philo_arr, int id)
{
	t_philo	*philo;

	philo = philo_arr + id;
	if (pthread_mutex_init(&philo->time_lock, NULL))
		return (1);
	if (pthread_mutex_init(&philo->count_lock, NULL))
		return (pthread_mutex_destroy(&philo->time_lock), 1);
	philo->id = id;
	philo->is_dead = 0;
	philo->ctx = ctx;
	philo->meal_count = 0;
	philo->last_meal = get_current_time();
	if (pthread_create(&philo->thread, NULL, philosopher, philo))
		return (destroy_philo(philo), 1);
	return (0);
}

int	init_philos(t_context *ctx, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < ctx->philo_count)
	{
		if (i % 2 == 0)
			usleep(500);
		if (new_philo(ctx, philos, i))
			break;
		i++;
	}
	return (i < ctx->philo_count);
}

int	init_forks(pthread_mutex_t *forks, int size)
{
	int	i;

	if (!forks)
		return (0);
	i = 0;
	while (i < size)
	{
		if (pthread_mutex_init(forks + i, NULL))
			break;
		i++;
	}
	if (i < size)
	{
		destroy_forks(forks, i);
		return (0);
	}
	return (1);
}

void	destroy_forks(pthread_mutex_t *forks, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(forks + i);
		i++;
	}
}

void	destroy_philosophers(t_philo *philos, int size)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < size)
	{
		philo = philos + i;
		destroy_philo(philo);
		i++;
	}
}
