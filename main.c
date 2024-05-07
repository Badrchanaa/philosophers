/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:11:01 by bchanaa           #+#    #+#             */
/*   Updated: 2024/05/07 21:46:58 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(int ms)
{
	printf("sleeping for %d\n", ms);
}

int	philosophers(t_context *ctx)
{
	(void)ctx;
	return (0);
}

int	main(int ac, char *av[])
{
	t_context	ctx;

	if (ac < 5 || ac > 6)
		return (0);
	if (!init_context(ac, av, &ctx))
		return (0);
	else
	{
		printf("ALL GOOD:\n");
		printf("	philo count: %d\n", ctx->philo_count);
		printf("	time to die: %d\n", ctx->tt_die);
		printf("	time to eat: %d\n", ctx->tt_eat);
		printf("	time to sleep: %d\n", ctx->tt_sleep);
	}
	philosophers(&ctx);
	return (0);
}
