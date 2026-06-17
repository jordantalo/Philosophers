/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:48:03 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 11:14:19 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>

# include "structs.h"

# define MAX_ARGS 5
# define MIN_ARGS 4
# define MAX_PHILOS 200
# define MAX_DURATION 10

//Fonctions to check the input and populate both rules and philo structures
int		check_input(int ac, char **av, t_rule *rules, t_supervising *s);
int		no_secure_input(t_rule *rules);
t_philo	*create_philos(t_rule *rules);

//Launch the simulation
int		init_start_time(t_rule *rules);
int		simulation(t_rule *rules, t_philo *philos);

//Open all semaphores needed to protect against race conditions
int		open_all_sems(t_rule *rules, t_philo *philos);

//Forks children process so simulatite philos
int		philo_process(t_philo *philo);
int		create_philo_threads(t_philo *philo);
int		join_philo_threads(t_philo *philo);
void	*routine_monitor(void *arg);
void	*routine_philo(void *arg);
void	*end_routine(void *arg);

//Supervisision of all children process by the parent process
int		supervising_philo_events(t_rule *rules);

//Close and unlink all the semaphores at the end of the simulation
int		close_all_sems(t_rule *rules, t_philo *philos, int do_unlink);
int		close_global_sems(t_rule *rules, int to_close, int do_unlink);
int		close_local_sems(t_philo *philo, int do_unlink);

//Utils functions
long	get_current_time(void);
void	ft_usleep(long wait_mls);
void	print_action(t_philo *philo, char *action, int id);
void	unlock_all_threads(t_rule *rules);

#endif
