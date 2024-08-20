/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:25:53 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/15 18:35:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	right_philo(int philo_id, int philo_count)
{
	if (philo_id == philo_count - 1)
		return (0);
	return (philo_id + 1);
}

inline int	philo_starved(t_philo *philo)
{
	size_t	current_time;
	size_t	last_meal;
	size_t	diff;

	current_time = get_current_time();
	pthread_mutex_lock(&philo->time_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->time_lock);
	diff = current_time - last_meal;
	return (diff > INT_MAX || (int)diff >= philo->ctx->tt_die);
}

void	print_state(t_philo *philo, t_state state, int id)
{
	t_context	*ctx;
	size_t		ts;

	ctx = philo->ctx;
	pthread_mutex_lock(&ctx->print_lock);
	ts = get_timestamp(&ctx->tv);
	if (state == HELD_FORK)
		printf("%zu %d has taken a fork\n", ts, id);
	else if (state == EATING)
		printf("%zu %d is eating.\n", ts, id);
	else if (state == SLEEPING)
		printf("%zu %d is sleeping.\n", ts, id);
	else if (state == THINKING)
		printf("%zu philo N%d is thinking.\n", ts, id);
	else if (state == DEAD)
		printf("%zu %d died.\n", ts, id);
	pthread_mutex_unlock(&ctx->print_lock);
}

int	is_dead(t_philo *philo)
{
	int	count;

	pthread_mutex_lock(&philo->count_lock);
	count = philo->meal_count;
	pthread_mutex_unlock(&philo->count_lock);
	return (count < 0);
}

void	apocalypse(t_context *ctx, t_philo *philo)
{
	pthread_mutex_lock(&philo->count_lock);
	philo->meal_count *= -1;
	pthread_mutex_unlock(&philo->count_lock);
	pthread_mutex_lock(&ctx->kill_lock);
	ctx->kill_all = 1;
	pthread_mutex_unlock(&ctx->kill_lock);
}
