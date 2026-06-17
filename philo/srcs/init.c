/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:48:39 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/11 11:16:52 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_philos_mutex(t_philo *philos, t_rule *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_of_philos)
	{
		if (pthread_mutex_init(&philos[i].data_mutex, NULL) != 0)
		{
			destroy_philos_mutex(philos, i);
			return (printf("Error occurs when trying to init mutex\n"), 1);
		}
		i++;
	}
	return (0);
}

int	init_forks_mutex(t_rule *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_of_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
		{
			destroy_forks_mutex(rules, i);
			return (printf("Error occurs when trying to init mutex\n"), 1);
		}
		i++;
	}
	return (0);
}

int	create_philos_threads(t_philo *philos, t_rule *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_of_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, routine_philo, &philos[i]))
		{
			printf("Error occurs when trying to create thread %d\n", i);
			pthread_mutex_lock(&rules->end_mutex);
			rules->simulation_end = 1;
			pthread_mutex_unlock(&rules->end_mutex);
			join_philos_threads(philos, i);
			return (1);
		}
		i++;
	}
	return (0);
}
