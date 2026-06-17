/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 13:43:02 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/16 19:21:33 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <pthread.h>

typedef struct s_rule
{
	int				num_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_of_meals;
	int				simulation_end;
	int				satiated_count;
	long			start_time;
	long			max_duration;
	pthread_t		monitor_thread;
	pthread_mutex_t	*forks;
	pthread_mutex_t	display_mutex;
	pthread_mutex_t	end_mutex;
}			t_rule;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				meals_eaten;
	int				is_satiated;
	long			last_meal;
	int				left_fork;
	int				right_fork;
	pthread_mutex_t	data_mutex;
	t_rule			*rules;
}			t_philo;

#endif
