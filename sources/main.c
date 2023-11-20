/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 17:33:52 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/07/24 17:33:52 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// mem management notes
// on heap = philos, fork array, pthread ptrs all locks
// forks are a mutex array in data since it saves me from having to allocate
// each fork seperately and they should only be used by the philo threads
// anyways && // currently not freeing old data in parsing if malloc fails
// cuz im exiting anyways

#include "philo.h"

void	start_simulation(t_data *data);

int	main(int argc, char **argv)
{
	t_data		data;
	bool		all_alive;

	if (argc < 5 || argc > 6 || !parsing(&data, argv, argc))
		return (printf(C_RED PARSE_ERROR));
	if (data.nbr_of_philos == 1)
		return (printf(SINGLE_PHILO_ERR));
	if (!philo_init(&data) || !init_all_mutex(&data))
		return (printf(C_RED MALLOC_ERROR));
	start_simulation(&data);
	all_alive = monitor_philos(&data);
	cleanup_threads_and_end(&data, all_alive, data.nbr_of_philos);
	return (0);
}

void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(data->print_lock);
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(data->philo_arr[i]->thread_id, NULL, philo_routine, \
		(void *)data->philo_arr[i]) != 0)
		{
			printf("ERROR: PTHREAD CREATE FAILURE\n");
			cleanup_threads_and_end(data, false, i - 1);
		}
		i++;
	}
	i--;
	data->start_time = timestamp();
	while (i >= 0)
	{
		data->philo_arr[i]->last_mealtime = data->start_time;
		i--;
	}
	pthread_mutex_unlock(data->print_lock);
}
