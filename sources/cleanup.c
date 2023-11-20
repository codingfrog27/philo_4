/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/25 16:35:47 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/10/25 16:35:47 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	free_data(t_data *data);

//sleeps for a moment to let all threads finish exiting
void	cleanup_threads_and_end(t_data *data, bool full, int nbr)
{
	int	i;

	i = 0;
	if (nbr < data->nbr_of_philos)
		pthread_mutex_unlock(data->print_lock);
	if (full)
		kill_everyone(data);
	while (i < nbr)
	{
		pthread_join(*data->philo_arr[i]->thread_id, NULL);
		i++;
	}
	free_data(data);
	usleep(1000);
	if (full)
		printf("EVERYONE FULL YIPPIE\n");
}

static void	free_data(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	pthread_mutex_destroy(data->print_lock);
	free(data->print_lock);
	while (i < data->nbr_of_philos)
	{
		philo = data->philo_arr[i];
		pthread_mutex_destroy(philo->philo_lock);
		if (philo->id % 2 == 1)
			pthread_mutex_destroy(philo->left_fork);
		else
			pthread_mutex_destroy(philo->right_fork);
		// free(philo->left_fork);
		// free(philo->philo_lock);
		// free(philo->thread_id);
		free(philo);
		i++;
	}
	free(data->philo_arr);
	free(data->forks);
}
