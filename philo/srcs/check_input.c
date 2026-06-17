/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 13:45:46 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/16 19:17:56 by jtalobre         ###   ########.fr       */
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

int	complete_rules(ssize_t input[MAX_ARGS], t_rule *rules)
{
	rules->num_of_philos = (int)input[0];
	rules->time_to_die = (long)input[1];
	rules->time_to_eat = (long)input[2];
	rules->time_to_sleep = (long)input[3];
	rules->num_of_meals = (int)input[4];
	rules->simulation_end = 0;
	rules->satiated_count = 0;
	rules->max_duration = MAX_DURATION * 1000;
	rules->forks = malloc(rules->num_of_philos * sizeof(pthread_mutex_t));
	if (!rules->forks)
		return (1);
	return (0);
}

int	check_input(int ac, char **av, t_rule *rules)
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
	if (complete_rules(input, rules))
		return (2);
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
		if (i == 0)
			philos[i].left_fork = rules->num_of_philos - 1;
		else
			philos[i].left_fork = i - 1;
		philos[i].right_fork = i;
		philos[i].is_satiated = 0;
		philos[i].rules = rules;
		i++;
	}
	return (philos);
}
