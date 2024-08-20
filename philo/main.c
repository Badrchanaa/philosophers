/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:11:01 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/16 16:31:57 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher(void *p_philo)
{
	int		max_meals;
	t_philo	*philo;

	philo = p_philo;
	max_meals = philo->ctx->max_meals;
	while (still_alive(philo) && !is_full(philo))
	{
		print_state(philo, THINKING, philo->id + 1);
		if ((philo->id + 1) % 2 != 0)
			usleep(500);
		if (philo_eat(philo))
			break ;
		pthread_mutex_lock(&philo->count_lock);
		philo->meal_count++;
		if (philo->ctx->max_meals - philo->meal_count == 0)
		{
			philo->meal_count *= -1;
			pthread_mutex_unlock(&philo->count_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->count_lock);
		if (philo_sleep(philo))
			break ;
	}
	return (philo->ctx);
}

t_philo	*start_simulation(t_context *ctx)
{
	int			i;
	t_philo		*philos;

	philos = malloc(sizeof(t_philo) * (ctx->philo_count));
	if (!philos)
		return (NULL);
	i = 0;
	gettimeofday(&ctx->tv, NULL);
	if (init_philos(ctx, philos))
		return (free(philos), NULL);
	return (philos);
}

int	join_all(t_philo *philos, int size)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < size)
	{
		philo = philos + i;
		pthread_join(philo->thread, NULL);
		i++;
	}
	return (0);
}

void	monitor(t_context *ctx, t_philo *philos)
{
	t_philo	*philo;
	int		i;
	int		dead_count;

	dead_count = 0;
	while (!ctx->kill_all && dead_count < ctx->philo_count)
	{
		i = 0;
		dead_count = 0;
		while (i < ctx->philo_count)
		{
			philo = philos + i;
			if (is_dead(philo))
				dead_count++;
			else if (philo_starved(philo))
			{
				print_state(philo, DEAD, i + 1);
				apocalypse(ctx, philo);
				break ;
			}
			i++;
		}
		usleep(2000);
	}
}

int	main(int ac, char *av[])
{
	t_context	ctx;
	t_philo		*philos;

	if (!valid_arguments(ac, av))
		return (printf("philo: invalid arguments.\n"), 1);
	if (!init_context(ac, av, &ctx))
		return (1);
	if (!init_forks(ctx.forks, ctx.philo_count))
		return (1);
	philos = start_simulation(&ctx);
	if (!philos)
		return (1);
	if (ctx.max_meals != 0)
		monitor(&ctx, philos);
	join_all(philos, ctx.philo_count);
	printf("Out of main\n");
	destroy_forks(ctx.forks, ctx.philo_count);
	destroy_philosophers(philos, ctx.philo_count);
	free(philos);
	free(ctx.forks);
	return (0);
}
