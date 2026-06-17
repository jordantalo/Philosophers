/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:48:03 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/20 14:09:39 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/time.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

# include "structs.h"

# define MAX_ARGS 5
# define MIN_ARGS 4
# define MAX_PHILOS 200
# define MAX_DURATION 30

//Fonctions to check the input and populate both rules and philo structures
int		check_input(int ac, char **av, t_rule *rules);
int		no_secure_input(t_rule *rules);
t_philo	*create_philos(t_rule *rules);

//Then init mutex and threads to start simulation
int		init_philos_mutex(t_philo *philos, t_rule *rules);
int		init_forks_mutex(t_rule *rules);
int		create_philos_threads(t_philo *philos, t_rule *rules);

//Launch the simulation
int		simulate(t_rule *rules, t_philo *philos);
int		init_start_time(t_rule *rules);

//Launch routines used by philo and monitor threads
void	*routine_philo(void *arg);
void	*routine_monitor(void *arg);
int		special_case(t_rule *rules);

//Utils needed to execute routines
long	get_current_time(void);
void	ft_usleep(long wait_mls);
void	print_action(t_philo *philo, char *action, int id);

//Destroy everything at the end or in case of errors
int		destroy_philos_mutex(t_philo *philos, int to_destroy);
int		destroy_forks_mutex(t_rule *rules, int to_destroy);
int		join_philos_threads(t_philo *philos, int to_join);

#endif
