/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 09:53:42 by bchanaa           #+#    #+#             */
/*   Updated: 2024/08/20 09:53:43 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_usage(void)
{
	printf("Usage: ./philo_bonus PHILOS TTD TTE TTS [MAX_MEALS]\n");
	printf("All arguments are in milliseconds and must be valid integers.\n");
}

int	init_context(int ac, char **av, t_context *ctx)
{
	ctx->state = UNKNOWN;
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
	return (1);
}

int	init_semaphores(t_context *ctx)
{
	ctx->sem_forks = open_semaphore(SEM_FORKS, ctx->philo_count);
	ctx->sem_waiter = open_semaphore(SEM_WAITER, 1);
	ctx->sem_print = open_semaphore(SEM_PRINT, 1);
	ctx->sem_state = open_semaphore(SEM_STATE, 1);
	ctx->sem_main_state = open_semaphore(SEM_MAIN_STATE, 1);
	ctx->sem_kill = open_semaphore(SEM_KILL, 0);
	ctx->sem_meal = open_semaphore(SEM_MEAL, 0);
	if (ctx->sem_forks == SEM_FAILED || ctx->sem_waiter == SEM_FAILED \
		|| ctx->sem_print == SEM_FAILED || ctx->sem_state == SEM_FAILED \
		|| ctx->sem_kill == SEM_FAILED || ctx->sem_meal == SEM_FAILED \
		|| ctx->sem_main_state == SEM_FAILED)
		return (0);
	return (1);
}
