/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:11:01 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/14 18:50:30 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


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
	int 	i;

	while (!ctx->kill_all)
	{
		i = 0;
		while (i < ctx->philo_count)
		{
			philo = philos + i;
			if (!philo->is_dead && philo_starved(philo))
			{
				printf("%zu philo N%d is dead.\n", get_timestamp(&ctx->tv), i + 1);
				memset(&philo->is_dead, 255, sizeof(int));
				memset(&ctx->kill_all, 255, sizeof(int));
				break;
			}
			i++;
		}
		usleep(1000);
	}
}

int	main(int ac, char *av[])
{
	t_context	ctx;
	t_philo		*philos;

	if (ac < 5 || ac > 6)
		return (1);
	if (!init_context(ac, av, &ctx))
		return (1);
	if (!init_forks(ctx.forks, ctx.philo_count))
		return (1);
	philos = start_simulation(&ctx);
	if (!philos)
		return (1);
	monitor(&ctx, philos);
	join_all(philos, ctx.philo_count);
	printf("Out of main\n");
	destroy_forks(ctx.forks, ctx.philo_count);
	destroy_philosophers(philos, ctx.philo_count);
	free(philos);
	return (0);
}
