/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:47:43 by bchanaa           #+#    #+#             */
/*   Updated: 2024/10/02 14:47:46 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine_loop(t_context *ctx, t_philo *philo)
{
	int	first_iteration;

	first_iteration = 1;
	while (still_alive(philo) && !is_full(philo))
	{
		print_state(philo, THINKING, philo->id + 1);
		if (first_iteration && (philo->id + 1) % 2 != 0)
			usleep(2000);
		if (first_iteration && ctx->philo_count % 2 == 0)
			first_iteration = 0;
		if (philo_eat(philo))
			break ;
		pthread_mutex_lock(&philo->count_lock);
		philo->meal_count++;
		if (ctx->max_meals - philo->meal_count == 0)
		{
			philo->meal_count *= -1;
			pthread_mutex_unlock(&philo->count_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->count_lock);
		if (philo_sleep(philo))
			break ;
	}
}

void	*philosopher(void *p_philo)
{
	int		max_meals;
	t_philo	*philo;

	philo = p_philo;
	max_meals = philo->ctx->max_meals;
	pthread_mutex_lock(&philo->ctx->kill_lock);
	if (philo->ctx->kill_all)
		return (philo->ctx);
	pthread_mutex_unlock(&philo->ctx->kill_lock);
	routine_loop(philo->ctx, philo);
	return (philo->ctx);
}
