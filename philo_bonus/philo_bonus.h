/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:22:19 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 19:30:56 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <stdio.h>
# include <fcntl.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"
# define SEM_STATE "/philo_state"
# define SEM_PERMS 0600

# define PHIL_ALIVE
# define PHIL_DEAD
# define PHIL_FULL

typedef struct	s_context
{
	struct timeval	tv;
	sem_t			*sem_forks;
	sem_t			*sem_print;
	//sem_t			*sem_kill;
	pid_t			pids[200];
	int				philo_count;
	int				tt_sleep;
	int				tt_eat;
	int				tt_die;
	int				max_meals;
}				t_context;

typedef struct	s_philo
{
	pthread_t	thread;
	sem_t		*sem_state;
	int			id;
	int			meal_count;
	size_t		last_meal;
}				t_philo;

size_t	time_diff(struct timeval *tv1, struct timeval *tv2);
size_t	get_current_time(void);
void	precise_sleep(unsigned int ms);

#endif
