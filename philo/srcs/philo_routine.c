/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 09:57:30 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/17 08:18:27 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks(t_philo *philo)
{
	int		id;
	int		first;
	int		second;

	id = philo->id;
	if (id % 2)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	else
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	pthread_mutex_lock(&philo->rules->forks[first]);
	print_action(philo, "%ld %d has taken a fork\n", id);
	pthread_mutex_lock(&philo->rules->forks[second]);
	print_action(philo, "%ld %d has taken a fork\n", id);
	return (0);
}

int	eat_spaghetti(t_philo *philo)
{
	int		id;

	id = philo->id;
	pthread_mutex_lock(&philo->data_mutex);
	philo->last_meal = get_current_time() - philo->rules->start_time;
	pthread_mutex_unlock(&philo->data_mutex);
	print_action(philo, "%ld %d is eating\n", id);
	ft_usleep(philo->rules->time_to_eat);
	return (0);
}

int	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->rules->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->rules->forks[philo->right_fork]);
	return (0);
}

int	sleep_and_think(t_philo *philo)
{
	int	id;

	id = philo->id;
	print_action(philo, "%ld %d is sleeping\n", id);
	ft_usleep(philo->rules->time_to_sleep);
	print_action(philo, "%ld %d is thinking\n", id);
	usleep(500);
	return (0);
}

void	*routine_philo(void *arg)
{
	t_philo	*philo;
	int		id;
	int		end;

	philo = (t_philo *)arg;
	id = philo->id;
	end = 0;
	if (philo->id % 2 == 0)
		ft_usleep(philo->rules->time_to_die / 2);
	while (!end)
	{
		take_forks(philo);
		eat_spaghetti(philo);
		release_forks(philo);
		pthread_mutex_lock(&philo->data_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->data_mutex);
		sleep_and_think(philo);
		pthread_mutex_lock(&philo->rules->end_mutex);
		end = philo->rules->simulation_end;
		pthread_mutex_unlock(&philo->rules->end_mutex);
	}
	return (NULL);
}
