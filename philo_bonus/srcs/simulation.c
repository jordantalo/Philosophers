/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 08:40:47 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/16 11:47:51 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	one_philo_routine(t_philo *philo)
{
	long	cur_time;

	cur_time = get_current_time() - philo->rules->start_time;
	if (cur_time == -1)
		return (1);
	printf("%ld 1 has taken a fork\n", cur_time);
	ft_usleep(philo->rules->time_to_die);
	cur_time = get_current_time() - philo->rules->start_time;
	if (cur_time == -1)
		return (1);
	printf("%ld 1 died\n", cur_time);
	sem_post(philo->rules->sems[SEM_DEATH_SIGNAL_ID]);
	return (0);
}

int	philo_process(t_philo *p)
{
	if (p->rules->num_of_philos == 1)
	{
		if (one_philo_routine(p))
			return (1);
		return (0);
	}
	if (pthread_create(&p->thread_philo, NULL, routine_philo, p))
		return (printf("Failed to create philo %d thread\n", p->pid), 1);
	if (pthread_create(&p->thread_monitor, NULL, routine_monitor, p))
	{
		unlock_all_threads(p->rules);
		pthread_join(p->thread_philo, NULL);
		return (printf("Failed to create monitor thread %d\n", p->pid), 1);
	}
	if (pthread_create(&p->thread_simu_end, NULL, end_routine, p))
	{
		unlock_all_threads(p->rules);
		pthread_join(p->thread_philo, NULL);
		pthread_join(p->thread_monitor, NULL);
		return (printf("Failed to create end thread %d\n", p->pid), 1);
	}
	pthread_join(p->thread_philo, NULL);
	pthread_join(p->thread_monitor, NULL);
	pthread_join(p->thread_simu_end, NULL);
	return (0);
}

void	launch_philos_process(t_rule *rules, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < rules->num_of_philos)
	{
		philos[i].pid = fork();
		if (philos[i].pid < 0)
		{
			printf("fork failed\n");
			exit(1);
		}
		if (philos[i].pid == 0)
		{
			if (philo_process(&philos[i]))
				printf("philo %d process failed\n", philos[i].id);
			close_all_sems(rules, philos, 0);
			free(philos);
			exit(0);
		}
		i++;
	}
}

void	wait_for_philos(t_rule *rules, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < rules->num_of_philos)
	{
		if (waitpid(philos[i].pid, NULL, 0) == -1)
			printf("waipid failed\n");
		i++;
	}
}

int	simulation(t_rule *rules, t_philo *philos)
{
	if (open_all_sems(rules, philos))
		return (1);
	if (init_start_time(rules))
		return (1);
	launch_philos_process(rules, philos);
	supervising_philo_events(rules);
	wait_for_philos(rules, philos);
	if (close_all_sems(rules, philos, 1))
		return (1);
	return (0);
}
