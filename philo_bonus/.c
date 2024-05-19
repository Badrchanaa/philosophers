/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:55:25 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 17:58:35 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_isspace(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	sign;

	sign = 1;
	i = 0;
	res = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + str[i] - '0';
		i++;
	}
	return (res * sign);
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

int	is_dead(t_philo *philo)
{
	int	count;

	sem_wait(philo->sem_state);
	count = philo->meal_count;
	sem_post(philo->sem_state);
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
