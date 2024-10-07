/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 22:13:09 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/18 22:13:10 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	close_semaphores(t_context *ctx)
{
	sem_close(ctx->sem_forks);
	sem_close(ctx->sem_print);
	sem_close(ctx->sem_state);
	sem_close(ctx->sem_waiter);
	sem_close(ctx->sem_meal);
	sem_close(ctx->sem_kill);
	sem_close(ctx->sem_main_state);
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
	return (sem);
}

int	valid_config(t_context *ctx)
{
	if (ctx->philo_count > 200)
		return (0);
	return (ctx->philo_count > 0 && ctx->tt_die > 0 \
			&& ctx->tt_eat > 0 && ctx->tt_sleep > 0);
}
