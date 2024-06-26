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

void	release_forks(t_philo *philo, int release_left, int release_right)
{
	int	left_id;
	int	right_id;
	
	left_id = philo->id;
	right_id = right_philo(philo->id, philo->ctx->philo_count);
	if (release_left)
		pthread_mutex_unlock(philo->ctx->forks + left_id);
	if (release_right)
		pthread_mutex_unlock(philo->ctx->forks + right_id);
}

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
	{
		printf("%zu philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
		return (1);
	}
	if ((philo->id + 1) % 2 != 0)
	{
		pthread_mutex_lock(philo->ctx->forks + left_id);
		if (!still_alive(philo))
			return (release_forks(philo, 1, 0), 1);
		printf("%zu philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
		pthread_mutex_lock(philo->ctx->forks + right_id);
		if (!still_alive(philo))
			return (release_forks(philo, 1, 1), 1);
		printf("%zu philo N%d took right fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	}
	else
	{
		pthread_mutex_lock(philo->ctx->forks + right_id);
		if (!still_alive(philo))
			return (release_forks(philo, 0, 1), 1);
		printf("%zu philo N%d took right fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
		pthread_mutex_lock(philo->ctx->forks + left_id);
		if (!still_alive(philo))
			return (release_forks(philo, 1, 1), 1);
		printf("%zu philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	}
	return (0);
}

int	philo_eat(t_philo *philo)
{
	int	ms;
	ms = philo->ctx->tt_eat;
	if (hold_forks(philo))
		return (1);
	printf("%zu philo N%d is eating.\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	if (ms > philo->ctx->tt_die)
	{
		precise_sleep(philo->ctx->tt_die);
		return (release_forks(philo, 1, 1), 1);
	}
	precise_sleep(philo->ctx->tt_eat);
	pthread_mutex_lock(&philo->time_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->time_lock);
	release_forks(philo, 1, 1);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (!still_alive(philo))
		return (1);
	printf("%zu philo N%d is sleeping.\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
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

void	*philosopher(void *p_philo)
{
	int		max_meals;
	t_philo	*philo;
	
	philo = p_philo;
	max_meals = philo->ctx->max_meals;
	while (still_alive(philo) && !is_full(philo))
	{
		if ((philo->id + 1) % 2 != 0)
			usleep(500);
		if (philo_eat(philo))
			break ;
		pthread_mutex_lock(&philo->count_lock);
		philo->meal_count++;
		if (philo->ctx->max_meals > 0 && philo->meal_count >= philo->ctx->max_meals)
		{
			philo->meal_count *= -1;
			pthread_mutex_unlock(&philo->count_lock);
			break;
		}
		pthread_mutex_unlock(&philo->count_lock);
		if (philo_sleep(philo))
			break ;
	}
	//printf("---------- N%d is dead: %d | meals: %d -------------\n", philo->id + 1, philo->is_dead, philo->meal_count);
	return (philo->ctx);
}
