/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 15:55:50 by saeby             #+#    #+#             */
/*   Updated: 2023/01/14 15:29:49 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *arguments)
{
	t_args	*args;
	int		stop;

	args = (t_args *) arguments;
	if (args->ph1->ph_id % 2)
		usleep(3000);
	stop = *(args->ph1->stop);
	while (!stop)
	{
		if (!args->ph1->ph_goal)
			break ;
		ph_goal(args->ph1);
		ph_die(args->ph1);
		if (args->ph1->ph_status == 0 && !*(args->ph1->stop))
			ph_eat(args->ph1, args->ph2);
		else if (args->ph1->ph_status == 1 && !*(args->ph1->stop))
			ph_sleep(args->ph1);
		else if (args->ph1->ph_status == 2 && !*(args->ph1->stop))
			ph_think(args->ph1, args->ph2);
		pthread_mutex_lock(args->ph1->stop_m);
		stop = *(args->ph1->stop);
		pthread_mutex_unlock(args->ph1->stop_m);
	}
	free(arguments);
	return (0);
}

void	ph_die(t_philo *philo)
{
	if (ph_time() - philo->ph_die > philo->ph_last_meal)
	{
		ph_log(ph_time(), "died", philo);
		pthread_mutex_lock(philo->stop_m);
		*philo->stop = 1;
		pthread_mutex_unlock(philo->stop_m);
	}
}

void	ph_eat(t_philo *philo, t_philo *other)
{
	if (*(philo->stop))
		return ;
	ph_log(ph_time(), "is eating", philo);
	philo->ph_last_meal = ph_time();
	while (!*(philo->stop) && ph_time() < philo->ph_last_meal + philo->ph_eat)
		ph_die(philo);
	ph_drop_forks(philo, other);
	philo->ph_goal -= 1;
	philo->ph_status = 1;
}

void	ph_sleep(t_philo *philo)
{
	int	start;

	if (*(philo->stop))
		return ;
	start = ph_time();
	ph_log(ph_time(), "is sleeping", philo);
	while (!*(philo->stop) && ph_time() < start + philo->ph_sleep)
		ph_die(philo);
	philo->ph_status = 2;
}

void	ph_think(t_philo *philo, t_philo *other)
{
	if (*(philo->stop) || *(other->stop))
		return ;
	ph_log(ph_time(), "is thinking", philo);
	while (!*(philo->stop) && philo->forks < 2)
	{
		ph_take_forks(philo, other);
		ph_die(philo);
	}
	philo->ph_status = 0;
}
