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
static bool	drop_forks(t_philo *philo);
/* enable me for fun colours!
bool	fun_philo_print(t_philo *philo, t_msg_types msg_type)
{
	static const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW, \
									C_ORANGE, C_PINK};
	static const char	*msgs[] = {C_PURPLE"is pondering their orb", \
						C_VIOLET"has taken a fork", C_ROSE"is eating", \
						C_CYAN"is sleeping"};
	bool				alive;

	pthread_mutex_lock(philo->data->print_lock);
	alive = philo->data->all_alive;
	if (alive)
		printf("%li %s%i %s%s\n", time_since_start(philo->data), \
		colours[philo->id % 6], philo->id, msgs[msg_type], C_RESET);
	pthread_mutex_unlock(philo->data->print_lock);
	return (alive);
}
*/

bool	philo_print(t_philo *philo, t_msg_types msg_type)
{
	static const char	*msgs[] = {"died", "is thinking", \
						"has taken a fork", "is eating", \
						"is sleeping"};
	bool				alive;

	pthread_mutex_lock(philo->data->print_lock);
	alive = philo->data->all_alive;
	if (alive)
		printf("%li %i %s\n", time_since_start(philo->data), \
		philo->id, msgs[msg_type]);
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
		philo_print(philo, thinking);
		philo_sleep(philo->data->eat_time / 2, philo);
	}
	else
		philo_print(philo, thinking);
	while (1)
	{
		if (!mealtime(philo))
			return (NULL);
		if (!philo_print(philo, sleeping))
			return (NULL);
		if (!philo_sleep(philo->data->sleep_time, philo))
			return (NULL);
		if (!philo_print(philo, thinking))
			return (NULL);
	}
	return (NULL);
}

static bool	mealtime(t_philo *philo)
{
	if (philo->social_eating)
		politely_waiting(philo);
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
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_mealtime = timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->philo_lock);
	if (!philo_sleep(philo->data->eat_time, philo))
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

void	politely_waiting(t_philo *philo)
{
	long	lifetime;
	long	start_time;
	long	wait_time;
	long	meals_eaten;

	pthread_mutex_lock(&philo->philo_lock);
	lifetime = philo->data->death_time - time_since_x(philo->last_mealtime);
	meals_eaten = philo->meals_eaten;
	pthread_mutex_unlock(&philo->philo_lock);
	if (lifetime > philo->data->eat_time && meals_eaten > 0)
	{
		wait_time = lifetime * 0.5;
		start_time = timestamp();
		while (time_since_x(start_time) < wait_time)
			usleep(100);
	}
}
