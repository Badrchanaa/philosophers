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

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <sys/wait.h>
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
# define SEM_WAITER "/philo_waiter"
# define SEM_KILL "/philo_kill"
# define SEM_MEAL "/philo_meal"
# define SEM_MAIN_STATE "/philo_main_state"
# define SEM_PERMS 0600

# define PHIL_ALIVE 0
# define PHIL_DEAD 1
# define PHIL_FULL 2

typedef enum e_state
{
	ALIVE,
	FULL,
	DEAD,
	UNKNOWN
}		t_state;

typedef struct s_context
{
	pthread_t		meal_thread;
	pthread_t		death_thread;
	int				state;
	struct timeval	tv;
	sem_t			*sem_forks;
	sem_t			*sem_print;
	sem_t			*sem_waiter;
	sem_t			*sem_state;
	sem_t			*sem_main_state;
	sem_t			*sem_kill;
	sem_t			*sem_meal;
	pid_t			pids[200];
	int				philo_count;
	int				tt_sleep;
	int				tt_eat;
	int				tt_die;
	int				max_meals;
}				t_context;

typedef struct s_philo
{
	t_context	*ctx;
	pthread_t	thread;
	int			id;
	int			state;
	int			meal_count;
	size_t		last_meal;
}				t_philo;

typedef struct s_philo_state
{
	size_t	last_meal;
	int		meal_count;
	int		state;
}		t_philo_state;

int		init_context(int ac, char **av, t_context *ctx);
int		init_semaphores(t_context *ctx);

void	set_state(t_context *ctx, t_state state);
t_state	get_state(t_context *ctx);

void	*monitor_meals(void *p_context);
void	*monitor_death(void *p_context);

int		kill_all(pid_t *pids, int size);
int		ft_atoi(const char *str);
int		valid_config(t_context *ctx);

size_t	time_diff(struct timeval *tv1, struct timeval *tv2);
size_t	get_current_time(void);
void	precise_sleep(unsigned int ms);
size_t	get_timestamp(struct timeval *start_tv);

sem_t	*open_semaphore(const char *name, int val);
void	close_semaphores(t_context *ctx);

int		is_dead(t_philo *philo, t_context *ctx);
void	get_philo_state(t_philo *philo, t_philo_state *state);
int		child_main(t_context *ctx, int id);

int		take_fork(t_philo *philo, t_context *ctx);
int		p_eat(t_philo *philo, t_context *ctx);
int		p_sleep(t_philo *philo, t_context *ctx);
void	repeat_routine(t_philo *philo, t_context *ctx);

#endif
