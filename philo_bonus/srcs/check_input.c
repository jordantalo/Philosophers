/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 13:45:46 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 11:14:06 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi(char *s)
{
	int		i;
	long	res;

	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+')
		i++;
	if (s[i] < '0' || s[i] > '9')
		return (-1);
	res = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10;
		res = res + s[i] - '0';
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	if (s[i] != '\0')
		return (-1);
	return (res);
}

int	complete_rules(long input[MAX_ARGS], t_rule *rules, t_supervising *s)
{
	rules->num_of_philos = (int)input[0];
	rules->time_to_die = (long)input[1];
	rules->time_to_eat = (long)input[2];
	rules->time_to_sleep = (long)input[3];
	rules->num_of_meals = (int)input[4];
	rules->max_duration = MAX_DURATION * 1000;
	rules->s_names[SEM_FORKS_ID] = SEM_FORKS_NAME;
	rules->s_names[SEM_PRINT_LOCK_ID] = SEM_PRINT_LOCK_NAME;
	rules->s_names[SEM_SATIATED_SIGNAL_ID] = SEM_SATIATED_SIGNAL_NAME;
	rules->s_names[SEM_DEATH_SIGNAL_ID] = SEM_DEATH_SIGNAL_NAME;
	rules->s_names[SEM_TIMEOUT_SIGNAL_ID] = SEM_TIMEOUT_SIGNAL_NAME;
	rules->s_names[SEM_END_ID] = SEM_END_NAME;
	rules->s_names[SEM_EAT_LOCK_ID] = SEM_EAT_LOCK_NAME;
	rules->s_names[SEM_STOP_CHILDREN_ID] = SEM_STOP_CHILDREN_NAME;
	rules->s_init_val[SEM_FORKS_ID] = rules->num_of_philos;
	rules->s_init_val[SEM_PRINT_LOCK_ID] = 1;
	rules->s_init_val[SEM_SATIATED_SIGNAL_ID] = 0;
	rules->s_init_val[SEM_DEATH_SIGNAL_ID] = 0;
	rules->s_init_val[SEM_TIMEOUT_SIGNAL_ID] = 0;
	rules->s_init_val[SEM_END_ID] = 1;
	rules->s_init_val[SEM_EAT_LOCK_ID] = rules->num_of_philos - 1;
	rules->s_init_val[SEM_STOP_CHILDREN_ID] = 0;
	rules->supervisor = s;
	rules->supervisor->is_end = 0;
	return (0);
}

int	check_input(int ac, char **av, t_rule *rules, t_supervising *supervisor)
{
	long	input[MAX_ARGS];
	int		i;

	i = 0;
	input[MAX_ARGS - 1] = -1;
	while (i < ac - 1)
	{
		input[i] = ft_atoi(av[i + 1]);
		if (input[i] == -1)
			return (1);
		i++;
	}
	if (complete_rules(input, rules, supervisor))
		return (1);
	return (0);
}

int	no_secure_input(t_rule *rules)
{
	if (rules->num_of_philos == 0)
	{
		printf("At least 1 philosophers is needed to start a simulation\n");
		return (1);
	}
	if (rules->num_of_philos > MAX_PHILOS)
	{
		printf("Too many philosophers, retry with maximum 200 philosophers\n");
		return (1);
	}
	return (0);
}

t_philo	*create_philos(t_rule *rules)
{
	int				i;
	t_philo			*philos;

	philos = malloc(rules->num_of_philos * sizeof(t_philo));
	if (!philos)
		return (NULL);
	i = 0;
	while (i < rules->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = 0;
		philos[i].is_satiated = 0;
		philos[i].simulation_end = 0;
		philos[i].base_name = SEM_DATA_NAME;
		philos[i].s_init_val = 1;
		philos[i].rules = rules;
		i++;
	}
	return (philos);
}
