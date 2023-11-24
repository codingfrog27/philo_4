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

bool	parsing(t_data *data, char **argv, int argc)
{
	int	i;

	i = 1;
	data->nbr_of_philos = philatoi(argv[i++]);
	data->death_time = philatoi(argv[i++]);
	data->eat_time = philatoi(argv[i++]);
	data->sleep_time = philatoi(argv[i++]);
	data->full_philos = 0;
	data->meals_needed = -1;
	data->all_alive = true;
	data->print_lock = NULL;
	data->philo_arr = NULL;
	data->forks = NULL;
	if (argc == 6)
		data->meals_needed = philatoi(argv[i]);
	if (data->nbr_of_philos == 0 || data->death_time == 0 || \
	data->eat_time == 0 || data->sleep_time == 0 || data->meals_needed == 0)
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
	t_philo			*philos;

	i = 0;
	philos = malloc(sizeof(t_philo) * (data->nbr_of_philos));
	if (!philos)
		return (false);
	while (i < data->nbr_of_philos)
	{
		philos[i].full = false;
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_mealtime = 0;
		philos[i].data = data;
		if (data->death_time > (data->eat_time * 3) \
		&& data->nbr_of_philos < 6)
			philos[i].social_eating = true;
		else
			philos[i].social_eating = false;
		i++;
	}
	data->philo_arr = philos;
	return (true);
}

bool	init_all_mutex(t_data *data)
{
	int		i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * (data->nbr_of_philos));
	data->print_lock = malloc(sizeof(pthread_mutex_t));
	if (!data->forks || !data->print_lock)
		return (error_cleanup(data));
	if (pthread_mutex_init(data->print_lock, NULL) != 0)
		return (error_cleanup(data));
	while (i < data->nbr_of_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0 || \
			pthread_mutex_init(&data->philo_arr[i].philo_lock, NULL) != 0)
			return (error_cleanup(data));
		i++;
	}
	assign_forks(data);
	return (true);
}

static void	assign_forks(t_data *data)
{
	int				i;
	t_philo			*philos;

	i = 1;
	philos = data->philo_arr;
	while (i < data->nbr_of_philos)
	{
		if (i % 2 == 1)
		{
			philos[i].right_fork = &data->forks[i - 1];
			philos[i].left_fork = &data->forks[i];
		}
		else
		{
			philos[i].right_fork = &data->forks[i];
			philos[i].left_fork = &data->forks[i - 1];
		}
		i++;
	}
	philos[0].right_fork = &data->forks[0];
	philos[0].left_fork = &data->forks[i - 1];
}
