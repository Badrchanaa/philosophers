/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 22:19:07 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 22:19:08 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

size_t	time_diff(struct timeval *tv1, struct timeval *tv2)
{
	return ((tv2->tv_sec * 1000 + tv2->tv_usec / 1000) - 
			(tv1->tv_sec * 1000 + tv1->tv_usec / 1000));
}

size_t	get_timestamp(struct timeval *start_tv)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (time_diff(start_tv, &tv));
}

size_t	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(unsigned int ms)
{
	struct timeval	start_tv;
	struct timeval	tv;
	size_t			passed_ms;

	gettimeofday(&start_tv, NULL);
	passed_ms = 0;
	while (passed_ms < ms)
	{
		usleep(100);
		gettimeofday(&tv, NULL);
		passed_ms = time_diff(&start_tv, &tv);
	}
}
