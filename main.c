/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:11:01 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/13 21:34:54 by bchanaa          ###   ########.fr       */
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

void	monitor(t_context *ctx, t_philo *philos)
{
	t_philo	*philo;
	int 	i;

	while (ctx->all_alive)
	{
		i = 0;
		while (i < ctx->philo_count)
		{
			philo = philos + i;
			if (!philo->is_dead && philo_starved(philo))
			{
				printf("%zu philo N%d is dead.\n", get_timestamp(&ctx->tv), i + 1);
				memset(&philo->is_dead, 255, sizeof(int));
				memset(&ctx->all_alive, 0, sizeof(int));
				break;
			}
			i++;
		}
	}
}

int	main(int ac, char *av[])
{
	t_context	*ctx;
	t_philo		*philos;

	if (ac < 5 || ac > 6)
		return (1);
	ctx = malloc(sizeof(t_context));
	if (!ctx)
		return (1);
	if (!init_context(ac, av, ctx))
		return (1);
	if (!init_forks(ctx->forks, ctx->philo_count))
		return (1);
	philos = start_simulation(ctx);
	if (!philos)
		return (1);
	monitor(ctx, philos);
	destroy_forks(ctx->forks, ctx->philo_count);
	free(ctx);
	printf("Out of main\n");
	return (0);
}
