/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 19:12:15 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/11 14:29:37 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// useconds_t	tv_to_usec(struct timeval *tv)
// {
// 	useconds_t	usec;
// 
// 	usec = tv->tv_sec * 1000000 + tv->tv_usec;
// 	return (usec);
// }
// 
// useconds_t	time_diff(struct timeval *start_tv)
// {
// 	unsigned long	diff;
// 	struct timeval	tv;
// 
// 	gettimeofday(&tv, NULL);
// 	diff = tv_to_usec(&tv) - tv_to_usec(start_tv);
// 	return (diff);
// }

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

// int main(int ac, char **av)
// {
// 	struct timeval tv;
// 	struct timeval tv2;
// 	gettimeofday(&tv, NULL);
// 	precise_sleep(3000);
// 	gettimeofday(&tv2, NULL);
// 	printf("slept for %d\n", time_diff(&tv, &tv2));
// 	return (0);
// }
