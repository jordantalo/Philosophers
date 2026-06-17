/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:28:11 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/17 08:21:16 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *philo)
{
	int		id;

	id = philo->id;
	sem_wait(philo->rules->sems[SEM_EAT_LOCK_ID]);
	sem_wait(philo->rules->sems[SEM_FORKS_ID]);
	sem_wait(philo->rules->sems[SEM_FORKS_ID]);
	print_action(philo, "%ld %d has taken a fork\n", id);
	print_action(philo, "%ld %d has taken a fork\n", id);
}

void	eat_spaghetti(t_philo *philo)
{
	int		id;

	id = philo->id;
	print_action(philo, "%ld %d is eating\n", id);
	sem_wait(philo->data_lock);
	philo->last_meal = get_current_time() - philo->rules->start_time;
	sem_post(philo->data_lock);
	ft_usleep(philo->rules->time_to_eat);
	sem_post(philo->rules->sems[SEM_FORKS_ID]);
	sem_post(philo->rules->sems[SEM_FORKS_ID]);
	sem_post(philo->rules->sems[SEM_EAT_LOCK_ID]);
}

void	sleep_and_think(t_philo *philo)
{
	int	id;

	id = philo->id;
	print_action(philo, "%ld %d is sleeping\n", id);
	ft_usleep(philo->rules->time_to_sleep);
	print_action(philo, "%ld %d is thinking\n", id);
	usleep(500);
}

void	*routine_philo(void *arg)
{
	t_philo	*philo;
	int		id;
	int		end;

	philo = (t_philo *)arg;
	id = philo->id;
	end = 0;
	while (!end)
	{
		take_forks(philo);
		eat_spaghetti(philo);
		sem_wait(philo->data_lock);
		philo->meals_eaten++;
		sem_post(philo->data_lock);
		sleep_and_think(philo);
		sem_wait(philo->data_lock);
		end = philo->simulation_end;
		sem_post(philo->data_lock);
	}
	return (NULL);
}

void	*end_routine(void *arg)
{
	int		end;
	t_philo	*philo;

	end = 0;
	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(philo->rules->time_to_eat / 2);
	while (!end)
	{
		sem_wait(philo->rules->sems[SEM_STOP_CHILDREN_ID]);
		end = 1;
	}
	sem_wait(philo->data_lock);
	philo->simulation_end = 1;
	sem_post(philo->data_lock);
	return (NULL);
}
