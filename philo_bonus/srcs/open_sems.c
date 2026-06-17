/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_sems.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtalobre <jtalobre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 15:21:06 by jtalobre          #+#    #+#             */
/*   Updated: 2025/12/15 10:05:19 by jtalobre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	open_global_sems(t_rule *r)
{
	int	i;

	i = 0;
	while (i < N_SEMS)
	{
		sem_unlink(r->s_names[i]);
		r->sems[i] = sem_open(r->s_names[i], O_CREAT | O_EXCL, 0644,
				r->s_init_val[i]);
		if (r->sems[i] == SEM_FAILED)
		{
			printf("Failed to open sems: %s\n", r->s_names[i]);
			close_global_sems(r, i, 1);
			return (1);
		}
		i++;
	}
	return (0);
}

void	append_id(int id, int i, char sem_name[32])
{
	int		j;
	char	buf[10];

	j = 0;
	if (id == 0)
		buf[j++] = '0';
	while (id != 0)
	{
		buf[j++] = '0' + id % 10;
		id = id / 10;
	}
	while (j > 0)
	{
		sem_name[i++] = buf[j - 1];
		j--;
	}
	sem_name[i] = '\0';
}

void	join_sem_name(char sem_name[32], char *base_name, int id)
{
	int	i;

	i = 0;
	while (base_name[i])
	{
		sem_name[i] = base_name[i];
		i++;
	}
	append_id(id, i, sem_name);
}

int	open_local_sems(t_philo *p)
{
	join_sem_name(p->s_name, p->base_name, p->id);
	sem_unlink(p->s_name);
	p->data_lock = sem_open(p->s_name, O_CREAT | O_EXCL, 0644, p->s_init_val);
	if (p->data_lock == SEM_FAILED)
	{
		printf("Failed to open sems: %s\n", p->s_name);
		close_local_sems(p, 1);
		return (1);
	}
	return (0);
}

int	open_all_sems(t_rule *rules, t_philo *philos)
{
	int	i;
	int	j;

	i = 0;
	if (open_global_sems(rules))
		return (1);
	while (i < rules->num_of_philos)
	{
		if (open_local_sems(&philos[i]))
		{
			j = 0;
			while (j < i)
			{
				close_local_sems(&philos[j], 1);
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}
