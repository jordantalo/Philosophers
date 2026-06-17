/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervising.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:38:30 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 11:06:38 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*listen_death(void *arg)
{
	t_rule	*rules;
	int		is_death;

	rules = (t_rule *)arg;
	is_death = 0;
	while (!is_death)
	{
		sem_wait(rules->sems[SEM_DEATH_SIGNAL_ID]);
		is_death = 1;
	}
	sem_wait(rules->sems[SEM_END_ID]);
	rules->supervisor->is_end = 1;
	sem_post(rules->sems[SEM_END_ID]);
	return (NULL);
}

void	*listen_full(void *arg)
{
	t_rule	*rules;
	int		all_full_count;

	rules = (t_rule *)arg;
	all_full_count = 0;
	while (all_full_count < rules->num_of_philos)
	{
		sem_wait(rules->sems[SEM_SATIATED_SIGNAL_ID]);
		all_full_count++;
	}
	sem_wait(rules->sems[SEM_END_ID]);
	rules->supervisor->is_end = 1;
	sem_post(rules->sems[SEM_END_ID]);
	return (NULL);
}

void	*listen_timeout(void *arg)
{
	t_rule	*rules;
	int		timeout;

	rules = (t_rule *)arg;
	timeout = 0;
	while (!timeout)
	{
		sem_wait(rules->sems[SEM_TIMEOUT_SIGNAL_ID]);
		timeout = 1;
	}
	sem_wait(rules->sems[SEM_END_ID]);
	rules->supervisor->is_end = 1;
	sem_post(rules->sems[SEM_END_ID]);
	return (NULL);
}

int	create_supervising_threads(t_rule *r)
{
	if (pthread_create(&r->supervisor->death_thread, NULL, listen_death, r))
	{
		printf("Failed death thread\n");
		unlock_all_threads(r);
		return (1);
	}
	if (pthread_create(&r->supervisor->all_full_thread, NULL, listen_full, r))
	{
		printf("Failed to create all full thread\n");
		unlock_all_threads(r);
		pthread_join(r->supervisor->death_thread, NULL);
		return (1);
	}
	if (pthread_create(&r->supervisor->timeout_thread, NULL, listen_timeout, r))
	{
		printf("Failed to create all full thread\n");
		unlock_all_threads(r);
		pthread_join(r->supervisor->death_thread, NULL);
		pthread_join(r->supervisor->all_full_thread, NULL);
		return (1);
	}
	return (0);
}

int	supervising_philo_events(t_rule *rules)
{
	int				end;

	if (create_supervising_threads(rules))
		return (1);
	end = 0;
	while (!end)
	{
		sem_wait(rules->sems[SEM_END_ID]);
		if (rules->supervisor->is_end)
			end = 1;
		sem_post(rules->sems[SEM_END_ID]);
		usleep(500);
	}
	unlock_all_threads(rules);
	pthread_join(rules->supervisor->death_thread, NULL);
	pthread_join(rules->supervisor->timeout_thread, NULL);
	pthread_join(rules->supervisor->all_full_thread, NULL);
	return (0);
}
