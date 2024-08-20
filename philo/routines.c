/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:04:09 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 18:04:11 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	still_alive(t_philo *philo)
{
	t_context	*ctx;
	int			res;

	if (is_dead(philo))
		return (0);
	ctx = philo->ctx;
	pthread_mutex_lock(&ctx->kill_lock);
	res = ctx->kill_all;
	pthread_mutex_unlock(&ctx->kill_lock);
	return (!res);
}

int	hold_forks(t_philo *philo)
{
	int	left_id;		
	int	right_id;		

	if (!still_alive(philo))
		return (1);
	left_id = philo->id;
	right_id = (philo->id + 1) % philo->ctx->philo_count;
	if (left_id == right_id)
		return (print_state(philo, HELD_FORK, philo->id + 1), 1);
	if ((philo->id + 1) % 2 == 0)
	{
		left_id = right_id;
		right_id = philo->id;
	}
	pthread_mutex_lock(philo->ctx->forks + left_id);
	if (!still_alive(philo))
		return (pthread_mutex_unlock(philo->ctx->forks + left_id), 1);
	print_state(philo, HELD_FORK, philo->id + 1);
	pthread_mutex_lock(philo->ctx->forks + right_id);
	if (!still_alive(philo))
		return (release_forks(philo), 1);
	print_state(philo, HELD_FORK, philo->id + 1);
	return (0);
}

int	philo_eat(t_philo *philo)
{
	int	ms;

	ms = philo->ctx->tt_eat;
	if (hold_forks(philo))
		return (1);
	print_state(philo, EATING, philo->id + 1);
	if (ms > philo->ctx->tt_die)
	{
		precise_sleep(philo->ctx->tt_die);
		return (release_forks(philo), 1);
	}
	precise_sleep(philo->ctx->tt_eat);
	pthread_mutex_lock(&philo->time_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->time_lock);
	release_forks(philo);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (!still_alive(philo))
		return (1);
	print_state(philo, SLEEPING, philo->id + 1);
	precise_sleep(philo->ctx->tt_sleep);
	return (0);
}

int	is_full(t_philo *philo)
{
	int	meal_count;

	if (philo->ctx->max_meals == -1)
		return (0);
	pthread_mutex_lock(&philo->count_lock);
	meal_count = philo->meal_count;
	pthread_mutex_unlock(&philo->count_lock);
	return (meal_count >= philo->ctx->max_meals);
}
