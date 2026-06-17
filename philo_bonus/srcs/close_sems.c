/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_sems.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:20:47 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 10:05:49 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	close_local_sems(t_philo *philo, int do_unlink)
{
	int	exit_status;

	exit_status = 0;
	if (sem_close(philo->data_lock) == -1)
	{
		printf("Failed to close semaphore %s\n", philo->s_name);
		exit_status = 1;
	}
	if (do_unlink && sem_unlink(philo->s_name) == -1)
	{
		printf("Failed to unlink semaphore %s\n", philo->s_name);
		exit_status = 1;
	}
	return (exit_status);
}

int	close_global_sems(t_rule *rules, int to_close, int do_unlink)
{
	int	i;
	int	exit_status;

	i = 0;
	exit_status = 0;
	while (i < to_close)
	{
		if (sem_close(rules->sems[i]) == -1)
		{
			printf("Failed to close semaphore %s\n", rules->s_names[i]);
			exit_status = 1;
		}
		if (do_unlink && sem_unlink(rules->s_names[i]) == -1)
		{
			printf("Failed to unlink semaphore %s\n", rules->s_names[i]);
			exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}

int	close_all_sems(t_rule *rules, t_philo *philos, int do_unlink)
{
	int	i;
	int	exit_status;

	i = 0;
	exit_status = 0;
	if (close_global_sems(rules, N_SEMS, do_unlink))
		exit_status = 1;
	while (i < rules->num_of_philos)
	{
		if (close_local_sems(&philos[i], do_unlink))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
