/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/10 18:02:24 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/02/08 18:45:38 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "philo.h"

long	timestamp(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long	time_since_x(long x)
{
	return (timestamp() - x);
}

long	time_since_start(t_data *data)
{
	return (timestamp() - data->start_time);
}

bool	philo_sleep(long sleep_time, t_philo *philo)
{
	int			i;
	long		start_time;
	long		nap_time;

	i = 0;
	start_time = timestamp();
	nap_time = 100 + (philo->data->nbr_of_philos * 10);
	while ((timestamp() - start_time) < sleep_time)
	{
		usleep(nap_time);
		if (i == 1)
		{
			if (!is_alive(philo, false, NULL))
				return (false);
			i = 0;
		}
		i++;
	}
	return (true);
}
