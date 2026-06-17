/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:39:45 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 11:16:38 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	is_philo_satiated(t_rule *rules, t_philo *philo)
{
	if (rules->num_of_meals == -1)
		return ;
	sem_wait(philo->data_lock);
	if (!philo->is_satiated && philo->meals_eaten >= rules->num_of_meals)
	{
		sem_post(rules->sems[SEM_SATIATED_SIGNAL_ID]);
		philo->is_satiated = 1;
	}
	sem_post(philo->data_lock);
}

int	monitor_philo(t_philo *philo, t_rule *rules, long cur_time)
{
	long	last_meal;
	int		end;

	end = 0;
	is_philo_satiated(rules, philo);
	sem_wait(philo->data_lock);
	last_meal = philo->last_meal;
	sem_post(philo->data_lock);
	if (cur_time - last_meal > rules->time_to_die)
	{
		print_action(philo, "%ld %d died\n", philo->id);
		sem_wait(rules->sems[SEM_PRINT_LOCK_ID]);
		sem_post(rules->sems[SEM_DEATH_SIGNAL_ID]);
		return (1);
	}
	sem_wait(philo->data_lock);
	end = philo->simulation_end;
	sem_post(philo->data_lock);
	if (end)
	{
		sem_wait(rules->sems[SEM_PRINT_LOCK_ID]);
		return (1);
	}
	return (0);
}

void	terminate_simulation(t_philo *philo, t_rule *rules)
{
	sem_wait(philo->data_lock);
	philo->simulation_end = 1;
	sem_post(philo->data_lock);
	ft_usleep(10);
	sem_post(rules->sems[SEM_PRINT_LOCK_ID]);
}

void	*routine_monitor(void *arg)
{
	t_philo	*philo;
	t_rule	*rules;
	long	cur_time;
	int		end;

	philo = (t_philo *)arg;
	rules = philo->rules;
	sem_wait(philo->data_lock);
	philo->last_meal = get_current_time() - rules->start_time;
	sem_post(philo->data_lock);
	end = 0;
	while (!end)
	{
		cur_time = get_current_time() - rules->start_time;
		end = monitor_philo(philo, rules, cur_time);
		if (cur_time > rules->max_duration)
		{
			sem_post(rules->sems[SEM_TIMEOUT_SIGNAL_ID]);
			break ;
		}
		usleep(500);
	}
	terminate_simulation(philo, rules);
	return (NULL);
}
