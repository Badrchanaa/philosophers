/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 10:10:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/07/21 10:10:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	get_philo_state(t_philo *philo, t_philo_state *state)
{
	state->meal_count = philo->meal_count;
	state->state = philo->state;
	state->last_meal = philo->last_meal;
}

void	*philosopher(void *p_philo)
{
	t_philo		*philo;
	t_context	*ctx;

	philo = p_philo;
	ctx = philo->ctx;
	repeat_routine(philo, philo->ctx);
	return (NULL);
}

int	monitor(void *p_philo)
{
	t_philo_state	state;
	t_philo			*philo;
	t_context		*ctx;

	philo = p_philo;
	ctx = philo->ctx;
	while (1)
	{
		sem_wait(ctx->sem_state);
		get_philo_state(philo, &state);
		sem_post(ctx->sem_state);
		if (ctx->max_meals > 0 && state.meal_count >= ctx->max_meals)
			return (sem_post(ctx->sem_meal), 0);
		if (get_current_time() - state.last_meal > (size_t) ctx->tt_die)
		{
			sem_wait(ctx->sem_print);
			printf("%zu %d died.\n", get_timestamp(&ctx->tv), philo->id);
			sem_post(ctx->sem_kill);
			return (1);
		}
		usleep(500);
	}
	return (0);
}

int	child_main(t_context *ctx, int id)
{
	int		status;
	t_philo	philo;

	philo.meal_count = 0;
	philo.id = id;
	philo.last_meal = get_current_time();
	philo.ctx = ctx;
	philo.state = PHIL_ALIVE;
	if (pthread_create(&philo.thread, NULL, philosopher, &philo))
		return (1);
	status = monitor(&philo);
	pthread_join(philo.thread, NULL);
	close_semaphores(ctx);
	return (status);
}
