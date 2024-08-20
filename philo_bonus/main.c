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

int	kill_all(pid_t *pids, int size)
{
	int	i;

	i = 0;
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
	int	wpid;

	full_count = 0;
	while (full_count < ctx->philo_count)
	{
		wpid = waitpid(0, &status, WNOHANG);
		if (wpid > 0)
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
		return (printf("philo_bonus: invalid arguments.\n"), 1);
	if (!init_semaphores(&ctx))
		return (printf("philo_bonus: failed to open semaphores\n"), 1);
	gettimeofday(&ctx.tv, NULL);
	if (!fork_philosophers(&ctx))
		return (close_semaphores(&ctx), 1);
	if (pthread_create(&ctx.meal_thread, NULL, monitor_meals, &ctx))
		return (printf("philo_bonus: create_thread failed\n"), 1);
	if (pthread_create(&ctx.death_thread, NULL, monitor_death, &ctx))
		return (printf("philo_bonus: create_thread failed\n"), 1);
	set_state(&ctx, ALIVE);
	while (get_state(&ctx) == ALIVE)
		usleep(1000);
	sem_post(ctx.sem_kill);
	sem_post(ctx.sem_meal);
	pthread_join(ctx.meal_thread, NULL);
	pthread_join(ctx.death_thread, NULL);
	while (waitpid(0, NULL, 0) >= 0)
		continue ;
	return (close_semaphores(&ctx), 0);
}
