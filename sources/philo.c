/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/21 17:54:52 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/08/21 17:54:52 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	mealtime(t_philo *philo);
static void	update_last_mealtime(t_philo *philo);
static bool	drop_forks(t_philo *philo);
// 	static const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW,
// 									C_ORANGE, C_PINK};

bool	philo_print(t_philo *philo, t_msg_types msg_type)
{
	static const char	*msgs[] = {"died", "is thinking", \
						"has taken a fork", "is eating", \
						"is sleeping"};
	bool				alive;

	pthread_mutex_lock(philo->data->print_lock);
	alive = philo->data->all_alive;
	if (alive)
	{
		printf("%li %i %s\n", time_since_start(philo->data), \
		philo->id, msgs[msg_type]);
	}
	pthread_mutex_unlock(philo->data->print_lock);
	return (alive);
}

void	*philo_routine(void *para)
{
	t_philo			*philo;

	philo = (t_philo *)para;
	pthread_mutex_lock(philo->data->print_lock);
	pthread_mutex_unlock(philo->data->print_lock);
	if (philo->id % 2)
	{
		philo_sleep(philo->data->time_to_eat / 2, philo);

	}
	while (1)
	{
		if (!philo_print(philo, thinking))
			return (NULL);
		if (!mealtime(philo))
			return (NULL);
		if (!philo_print(philo, sleeping))
			return (NULL);
		if (!philo_sleep(philo->data->sleep_time, philo))
			return (NULL);
	}
	return (NULL);
}

static bool	mealtime(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (!philo_print(philo, grabbing_fork))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (false);
	}
	pthread_mutex_lock(philo->right_fork);
	if (!philo_print(philo, grabbing_fork))
		return (drop_forks(philo));
	if (!philo_print(philo, eating))
		return (drop_forks(philo));
	update_last_mealtime(philo);
	if (!philo_sleep(philo->data->time_to_eat, philo))
		return (drop_forks(philo));
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (true);
}

static bool	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (false);
}

static void	update_last_mealtime(t_philo *philo)
{
	pthread_mutex_lock(philo->philo_lock);
	philo->last_mealtime = timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->philo_lock);
}
