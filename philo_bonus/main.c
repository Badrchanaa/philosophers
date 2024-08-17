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

int	init_context(int ac, char **av, t_context *ctx)
{
	ctx->philo_count = ft_atoi(av[1]);
	ctx->tt_die = ft_atoi(av[2]);
	ctx->tt_eat = ft_atoi(av[3]);
	ctx->tt_sleep = ft_atoi(av[4]);
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
	ctx->sem_kill = open_semaphore(SEM_KILL, 1);
	if (ctx->sem_kill == SEM_FAILED)
		return (0);
	return (1);
}

int	fork_philosophers(t_context *ctx)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < ctx->philo_count)
	{
		pid = fork();
		if (pid < 0)
			break ;
		if (pid == 0)
			exit(child_main(ctx, i + 1));
		else
		{
			ctx->pids[i] = pid;
			printf("forked philo N%d with pid %d\n", i + 1, pid);
		}
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

int	kill_others(t_context *ctx, int id)
{
	int	i;

	i = 0;
	while (i < ctx->philo_count)
	{
		if (i != id - 1)
		printf("%d killed %d (pid %d)\n", id, i+1, ctx->pids[i]);
			kill(ctx->pids[i], SIGINT);
		i++;
	}
	//kill(ctx->pids[id - 1], SIGKILL);
	return (0);
}

int	kill_all(pid_t *pids, int size)
{
	int	i;

	i = 0;
	printf("kill all\n");
	while (i < size)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	return (0);
}

int	wait_for_death(t_context *ctx)
{
	int	status;
	int	full_count;
	int wpid;

	full_count = 0;
	while (full_count < ctx->philo_count)
	{
		wpid = waitpid(0, &status, WNOHANG);
		if ( wpid > 0)
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
				full_count++;
			else
				return (1);
		}
		usleep(500);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_context	ctx;

	if (ac < 5 || ac > 6)
		return (1);
	if (!init_context(ac, av, &ctx))
		return (1);
	gettimeofday(&ctx.tv, NULL);
	if (!fork_philosophers(&ctx))
		return (close_semaphores(&ctx), 1);
	if (wait_for_death(&ctx))
		kill_all(ctx.pids, ctx.philo_count);
	sem_wait(ctx.sem_print);
	sem_post(ctx.sem_print);
	while (waitpid(0, NULL, 0) >= 0)
		continue ;
	close_semaphores(&ctx);
	return (0);
}

int	main(int ac, char **av)
{
	t_context	ctx;
	if (ac < 5 || ac > 6)
		return (1);
	if (!init_context(ac, av, &ctx))
		return (1);
	gettimeofday(&ctx.tv, NULL);
	if (!fork_philosophers(&ctx))
		return (close_semaphores(&ctx), 1);
	monitor_all();
		
	return (0);

}

// MAIN PROCESS:
// Forks the philosopher processes and waits for one to terminate to signal all child processes

// CHILD PROCESS:
// Creates a new thread for the philosopher routine and monitor its death
// monitor should notify philosopher thread of its death as soon as possible
// then child process should notify the main process or its siblings of its death asap

// ON CHILD:
// how to notify siblings of death without causing philosopher thread to hang ?
// notifying using print or state semaphores causes philosopher thread to hang
// can only use semaphores or exit
// ON MAIN:
// using a semaphore instead of waitpid NOHANG can work for death but may not work for meal limit
// using waitpid NOHANG does not notify other child processes immediately
// blocking print semaphore causes philosopher thread to hang

// Main process scheduling of philosophers to eat