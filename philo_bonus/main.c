/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 21:10:50 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 20:21:40 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	valid_config(t_context *ctx)
{
	return (ctx->philo_count > 0 && ctx->tt_die > 0
			&& ctx->tt_eat >= 0 && ctx->tt_sleep >= 0);
}

sem_t	*open_semaphore(const char *name, int val)
{
	sem_t	*sem;

	sem = sem_open(name, O_CREAT | O_EXCL, SEM_PERMS, val);
	if (sem == SEM_FAILED)
	{
		sem_unlink(name);
		sem = sem_open(name, O_CREAT | O_EXCL, SEM_PERMS, val);
	}
	if (sem != SEM_FAILED)
		sem_unlink(name);
	return (sem);
}

int	init_context(int ac, char **av, t_context *ctx)
{
	ctx->philo_count = ft_atoi(av[1]);
	ctx->tt_die = ft_atoi(av[2]);
	ctx->tt_eat = ft_atoi(av[3]);
	ctx->tt_sleep =	ft_atoi(av[4]);
	if (ac == 6)
		ctx->max_meals = ft_atoi(av[5]);
	else
		ctx->max_meals = -1;
	if (!valid_config(ctx))
		return (0);
	ctx->sem_forks = open_semaphore(SEM_FORKS, ctx->philo_count);
	if (ctx->sem_forks == SEM_FAILED)
		return (0);
	ctx->sem_print = open_semaphore(SEM_PRINT, 1);
	if (ctx->sem_print == SEM_FAILED)
		return (0);
	return (1);
}

void	child_main(t_context *ctx, int id)
{
	t_philo	philo;

	philo->meal_count = 0;
	philo->id = id;
	philo->last_meal = get_current_time();
	pthread_create(&philo->thread, NULL, philosopher, &philo);
	monitor(ctx);
	pthread_join(&philo->thread, NULL);
	sem_close(ctx->sem_forks);
}

int	fork_philosophers(t_context *ctx)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < ctx->philo_count)
	{
		pid = fork();
		if (pid < 0) // Kill ALL ??
			break;
		if (pid == 0)
		{
			child_main(ctx, i + 1);
			exit(0);
		}
		else
			ctx->pids[i] = pid;
		i++;
	}
	if (i < ctx->philo_count)
	{
		while (i > 0)
		{
			waitpid(-1, NULL, 0);
			i--;
		}
	}
	return (1);
}

void	wait_all(t_context *ctx)
{
	while (wait(NULL) >= 0)
		continue;
}

int	kill_all(pid_t *pids, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		kill(pids[i], SIGKILL);
		i++:
	}
	return (0);
}
// Solution 1
// Each child process monitors its philosopher, and posts sem_kill if it dies
// Main process waits for sem_kill then kills all philosophers

// Solution 2 (best)
// Each child process monitors its philosopher, and kills itself when its philosopher dies
// Main process waits for any child process to terminate, then kills all the other ones

int	main(int ac, char **av)
{
	t_context	ctx;

	if (ac < 5 || ac > 6)
		return (1);
	if (!init_context(ac, av, &ctx))
		return (1);
	// Wait for a philosopher to die
	sem_wait(ctx.sem_kill);
	fork_philosophers();
	if (wait(NULL)) // Someone died
		kill_all(ctx.pids, ctx.philo_count);
	//kill_all(ctx.pids, ctx.philo_count);
	wait_all(&ctx);
	return (0);
}
