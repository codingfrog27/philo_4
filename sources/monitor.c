/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/29 13:09:22 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/08/29 13:09:22 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	monitor_philos(t_data *data)
{
	int	i;
	int	full_philos;

	i = 0;
	full_philos = 0;
	usleep(1000);
	while (1)
	{
		while (i < data->nbr_of_philos)
		{
			if (!is_alive(data->philo_arr[i], true, &full_philos))
				return (false);
			i++;
		}
		if (full_philos == data->nbr_of_philos)
			return (true);
		i = 0;
		full_philos = 0;
		usleep(1000);
	}
}

bool	is_alive(t_philo *philo, bool in_monitor, int *full_philos)
{
	bool	alive;

	alive = true;
	pthread_mutex_lock(philo->philo_lock);
	if (time_since_x(philo->last_mealtime) > philo->data->time_till_death)
	{
		alive = false;
		pthread_mutex_lock(philo->data->print_lock);
		if (philo->data->all_alive)
		{
			printf("%li %i died\n", time_since_start(philo->data), \
			philo->id);
			philo->data->all_alive = false;
			pthread_mutex_unlock(philo->data->print_lock);
		}
	}
	if (in_monitor)
	{
		if (philo->meals_eaten >= philo->data->meals_needed && \
				philo->data->meals_needed != -1)
			*full_philos = *full_philos + 1;
	}
	pthread_mutex_unlock(philo->philo_lock);
	return (alive);
}

void	kill_everyone(t_data *data)
{
	pthread_mutex_lock(data->print_lock);
	data->all_alive = false;
	pthread_mutex_unlock(data->print_lock);
}
