/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 09:57:41 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/11 12:03:59 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_new_philo_satiated(t_philo *philo, int num_of_meals)
{
	int	new_philo_satiated;

	new_philo_satiated = 0;
	pthread_mutex_lock(&philo->data_mutex);
	if (!philo->is_satiated && philo->meals_eaten >= num_of_meals)
	{
		philo->is_satiated = 1;
		new_philo_satiated = 1;
	}
	pthread_mutex_unlock(&philo->data_mutex);
	return (new_philo_satiated);
}

int	is_philos_satiated(t_rule *rules, t_philo *philo)
{
	int	num_of_meals;
	int	all_satiated;

	if (rules->num_of_meals == -1)
		return (0);
	num_of_meals = rules->num_of_meals;
	all_satiated = 0;
	rules->satiated_count += is_new_philo_satiated(philo, num_of_meals);
	if (rules->satiated_count == rules->num_of_philos)
	{
		pthread_mutex_lock(&rules->display_mutex);
		all_satiated = 1;
	}
	return (all_satiated);
}

int	monitor_philos(t_philo *philos, t_rule *rules, long cur_time)
{
	int		i;
	long	last_meal;

	i = 0;
	while (i < rules->num_of_philos)
	{
		if (is_philos_satiated(rules, &philos[i]))
			return (1);
		pthread_mutex_lock(&philos[i].data_mutex);
		last_meal = philos[i].last_meal;
		pthread_mutex_unlock(&philos[i].data_mutex);
		if (cur_time - last_meal > rules->time_to_die)
		{
			pthread_mutex_lock(&rules->display_mutex);
			printf("%ld %d died\n", cur_time, philos[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*routine_monitor(void *arg)
{
	t_philo	*philos;
	t_rule	*rules;
	long	cur_time;
	int		end;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	end = 0;
	while (!end)
	{
		cur_time = get_current_time() - rules->start_time;
		end = monitor_philos(philos, rules, cur_time);
		if (cur_time > rules->max_duration)
		{
			pthread_mutex_lock(&rules->display_mutex);
			end = 1;
		}
		ft_usleep(1);
	}
	pthread_mutex_lock(&rules->end_mutex);
	rules->simulation_end = 1;
	pthread_mutex_unlock(&rules->end_mutex);
	ft_usleep(100);
	pthread_mutex_unlock(&rules->display_mutex);
	return (NULL);
}
