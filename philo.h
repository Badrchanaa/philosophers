/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:26:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/13 21:47:30 by bchanaa          ###   ########.fr       */
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
# include <limits.h>

# define ST_EAT
# define ST_THINK
# define ST_SLEEP

typedef struct	s_context
{
	struct timeval	tv;
	pthread_mutex_t	*forks;
	int				philo_count;
	int				tt_sleep;
	int				tt_eat;
	int				tt_die;
	int				max_meals;
	int				all_alive;
}				t_context;

typedef struct	s_philo
{
	t_context		*ctx;
	pthread_t		thread;
	size_t			last_meal;
	pthread_mutex_t	time_lock;
	int				id;
	int				is_dead;
	int				meal_count;
	pthread_mutex_t	count_lock;
}			t_philo;

void	*philosopher(void *p_philo);
int		ft_atoi(const char *str);
int		left_philo(int philo_id, int philo_count);
int		right_philo(int philo_id, int philo_count);
int		init_context(int ac, char **av, t_context *ctx);
int		init_philos(t_context *ctx, t_philo *philos);
void	precise_sleep(unsigned int ms);
void	destroy_forks(pthread_mutex_t *forks, int size);
int		init_forks(pthread_mutex_t *forks, int size);
int		philo_starved(t_philo *philo);
size_t	time_diff(struct timeval *tv1, struct timeval *tv2);
size_t	get_timestamp(struct timeval *start_tv);
size_t	get_current_time(void);

#endif
