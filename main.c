/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:11:01 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/11 17:12:42 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int add_one(int *num)
{
	int				tmp;
	int				i;
	unsigned char	c;
	unsigned char	*tmp_p;

	tmp = *num;
	tmp++;
	tmp_p = (unsigned char *)&tmp;
	i = 0;
	while (i < 4)
	{
		c = tmp_p[i];
		memset(((unsigned char *)num) + i, c, sizeof(char));
		i++;
	}
	return (tmp);
}

int	philo_eat(t_philo *philo)
{
	int		ms;
	int		left_id;
	int		right_id;

	if (philo->is_dead)
		return (1);
	ms = philo->ctx->tt_eat;
	left_id = philo->id;
	right_id = right_philo(philo->id, philo->ctx->philo_count);
	pthread_mutex_lock(philo->ctx->mutexes + left_id);
	printf("%u philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	pthread_mutex_lock(philo->ctx->mutexes + right_id);
	printf("%u philo N%d took right fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	printf("%u philo N%d is eating.\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	precise_sleep(philo->ctx->tt_eat);
	gettimeofday(&philo->last_meal_tv, NULL);
	add_one(&philo->meal_count);
	pthread_mutex_unlock(philo->ctx->mutexes + left_id);
	pthread_mutex_unlock(philo->ctx->mutexes + right_id);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (philo->is_dead)
		return (1);
	printf("%u philo N%d is sleeping.\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	precise_sleep(philo->ctx->tt_sleep);
	return (0);
}

void	*philosopher(void *p_philo)
{
	int		max_meals;
	t_philo	*philo;
	
	philo = p_philo;
	max_meals = philo->ctx->max_meals;
	while (!philo->is_dead && (max_meals == -1 || (philo->meal_count < max_meals)))
	{
		if (philo_eat(philo))
			return (philo->ctx);
		if (philo_sleep(philo))
			return (philo->ctx);
		printf("%u philo N%d is thinking.\n", get_timestamp(&philo->ctx->tv), philo->id);
	}

	return (philo->ctx);
}

void	new_philo(t_context *ctx, t_philo *philo_arr, int id)
{
	t_philo	*philo;

	philo = philo_arr + id;
	philo->id = id;
	philo->is_dead = 0;
	philo->ctx = ctx;
	philo->meal_count = 0;
	gettimeofday(&philo->last_meal_tv, NULL);
	pthread_create(&philo->thread, NULL, philosopher, philo);
	pthread_detach(philo->thread);
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
	while (i < ctx->philo_count)
	{
		new_philo(ctx, philos, i);
		i++;
	}
	return (philos);
}

int	main(int ac, char *av[])
{
	t_context	*ctx;
	t_philo		*philos;

	ctx = malloc(sizeof(t_context));
	if (!ctx)
		return (1);
	if (ac < 5 || ac > 6)
		return (1);
	if (!init_context(ac, av, ctx))
		return (1);
	else
	{
		printf("CONFIG:\n");
		printf("	philo count: %d\n", ctx->philo_count);
		printf("	time to die: %d\n", ctx->tt_die);
		printf("	time to eat: %d\n", ctx->tt_eat);
		printf("	time to sleep: %d\n", ctx->tt_sleep);
		printf("	max meals: %d\n", ctx->max_meals);
	}
	if (!init_mutexes(ctx->mutexes, ctx->philo_count))
		return (printf("init mutex failed\n"), 1);
	philos = start_simulation(ctx);
	int i;
	int	all_alive = 1;
	while (all_alive)
	{
		i = 0;
		while (i < ctx->philo_count)
		{
			t_philo	*philo = philos + i;
			if (!philo->is_dead && philo_starved(philo))
			{
				printf("%u philo N%d is dead.\n", get_timestamp(&ctx->tv), i + 1);
				//philo->is_dead = PHILO_DEAD;
				memset(&philo->is_dead, 255, sizeof(int));
				all_alive = 0;
				break;
			}
			i++;
		}
	}
	destroy_mutexes(ctx->mutexes, ctx->philo_count);
	free(ctx->forks);
	free(ctx);
	return (0);
}
