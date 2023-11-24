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

#include "philo.h"

static void	start_simulation(t_data *data, t_routine_func routine);
static void	*single_philo_routine(void *para);

int	main(int argc, char **argv)
{
	t_data		data;
	bool		alive_and_full;

	if (argc < 5 || argc > 6 || !parsing(&data, argv, argc))
		return (printf(C_RED PARSE_ERROR));
	if (!philo_init(&data) || !init_all_mutex(&data))
		return (printf(C_RED MALLOC_ERROR));
	if (data.nbr_of_philos == 1)
		start_simulation(&data, &single_philo_routine);
	else
		start_simulation(&data, &philo_routine);
	alive_and_full = monitor_philos(&data);
	cleanup_threads_and_end(&data, alive_and_full, data.nbr_of_philos);
	return (0);
}

static void	start_simulation(t_data *data, t_routine_func routine)
{
	int	i;

	i = 0;
	pthread_mutex_lock(data->print_lock);
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(&data->philo_arr[i].thread_id, NULL, routine, \
		(void *)&data->philo_arr[i]) != 0)
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
		data->philo_arr[i].last_mealtime = data->start_time;
		i--;
	}
	pthread_mutex_unlock(data->print_lock);
}

static void	*single_philo_routine(void *para)
{
	t_philo			*philo;

	philo = (t_philo *)para;
	pthread_mutex_lock(philo->data->print_lock);
	pthread_mutex_unlock(philo->data->print_lock);
	philo_print(philo, thinking);
	pthread_mutex_lock(philo->left_fork);
	if (!philo_print(philo, grabbing_fork))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	while (1)
	{
		if (!philo_sleep(philo->data->sleep_time, philo))
			return (NULL);
	}
}
