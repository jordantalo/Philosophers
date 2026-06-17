/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:37:05 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/12 08:54:38 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	create_threads(t_philo *philos, t_rule *rules)
{
	if (create_philos_threads(philos, rules))
		return (1);
	if (pthread_create(&rules->monitor_thread, NULL, routine_monitor, philos))
	{
		printf("Error occurs when trying to create monitoring thread\n");
		pthread_mutex_lock(&rules->end_mutex);
		rules->simulation_end = 1;
		pthread_mutex_unlock(&rules->end_mutex);
		join_philos_threads(philos, rules->num_of_philos);
		return (1);
	}
	return (0);
}

int	join_threads(t_philo *philos, t_rule *rules)
{
	join_philos_threads(philos, rules->num_of_philos);
	if (pthread_join(rules->monitor_thread, NULL))
		printf("pthread_join failed\n");
	return (0);
}

int	init_mutex(t_philo *philos, t_rule *rules)
{
	if (init_philos_mutex(philos, rules))
		return (1);
	if (init_forks_mutex(rules))
		return (destroy_philos_mutex(philos, rules->num_of_philos), 1);
	if (pthread_mutex_init(&rules->display_mutex, NULL) != 0)
	{
		destroy_philos_mutex(philos, rules->num_of_philos);
		destroy_forks_mutex(rules, rules->num_of_philos);
		return (1);
	}
	if (pthread_mutex_init(&rules->end_mutex, NULL) != 0)
	{
		destroy_philos_mutex(philos, rules->num_of_philos);
		destroy_forks_mutex(rules, rules->num_of_philos);
		pthread_mutex_destroy(&rules->display_mutex);
		return (1);
	}
	return (0);
}

int	destroy_mutex(t_philo *philos, t_rule *rules)
{
	int	status;

	status = 0;
	if (destroy_philos_mutex(philos, rules->num_of_philos))
	{
		printf("mutex destroy failed\n");
		status = 1;
	}
	if (destroy_forks_mutex(rules, rules->num_of_philos))
	{
		printf("mutex destroy failed\n");
		status = 1;
	}
	if (pthread_mutex_destroy(&rules->display_mutex))
	{
		status = 1;
		printf("mutex destroy failed\n");
	}
	if (pthread_mutex_destroy(&rules->end_mutex))
	{
		status = 1;
		printf("mutex destroy failed\n");
	}
	return (status);
}

int	simulate(t_rule *rules, t_philo *philos)
{
	int	exit_status;

	exit_status = 0;
	if (init_mutex(philos, rules))
		return (1);
	if (init_start_time(rules))
		return (destroy_mutex(philos, rules), 1);
	if (rules->num_of_philos == 1)
	{
		if (special_case(rules))
			return (destroy_mutex(philos, rules), 1);
		return (destroy_mutex(philos, rules), 0);
	}
	if (create_threads(philos, rules))
		return (destroy_mutex(philos, rules), 1);
	if (join_threads(philos, rules))
		exit_status = 1;
	if (destroy_mutex(philos, rules))
		exit_status = 1;
	return (exit_status);
}
