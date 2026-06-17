/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:28:02 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/16 11:47:12 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	special_case(t_rule *rules)
{
	long	cur_time;

	cur_time = get_current_time() - rules->start_time;
	if (cur_time == -1)
		return (1);
	printf("%ld 1 has taken a fork\n", cur_time);
	ft_usleep(rules->time_to_die);
	cur_time = get_current_time() - rules->start_time;
	if (cur_time == -1)
		return (1);
	printf("%ld 1 died\n", cur_time);
	return (0);
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
	int		is_simulation_end;
	long	timestamp;

	is_simulation_end = 0;
	timestamp = get_current_time() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->display_mutex);
	pthread_mutex_lock(&philo->rules->end_mutex);
	is_simulation_end = philo->rules->simulation_end;
	pthread_mutex_unlock(&philo->rules->end_mutex);
	if (!is_simulation_end)
		printf(action, timestamp, id);
	pthread_mutex_unlock(&philo->rules->display_mutex);
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
