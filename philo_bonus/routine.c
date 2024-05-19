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
	sem_wait(philo->sem_state);
	state = philo->sem_state;
	sem_post(philo->sem_state);
	return (state == PHIL_DEAD || state == PHIL_FULL);
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
	if (!take_fork(philo, ctx))
		return (0);
	if (!take_fork(philo, ctx))
		return (0);
	precise_sleep(ctx->tt_eat);
	if (is_dead(philo, ctx))
		return (0);
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
}

void	repeat_routine(t_philo *philo, t_context *ctx)
{
	if (is_dead(philo))
		return ;
	sem_wait(ctx->sem_print);
	printf("%zu %d is thinking\n", get_timestamp(&ctx->tv), philo->id);
	sem_post(ctx->sem_print);
	if (!eat(philo, ctx))
		return ;
	if (!sleep(philo, ctx))
		return ;
	repeat_routine(philo, ctx);
}
