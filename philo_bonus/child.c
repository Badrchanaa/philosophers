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

void	get_philo_state(t_philo *philo, t_state *state)
{
	state->meal_count = philo->meal_count;
	state->state = philo->state;
	state->last_meal = philo->last_meal;
}

void	*philosopher(void *p_philo)
{
	t_philo	*philo;

	philo = p_philo;
	repeat_routine(philo, philo->ctx);
	return (NULL);
}

void	*monitor(void *p_philo)
{
	t_state	state;
	t_philo	*philo;
	t_context	*ctx;

	philo = p_philo;
	ctx = philo->ctx;
	while (1)
	{
		sem_wait(ctx->sem_state);
		get_philo_state(philo, &state);
		if (ctx->max_meals > 0 && state.meal_count >= ctx->max_meals)
			return (sem_post(ctx->sem_state), NULL);
		if (get_current_time() - state.last_meal > (size_t) ctx->tt_die)
		{
			sem_wait(ctx->sem_print);
			printf("%zu %d died.\n", get_timestamp(&ctx->tv), philo->id);
			sem_post(ctx->sem_print);
			sem_post(ctx->sem_forks);
			philo->state = PHIL_DEAD;
			sem_post(ctx->sem_state);
			//close_semaphores(ctx);
			//exit(1);
			return (NULL);
		}
		sem_post(ctx->sem_state);
		usleep(500);
	}
	return (NULL);
}

int	child_main(t_context *ctx, int id)
{
	t_philo	philo;

	//int		status;
	philo.meal_count = 0;
	philo.id = id;
	philo.last_meal = get_current_time();
	philo.ctx = ctx;
	philo.state = PHIL_ALIVE;
	if (pthread_create(&philo.thread, NULL, philosopher, &philo))
		return (1);
	//if (pthread_detach(philo.thread))
		//printf("oups\n");
	monitor(&philo);
	pthread_join(philo.thread, NULL);
	close_semaphores(ctx);
	printf("im out\n");
	//status = monitor(ctx, &philo);
	return (philo.state == PHIL_DEAD);
}
