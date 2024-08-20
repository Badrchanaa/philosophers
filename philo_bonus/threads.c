/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 09:53:37 by bchanaa           #+#    #+#             */
/*   Updated: 2024/08/20 09:53:38 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_state	get_state(t_context *ctx)
{
	t_state	state;

	sem_wait(ctx->sem_main_state);
	state = ctx->state;
	sem_post(ctx->sem_main_state);
	return (state);
}

void	set_state(t_context *ctx, t_state state)
{
	sem_wait(ctx->sem_main_state);
	ctx->state = state;
	sem_post(ctx->sem_main_state);
}

void	*monitor_meals(void *p_context)
{
	t_context	*ctx;
	int			full_count;

	ctx = p_context;
	while (get_state(ctx) == UNKNOWN)
		usleep(1000);
	if (get_state(ctx) != ALIVE)
		return (NULL);
	full_count = 0;
	while (full_count < ctx->philo_count)
	{
		sem_wait(ctx->sem_meal);
		if (get_state(ctx) != ALIVE)
			return (NULL);
		full_count++;
	}
	set_state(ctx, FULL);
	return (NULL);
}

void	*monitor_death(void *p_context)
{
	t_context	*ctx;

	ctx = p_context;
	while (get_state(ctx) == UNKNOWN)
		usleep(1000);
	if (get_state(ctx) != ALIVE)
		return (NULL);
	sem_wait(ctx->sem_kill);
	if (get_state(ctx) != ALIVE)
		return (NULL);
	kill_all(ctx->pids, ctx->philo_count);
	set_state(ctx, DEAD);
	return (NULL);
}
