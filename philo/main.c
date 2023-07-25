/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 19:56:56 by saeby             #+#    #+#             */
/*   Updated: 2023/01/14 15:42:32 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_philo			*philos;
	pthread_t		*threads;
	int				stop;
	int				goal_reached;
	pthread_mutex_t	stop_m;

	stop = 0;
	goal_reached = 0;
	pthread_mutex_init(&stop_m, NULL);
	if (ac < 5 || ft_atoi(av[1]) == 0 || ac > 6)
		return (ph_error("Wrong number of arguments."));
	philos = malloc(ft_atoi(av[1]) * sizeof(t_philo));
	if (!philos)
		return (ph_terminate(philos, NULL, ft_atoi(av[1])));
	if (ph_init(philos, av, ac, &stop))
		return (ph_terminate(philos, NULL, ft_atoi(av[1])));
	ph_set_times(philos, av);
	ph_set_goal(philos, &goal_reached, &stop_m);
	threads = malloc(ft_atoi(av[1]) * sizeof(pthread_t));
	if (!threads)
		return (ph_terminate(philos, threads, ft_atoi(av[1])));
	if (ph_t_init(threads, ft_atoi(av[1]), philos))
		return (ph_terminate(philos, threads, ft_atoi(av[1])));
	ph_terminate(philos, threads, ft_atoi(av[1]));
	return (0);
}
