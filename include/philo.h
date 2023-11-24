/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/09 12:55:20 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/02/07 14:59:51 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "h_colors.h"
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>

# define PARSE_ERROR "ERROR, input could not be parsed. Make sure to give 5 or \
6 positive interger args\n"

# define SINGLE_PHILO_ERR "Single philo dies sad and \
 alone without ever eating 😭\n"

# define MALLOC_ERROR "ERROR occured when allocating memory\n"

typedef struct s_data	t_data;

typedef struct s_philo
{
	bool			full;
	int				id;
	int				meals_eaten;
	long			last_mealtime;
	long			start_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*philo_lock;
	pthread_t		thread_id;
	t_data			*data;
	bool			social_eating;
}				t_philo;

typedef enum e_msg_types
{
	death,
	thinking,
	grabbing_fork,
	eating,
	sleeping,
}	t_msg_types;

//also using mealcheck to see if philo is alive + as start lock?
struct s_data
{
	long			nbr_of_philos;
	long			time_till_death;
	long			time_to_eat;
	long			sleep_time;
	long			meals_needed;
	long			full_philos;
	long			start_time;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*forks;
	t_philo			**philo_arr;
	bool			all_alive;
};

//init
bool	parsing(t_data *data, char **argv, int argc);
bool	init_all_mutex(t_data *data);
bool	philo_init(t_data	*data);

//core
void	*philo_routine(void *para);
bool	philo_print(t_philo *philo, t_msg_types msg_type);
bool	monitor_philos(t_data *data);
bool	is_alive(t_philo *philo, bool in_monitor, int *full_philos);
//cleanup
void	cleanup_threads_and_end(t_data *data, bool full, int nbr);
void	kill_everyone(t_data *data);

//time
long	time_since_x(long start);
long	time_since_start(t_data *data);
long	timestamp(void);
bool	philo_sleep(long sleep_time, t_philo *philo);
#endif // !PHILO_H
