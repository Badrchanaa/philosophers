/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 22:12:59 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 22:13:00 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	is_dead(t_philo *philo, t_context *ctx)
{
	int	state;

	sem_wait(ctx->sem_state);
	state = philo->state;
	sem_post(ctx->sem_state);
	return (state == PHIL_DEAD || state == PHIL_FULL);
}

void	get_philo_state(t_philo *philo, t_state *state)
{
	state->meal_count = philo->meal_count;
	state->state = philo->state;
	state->last_meal = philo->last_meal;
}

int	take_fork(t_philo *philo, t_context *ctx)
{
	if (is_dead(philo, ctx))
		return (0);
	sem_wait(ctx->sem_forks);
	if (is_dead(philo, ctx))
		return (0);
	sem_wait(ctx->sem_print);
	if (is_dead(philo, ctx))
		return (0);
	printf("%zu %d has taken a fork.\n", get_timestamp(&ctx->tv), philo->id);
	sem_post(ctx->sem_print);
	return (1);
}

int	p_eat(t_philo *philo, t_context *ctx)
{
	sem_wait(ctx->sem_waiter);
	if (!take_fork(philo, ctx))
		return (0);
	if (ctx->philo_count < 2)
		return (sem_post(ctx->sem_forks), 0);
	if (!take_fork(philo, ctx))
		return (0);
	sem_post(ctx->sem_waiter);
	sem_wait(ctx->sem_print);
	printf("%zu %d is eating.\n", get_timestamp(&ctx->tv), philo->id);
	sem_post(ctx->sem_print);
	precise_sleep(ctx->tt_eat);
	if (is_dead(philo, ctx))
		return (0);
	sem_wait(ctx->sem_state);
	philo->meal_count++;
	if (philo->meal_count == ctx->max_meals)
		philo->state = PHIL_FULL;
	philo->last_meal = get_current_time();
	sem_post(ctx->sem_state);
	sem_post(ctx->sem_forks);
	sem_post(ctx->sem_forks);
	return (1);
}

int	p_sleep(t_philo *philo, t_context *ctx)
{
	if (is_dead(philo, ctx))
		return (0);
	sem_wait(ctx->sem_print);
	if (is_dead(philo, ctx))
		return (0);
	printf("%zu %d is sleeping.\n", get_timestamp(&ctx->tv), philo->id);
	sem_post(ctx->sem_print);
	precise_sleep(ctx->tt_sleep);
	return (1);
}

void	repeat_routine(t_philo *philo, t_context *ctx)
{
	if (is_dead(philo, ctx))
		return ;
	sem_wait(ctx->sem_print);
	printf("%zu %d is thinking\n", get_timestamp(&ctx->tv), philo->id);
	sem_post(ctx->sem_print);
	if (!p_eat(philo, ctx))
		return ;
	if (!p_sleep(philo, ctx))
		return ;
	repeat_routine(philo, ctx);
}
