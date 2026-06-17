/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:47:56 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 08:33:12 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	destroy_philos_mutex(t_philo *philos, int to_destroy)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (i < to_destroy)
	{
		if (pthread_mutex_destroy(&philos[i].data_mutex))
		{
			printf("mutex destroy failed\n");
			res = 1;
		}
		i++;
	}
	return (res);
}

int	destroy_forks_mutex(t_rule *rules, int to_destroy)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (i < to_destroy)
	{
		if (pthread_mutex_destroy(&rules->forks[i]))
		{
			printf("mutex destroy failed\n");
			res = 1;
		}
		i++;
	}
	return (res);
}

int	join_philos_threads(t_philo *philos, int to_join)
{
	int	i;
	int	exit_status;

	i = 0;
	exit_status = 0;
	while (i < to_join)
	{
		if (pthread_join(philos[i].thread, NULL))
		{
			printf("pthread_join failed\n");
			exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}
