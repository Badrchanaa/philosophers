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
	ctx->sem_waiter = open_semaphore(SEM_WAITER, 1);
	if (ctx->sem_waiter == SEM_FAILED)
		return (0);
	ctx->sem_state = open_semaphore(SEM_STATE, 1);
	if (ctx->sem_state == SEM_FAILED)
		return (0);
	return (1);
}

void	close_semaphores(t_context *ctx)
{
	sem_close(ctx->sem_forks);
	sem_close(ctx->sem_print);
	sem_close(ctx->sem_state);
	sem_close(ctx->sem_waiter);
}

int	child_main(t_context *ctx, int id)
{
	t_philo	philo;
	int		status;

	philo->meal_count = 0;
	philo->id = id;
	philo->last_meal = get_current_time();
	pthread_create(&philo->thread, NULL, philosopher, &philo);
	status = monitor(ctx, &philo);
	pthread_join(&philo->thread, NULL);
	close_semaphores(ctx);
	return (status);
}

void	monitor(t_context *ctx, t_philo *philo)
{
	t_state	state;

	while (1)
	{
		sem_wait(sem_state);
		get_philo_state(philo, ctx, &state);
		if (state->meal_count >= ctx->max_meals)
			return (0);
		if (get_current_time() - state->last_meal > tt_die)
		{
			sem_wait(sem_print);
			printf("%zu philo N%d is dead.\n", get_timestamp(&ctx->tv), philo->id);
			sem_post(sem_print);
			philo->state = PHIL_DEAD;
			sem_post(sem_state);
			return (1);
		}
		sem_post(sem_state);
		precise_sleep(500);
	}
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
			break ;
		if (pid == 0)
			exit(child_main(ctx, i + 1));
		else
			ctx->pids[i] = pid;
		i++;
	}
	if (i < ctx->philo_count)
	{
		while (i-- > 0)
			waitpid(-1, NULL, 0);
		return (0);
	}
	return (1);
}

void	wait_all(t_context *ctx)
{
	while (waitpid(0, NULL, 0) >= 0)
		continue ;
}

int	kill_all(pid_t *pids, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (waitpid(pids[i], NULL, WNOHANG) >= 0) // Check if child process is still running
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

int	wait_for_death(t_context *ctx)
{
	int	status;

	while (1)
	{
		if (waitpid(0, &status, WNOHANG) > 0)
		{	
			if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
				break ;
		}
		usleep(500);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_context	ctx;

	if (ac < 5 || ac > 6)
		return (1);
	if (!init_context(ac, av, &ctx))
		return (1);
	// Wait for a philosopher to die
	sem_wait(ctx.sem_kill);
	if (!fork_philosophers(&ctx))
		return (close_semaphores(&ctx), 1);
	wait_for_death(&ctx);
	kill_all(ctx.pids, ctx.philo_count);
	// if (wait(NULL)) // Someone died
	// 	kill_all(ctx.pids, ctx.philo_count);
	wait_all(&ctx);
	close_semaphores(&ctx);
	return (0);
}
