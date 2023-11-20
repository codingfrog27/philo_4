/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 17:34:42 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/07/24 17:34:42 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philatoi(char *str);
static void	assign_forks(t_data *data);
static bool	init_philo_members(t_data *data, t_philo **philos, int i);


//so sad C doesn't allow you to while loop through struct membersm, could
bool	parsing(t_data *data, char **argv, int argc)
{
	int	i;

	i = 1;
	data->nbr_of_philos = philatoi(argv[i++]);
	data->time_till_death = philatoi(argv[i++]);
	data->time_to_eat = philatoi(argv[i++]);
	data->sleep_time = philatoi(argv[i++]);
	data->full_philos = 0;
	data->meals_needed = -1;
	data->all_alive = true;
	if (argc == 6)
		data->meals_needed = philatoi(argv[i]);
	if (data->nbr_of_philos == 0 || data->time_till_death == 0 || \
	data->time_to_eat == 0 || data->sleep_time == 0 || data->meals_needed == 0)
		return (false);
	return (true);
}

/**
 * @brief just a small atoi for philo (libft not allowed)
 * since only positive nbrs are allowed for input >0 can be error return
 * So it might look like it misses checks but I think the isnum is sufficient?
 * @param str
 * @return converted int
 */
int	philatoi(char *str)
{
	long	ret;
	int		i;

	ret = 0;
	i = 0;
	while (str[i])
	{
		ret = (ret * 10) + (str[i] - '0');
		if (str[i] < '0' || str[i] > '9' || ret > INT_MAX)
			return (0);
		i++;
	}
	return ((int)ret);
}

bool	philo_init(t_data	*data)
{
	int				i;
	t_philo			**philos;

	i = 0;
	philos = malloc(sizeof(t_philo *) * (data->nbr_of_philos));
	if (!philos)
		return (false);
	while (i < data->nbr_of_philos)
	{
		if (!init_philo_members(data, philos, i))
			return (false);
		i++;
	}
	data->philo_arr = philos;
	return (true);
}

static bool	init_philo_members(t_data *data, t_philo **philos, int i)
{
	pthread_t		*thread;

	thread = malloc(sizeof(pthread_t));
	if (!thread)
		return (false);
	philos[i] = malloc(sizeof(t_philo));
	if (!philos[i])
		return (free(thread), false);
	philos[i]->full = false;
	philos[i]->thread_id = thread;
	philos[i]->id = i + 1;
	philos[i]->meals_eaten = 0;
	philos[i]->last_mealtime = 0;
	philos[i]->data = data;

	if (data->time_till_death > (data->time_to_eat * 3) \
	&& data->nbr_of_philos < 6)
		philos[i]->social_eating = true;
	else
		philos[i]->social_eating = false;

	return (true);
}

bool	init_all_mutex(t_data *data)
{
	int		i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t *) * (data->nbr_of_philos));
	if (!data->forks)
		return (false);
	// data->print_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data->print_lock, NULL);
	while (i < data->nbr_of_philos)
	{
		data->forks[i] = malloc(sizeof(pthread_mutex_t));
		data->philo_arr[i]->philo_lock = malloc(sizeof(pthread_mutex_t));
		if (!data->forks[i] || !data->philo_arr[i]->philo_lock)
			return (false);
		pthread_mutex_init(data->forks[i], NULL);
		pthread_mutex_init(data->philo_arr[i]->philo_lock, NULL); //cjecl if succeed
		// data->philo_arr[i]->left_fork = data->forks[i];
		i++;
	}
	assign_forks(data);
	return (true);
}

//last philo switches left and right to avoid potential deadlock
static void	assign_forks(t_data *data)
{
	int				i;
	t_philo			**philos;

	i = 1;
	philos = data->philo_arr;
	while (i < data->nbr_of_philos)
	{
		if (i % 2 == 1)
		{
			philos[i]->right_fork = data->forks[i - 1];
			philos[i]->left_fork = data->forks[i];
		}
		else
		{
			philos[i]->right_fork = data->forks[i];
			philos[i]->left_fork = data->forks[i - 1];
		}
		i++;
	}
	philos[0]->right_fork = data->forks[0];
	philos[0]->left_fork = data->forks[i - 1];
}
