/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:26:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/12 18:18:14 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>

# define ST_EAT
# define ST_THINK
# define ST_SLEEP

typedef struct	s_context
{
	struct timeval	tv;
	int				*forks;
	pthread_mutex_t	*mutexes;
	int				philo_count;
	int				tt_sleep;
	int				tt_eat;
	int				tt_die;
	int				max_meals;
	int				all_alive;
}				t_context;

typedef struct	s_philo
{
	t_context	*ctx;
	pthread_t	thread;
	struct timeval	last_meal_tv;
	int			id;
	int			is_dead;
	int			meal_count;
}			t_philo;

int		ft_atoi(const char *str);
int		left_philo(int philo_id, int philo_count);
int		right_philo(int philo_id, int philo_count);
int		init_context(int ac, char **av, t_context *ctx);
void	precise_sleep(unsigned int ms);
int		init_mutexes(pthread_mutex_t *mutexes, int size);
void	destroy_mutexes(pthread_mutex_t *mutexes, int size);
int	philo_starved(t_philo *philo);
unsigned int	time_diff(struct timeval *tv1, struct timeval *tv2);
unsigned int	get_timestamp(struct timeval *start_tv);

#endif
