/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 20:14:17 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/13 21:49:10 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(t_philo *philo)
{
	int	ms;
	int	left_id;		
	int	right_id;		

	if (philo->is_dead)
		return (1);
	ms = philo->ctx->tt_eat;
	left_id = philo->id;
	right_id = right_philo(philo->id, philo->ctx->philo_count);
	pthread_mutex_lock(philo->ctx->forks + left_id);
	printf("%zu philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	pthread_mutex_lock(philo->ctx->forks + right_id);
	printf("%zu philo N%d took right fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	printf("%zu philo N%d is eating.\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	precise_sleep(philo->ctx->tt_eat);
	pthread_mutex_lock(&philo->time_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->time_lock);
	pthread_mutex_unlock(philo->ctx->forks + left_id);
	pthread_mutex_unlock(philo->ctx->forks + right_id);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (philo->is_dead)
		return (1);
	printf("%zu philo N%d is sleeping.\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	precise_sleep(philo->ctx->tt_sleep);
	return (0);
}

void	*philosopher(void *p_philo)
{
	int		max_meals;
	t_philo	*philo;
	
	philo = p_philo;
	max_meals = philo->ctx->max_meals;
	while (philo->ctx->all_alive && !philo->is_dead && (max_meals == -1 || (philo->meal_count < max_meals)))
	{
		if (philo_eat(philo))
			break ;
		philo->meal_count++;
		if (philo->ctx->max_meals > 0 && philo->meal_count >= philo->ctx->max_meals)
		{
			memset(&philo->is_dead, 255, sizeof(int));
			break;
		}
		if (philo_sleep(philo))
			break ;
	}
	printf("---------- N%d is dead: %d | meals: %d -------------\n", philo->id + 1, philo->is_dead, philo->meal_count);
	return (philo->ctx);
}
