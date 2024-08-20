/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 19:12:15 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/13 21:48:09 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	time_diff(struct timeval *tv1, struct timeval *tv2)
{
	return ((tv2->tv_sec * 1000 + tv2->tv_usec / 1000) - \
			(tv1->tv_sec * 1000 + tv1->tv_usec / 1000));
}

size_t	get_timestamp(struct timeval *start_tv)
{
	struct timeval	tv;

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

void	print_time(struct timeval *start_tv)
{
	struct timeval	tv;
	unsigned int	ms;

	gettimeofday(&tv, NULL);
	ms = time_diff(&tv, start_tv);
	printf("timestamp: %u\n", ms);
}
