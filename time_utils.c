/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 19:12:15 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/12 15:25:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	time_diff(struct timeval *tv1, struct timeval *tv2)
{
	return ((tv2->tv_sec * 1000 + tv2->tv_usec / 1000) - 
			(tv1->tv_sec * 1000 + tv1->tv_usec / 1000));
}

unsigned int	get_timestamp(struct timeval *start_tv)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (time_diff(start_tv, &tv));
}

void	precise_sleep(unsigned int ms)
{
	struct timeval	start_tv;
	struct timeval	tv;
	unsigned int	passed_ms;

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
