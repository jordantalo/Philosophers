/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:47:02 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 08:58:29 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_philo			*philos;
	t_supervising	supervisor;
	t_rule			rules;
	int				res;

	res = 0;
	if (ac < MIN_ARGS + 1 || ac > MAX_ARGS + 1)
		return (1);
	res = check_input(ac, av, &rules, &supervisor);
	if (res)
		return (printf("Error\n"), res);
	if (no_secure_input(&rules))
		return (1);
	philos = create_philos(&rules);
	if (!philos)
		return (1);
	if (simulation(&rules, philos))
		res = 1;
	free(philos);
	return (res);
}
