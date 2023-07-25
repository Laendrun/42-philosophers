/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 13:04:41 by saeby             #+#    #+#             */
/*   Updated: 2023/01/15 10:18:40 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ph_error(char *s)
{
	ft_putstr_fd(s, 2);
	return (1);
}

void	ph_log(int time, char *message, t_philo *philo)
{
	pthread_mutex_lock(philo->stop_m);
	if (!*philo->stop)
		printf("%d %d %s\n", time, philo->ph_id, message);
	pthread_mutex_unlock(philo->stop_m);
}

void	ph_free(t_philo *philos, int nbr)
{
	int	i;

	i = 0;
	while (i < nbr)
	{
		pthread_mutex_destroy(philos[i].fork_m);
		free(philos[i].fork_m);
		i++;
	}
	free(philos);
}

int	ph_terminate(t_philo *philos, pthread_t *threads, int nbr)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(philos[0].stop_m);
	if (threads)
	{
		while (i < nbr)
		{
			pthread_join(threads[i], NULL);
			i++;
		}
		free(threads);
	}
	if (philos)
		ph_free(philos, nbr);
	return (1);
}

int	ph_time(void)
{
	struct timeval	time;
	static int		t = 0;

	gettimeofday(&time, NULL);
	if (t == 0)
		t = time.tv_sec;
	return (((time.tv_sec - t) * 1000) + (time.tv_usec / 1000));
}
