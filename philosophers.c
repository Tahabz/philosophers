#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


pthread_mutex_t lock;
enum States {EATING, HUNGRY, THINKING}; 

typedef struct philosophers
{
    int state;
    int s;
    int index;
} t_phils;

t_phils philos[3];

void test(int phil_index)
{
    t_phils *philo = &philos[phil_index];
    int right = phil_index + 1;
    int left = phil_index - 1;
    if (phil_index == 2)
        right = 0;
    if (phil_index == 0)
        left = 2;
    t_phils philo_right = philos[right];
    t_phils philo_left = philos[left];
    
    if (philo->state == HUNGRY && philo_left.state != EATING && philo_right.state != EATING)
    {
        philo->state = EATING;
        printf("\n philo: %d is EATING \n", phil_index);
        philo->s += 1;
    }
}

void put_forks(int phil_index)
{
    pthread_mutex_lock(&lock);

        t_phils *philo = &philos[phil_index];
        int right = phil_index + 1;
        int left = phil_index - 1;
        if (phil_index == 2)
            right = 0;
        if (phil_index == 0)
            left = 2;

        philo->state = THINKING;

        printf("\n philo: %d is THINKING \n", phil_index);
        test(left);
        test(right);

    pthread_mutex_unlock(&lock);
}

void down(int phil_index)
{
    if (philos[phil_index].s > 0) {
        philos[phil_index].s = 0;
    }
    else
    {
        while (philos[phil_index].s == 0);
        philos[phil_index].s = 1;
    }
}

void take_forks(int phil_index)
{
    pthread_mutex_lock(&lock);
    
        philos[phil_index].state = HUNGRY;
        printf("\n philo: %d is Hungry \n", phil_index);
        test(phil_index);
    pthread_mutex_unlock(&lock);
    down(phil_index);

}

void eat()
{
    int i = 10000000;
    while (i--);
}

void think()
{
    int i = 10000000;
    while (i--)
        ;
}

void *philosopher(void *i)
{
    int *phil_index = i;

    while (true)
    {
        think();
        take_forks(*phil_index);
        eat();
        put_forks(*phil_index);
    }
    return i;
}

void init_philos()
{
    philos[0].state = THINKING;
    philos[1].state = THINKING;
    philos[2].state = THINKING;

    philos[0].index = 0;
    philos[1].index = 1;
    philos[2].index = 2;
    
    philos[0].s = 0;
    philos[1].s = 0;
    philos[2].s = 0;
}

int main(void)
{
    int error;
    pthread_t tid[3];
    int i = 0;
    init_philos();
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }
    while (i < 3) {
        error = pthread_create(&(tid[i]), NULL, &philosopher, &philos[i].index);
        if (error != 0)
            printf("\n Thread can't be created : [%s] \n", strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL); 
    pthread_join(tid[2], NULL); 

    pthread_mutex_destroy(&lock);

    return (0);
}