/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 13:43:02 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 09:33:37 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define N_SEMS 8
# define SEM_FORKS_NAME "/forks"
# define SEM_PRINT_LOCK_NAME "/print_lock"
# define SEM_SATIATED_SIGNAL_NAME "/satiated_signal"
# define SEM_DEATH_SIGNAL_NAME "/death_signal"
# define SEM_TIMEOUT_SIGNAL_NAME "/timeout_signal"
# define SEM_EAT_LOCK_NAME "/eat_lock"
# define SEM_END_NAME "/end"
# define SEM_STOP_CHILDREN_NAME "/stop_children"
# define SEM_FORKS_ID 0
# define SEM_PRINT_LOCK_ID 1
# define SEM_SATIATED_SIGNAL_ID 2
# define SEM_DEATH_SIGNAL_ID 3
# define SEM_TIMEOUT_SIGNAL_ID 4
# define SEM_END_ID 5
# define SEM_EAT_LOCK_ID 6
# define SEM_STOP_CHILDREN_ID 7
# define SEM_DATA_NAME "/data_lock"

typedef struct s_supervising
{
	pthread_t	death_thread;
	pthread_t	all_full_thread;
	pthread_t	timeout_thread;
	int			is_end;
}				t_supervising;

typedef struct s_rule
{
	int				num_of_philos;
	int				num_of_meals;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	long			max_duration;
	sem_t			*sems[N_SEMS];
	char			*s_names[N_SEMS];
	int				s_init_val[N_SEMS];
	t_supervising	*supervisor;
}			t_rule;

typedef struct s_philo
{
	pthread_t		thread_philo;
	pthread_t		thread_monitor;
	pthread_t		thread_simu_end;
	sem_t			*data_lock;
	char			s_name[32];
	char			*base_name;
	int				s_init_val;
	long			last_meal;
	int				id;
	int				meals_eaten;
	int				is_satiated;
	int				simulation_end;
	int				pid;
	t_rule			*rules;
}			t_philo;

#endif
