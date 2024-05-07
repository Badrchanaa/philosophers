/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:26:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/07 21:37:02 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct	s_context
{
	size_t	philo_count;
	size_t	tt_sleep;
	size_t	tt_eat;
	size_t	tt_die;
	size_t	max_eat;
}				t_context;

int	ft_atoi(const char *str);
int	left_philo(int philo_id, int philo_count);
int	right_philo(int philo_id, int philo_count);
int	init_context(int ac, char **av, t_context *ctx);

#endif
