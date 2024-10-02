/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:11:01 by bchanaa           #+#    #+#             */
/*   Updated: 2024/10/02 14:36:15 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*start_simulation(t_context *ctx)
{
	int		i;
	t_philo	*philos;
	int		err;

	philos = malloc(sizeof(t_philo) * (ctx->philo_count));
	if (!philos)
		return (printf("malloc err\n"), NULL);
	i = 0;
	gettimeofday(&ctx->tv, NULL);
	err = init_philos(ctx, philos);
	if (err)
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
	if (!init_context(ac, av, &ctx) || !init_forks(ctx.forks, ctx.philo_count))
		return (printf("philo: something went wrong\n"), 1);
	pthread_mutex_lock(&ctx.kill_lock);
	philos = start_simulation(&ctx);
	if (!philos)
		return (printf("philo: could not spawn philosohpers\n"), 1);
	pthread_mutex_unlock(&ctx.kill_lock);
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
