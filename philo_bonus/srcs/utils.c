/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:28:02 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 11:04:27 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	unlock_all_threads(t_rule *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_of_philos)
	{
		sem_post(rules->sems[SEM_STOP_CHILDREN_ID]);
		i++;
	}
	sem_post(rules->sems[SEM_DEATH_SIGNAL_ID]);
	sem_post(rules->sems[SEM_TIMEOUT_SIGNAL_ID]);
	i = 0;
	while (i < rules->num_of_philos)
	{
		sem_post(rules->sems[SEM_SATIATED_SIGNAL_ID]);
		i++;
	}
}

int	init_start_time(t_rule *rules)
{
	rules->start_time = get_current_time();
	if (rules->start_time == -1)
		return (1);
	return (0);
}

void	print_action(t_philo *philo, char *action, int id)
{
	long	timestamp;
	int		is_simulation_end;

	is_simulation_end = 0;
	timestamp = get_current_time() - philo->rules->start_time;
	sem_wait(philo->rules->sems[SEM_PRINT_LOCK_ID]);
	sem_wait(philo->data_lock);
	is_simulation_end = philo->simulation_end;
	sem_post(philo->data_lock);
	if (!is_simulation_end)
		printf(action, timestamp, id);
	sem_post(philo->rules->sems[SEM_PRINT_LOCK_ID]);
}

void	ft_usleep(long wait_mls)
{
	long	start;

	start = get_current_time();
	while (get_current_time() - start < wait_mls)
		usleep(100);
}

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return (time.tv_sec * (long)1000 + time.tv_usec / 1000);
}
