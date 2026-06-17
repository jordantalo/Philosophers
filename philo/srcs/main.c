/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:47:02 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/11 14:01:47 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_philo	*philos;
	t_rule	rules;
	int		res;

	res = 0;
	if (ac < MIN_ARGS + 1 || ac > MAX_ARGS + 1)
		return (1);
	res = check_input(ac, av, &rules);
	if (res)
		return (printf("Error\n"), res);
	if (no_secure_input(&rules))
		return (free(rules.forks), 1);
	philos = create_philos(&rules);
	if (!philos)
		return (free(rules.forks), 1);
	res = simulate(&rules, philos);
	free(philos);
	free(rules.forks);
	return (res);
}
