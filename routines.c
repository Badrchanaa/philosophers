/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */ /*   routines.c                                         :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 20:14:17 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/14 17:13:44 by bchanaa          ###   ########.fr       */
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

int	hold_forks(t_philo *philo)
{
	int	left_id;		
	int	right_id;		

	if (philo->is_dead || philo->ctx->kill_all)
		return (1);
	left_id = philo->id;
	right_id = (philo->id + 1) % philo->ctx->philo_count;
	if (left_id == right_id)
	{
		printf("%zu philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
		return (1);
	}
	pthread_mutex_lock(philo->ctx->forks + left_id);
	if (philo->ctx->kill_all)
		return (release_forks(philo, 1, 0), 1);
	printf("%zu philo N%d took left fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
	pthread_mutex_lock(philo->ctx->forks + right_id);
	if (philo->ctx->kill_all)
		return (release_forks(philo, 1, 1), 1);
	printf("%zu philo N%d took right fork\n", get_timestamp(&philo->ctx->tv), philo->id + 1);
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
	if (philo->is_dead || philo->ctx->kill_all)
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
	while (!philo->ctx->kill_all && !philo->is_dead && (max_meals == -1 || (philo->meal_count < max_meals)))
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
	//printf("---------- N%d is dead: %d | meals: %d -------------\n", philo->id + 1, philo->is_dead, philo->meal_count);
	return (philo->ctx);
}
