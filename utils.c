/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:25:53 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/13 21:47:10 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	right_philo(int philo_id, int philo_count)
{
	if (philo_id == philo_count - 1)
		return (0);
	return (philo_id + 1);
}

int	philo_starved(t_philo *philo)
{
	size_t	current_time;
	size_t	last_meal;
	size_t	diff;

	pthread_mutex_lock(&philo->time_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->time_lock);
	current_time = get_current_time();
	diff = current_time - last_meal;
	return (diff > INT_MAX || (int)diff >= philo->ctx->tt_die);
}
