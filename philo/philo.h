/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 21:26:49 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 15:51:48 by bchanaa          ###   ########.fr       */
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

typedef enum e_state
{
	HELD_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DEAD
}	t_state;

typedef struct s_context
{
	struct timeval	tv;
	pthread_mutex_t	kill_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	int				first_delay;
	int				philo_count;
	int				tt_sleep;
	int				tt_eat;
	int				tt_die;
	int				max_meals;
	int				kill_all;
}				t_context;

typedef struct s_philo
{
	t_context		*ctx;
	pthread_t		thread;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	count_lock;
	size_t			last_meal;
	int				id;
	int				is_dead;
	int				meal_count;
}			t_philo;

int		still_alive(t_philo *philo);
void	destroy_philo(t_philo *philo);
int		is_full(t_philo *philo);
int		philo_eat(t_philo *philo);
int		philo_sleep(t_philo *philo);
void	release_forks(t_philo *philo);
int		ft_atoi(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
int		ft_isdigit(int c);
void	print_state(t_philo *philo, t_state state, int id);
int		valid_arguments(int ac, char **av);
int		valid_config(t_context *ctx);
void	*philosopher(void *p_philo);
void	routine_loop(t_context *ctx, t_philo *philo);
int		ft_atoi(const char *str);
int		left_philo(int philo_id, int philo_count);
int		right_philo(int philo_id, int philo_count);
int		init_context(int ac, char **av, t_context *ctx);
int		init_philos(t_context *ctx, t_philo *philos);
void	precise_sleep(unsigned int ms);
void	destroy_forks(pthread_mutex_t *forks, int size);
void	destroy_philosophers(t_philo *philos, int size);
int		init_forks(pthread_mutex_t *forks, int size);
int		philo_starved(t_philo *philo);
size_t	time_diff(struct timeval *tv1, struct timeval *tv2);
size_t	get_timestamp(struct timeval *start_tv);
size_t	get_current_time(void);

void	apocalypse(t_context *ctx, t_philo *philo);
int		is_dead(t_philo *philo);

#endif
