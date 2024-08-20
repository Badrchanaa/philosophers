/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:54:19 by bchanaa           #+#    #+#             */
/*   Updated: 2024/08/20 14:54:20 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(pthread_mutex_t *forks, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(forks + i);
		i++;
	}
}

void	destroy_philosophers(t_philo *philos, int size)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < size)
	{
		philo = philos + i;
		destroy_philo(philo);
		i++;
	}
}

void	release_forks(t_philo *philo)
{
	int	left_id;
	int	right_id;

	left_id = philo->id;
	right_id = right_philo(philo->id, philo->ctx->philo_count);
	pthread_mutex_unlock(philo->ctx->forks + left_id);
	pthread_mutex_unlock(philo->ctx->forks + right_id);
}
