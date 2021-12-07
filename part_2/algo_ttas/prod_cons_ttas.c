#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "semaphore_ttas.h"

// Initialisation
#define MIN_INT -2147483648
#define MAX_INT 2147483647
#define N 8

#define true 1
#define false 0

sem *empty;
sem *full;
int BUFFER[N];

int number_prod = 0;
int number_cons = 0;
pthread_mutex_t verrou;

void error(int err, char *msg)
{
	fprintf(stderr, "%s a retourné %d, message d'erreur : %s\n", msg, err, strerror(errno));
	exit(EXIT_FAILURE);
}

// Producer
void *producer()
{
	int item;
	int index_write = 0;
	while (true)
	{
		pthread_mutex_lock(&verrou);
		if (number_prod == 1024)
		{
			pthread_mutex_unlock(&verrou);
			semaphore_post(empty);
			break;
		}
		pthread_mutex_unlock(&verrou);
		item = MIN_INT + rand() % (MAX_INT - MIN_INT + 1);
		semaphore_wait(empty); // waiting for a free spot
		pthread_mutex_lock(&verrou);
		// section critique
		if (number_prod == 1024)
		{
			pthread_mutex_unlock(&verrou);
			semaphore_post(empty);
			break;
		}
		BUFFER[index_write] = item;
		index_write = (index_write + 1) % 8;
		number_prod++;
		pthread_mutex_unlock(&verrou);
		semaphore_post(full); // signal that something is ready to be consumed
	}
}

// Consumer
void *consumer()
{
	int item;
	int index_read = 0;
	while (true)
	{
		pthread_mutex_lock(&verrou);
		if (number_cons == 1024) //checking if we arrive at the end of the algo
		{
			pthread_mutex_unlock(&verrou);
			semaphore_post(full);
			break;
		}
		pthread_mutex_unlock(&verrou);
		semaphore_wait(full); // waiting for something to consume
		pthread_mutex_lock(&verrou);
		// section critique
		if (number_cons == 1024)
		{
			pthread_mutex_unlock(&verrou);
			semaphore_post(full);
			break;
		}
		while (rand() > RAND_MAX / 10000)
			;
		index_read = (index_read + 1) % 8;
		number_cons++;
		pthread_mutex_unlock(&verrou);
		semaphore_post(empty); // signal one spot is free
	}
}

int main(int argc, char **argv)
{
	int err;

	int nbr_producteur = atoi(argv[1]);
	int nbr_consommateur = atoi(argv[2]);

	pthread_t threads_producteur[nbr_producteur];
	pthread_t threads_consommateur[nbr_consommateur];

	int arg_producteur[nbr_producteur];
	int arg_consommateur[nbr_consommateur];

	err = pthread_mutex_init( &verrou, NULL);
      if(err !=0){
        error(err,"pthread_mutex_init");
      }

    empty=malloc(sizeof(sem));
    (*empty).verrou = malloc(sizeof(int volatile));
    full=malloc(sizeof(sem));
    (*full).verrou = malloc(sizeof(int volatile));

	semaphore_init(empty, N);
	semaphore_init(full, 0);

	//threads consumer
	for (long i = 0; i < nbr_consommateur; i++)
	{
		arg_consommateur[i] = i;
		err = pthread_create(&(threads_consommateur[i]), NULL, consumer, NULL);
		if (err != 0)
			error(err, "pthread_create_consom");
	}

	//threads producer
	for (long i = 0; i < nbr_producteur; i++)
	{
		arg_producteur[i] = i;
		err = pthread_create(&(threads_producteur[i]), NULL, producer, NULL);
		if (err != 0)
			error(err, "pthread_create_producer");
	}

	// joining threads producer
	for (int i = 0; i < nbr_producteur; i++)
	{
		err = pthread_join(threads_producteur[i], NULL);

		if (err != 0)
			error(err, "pthread_join");
	}

	// joining threads consumer
	for (int i = 0; i < nbr_consommateur; i++)
	{

		err = pthread_join(threads_consommateur[i], NULL);

		if (err != 0)
			error(err, "pthread_join");
	}

	//destroying sem and mutex
	semaphore_destroy(empty);

    err = pthread_mutex_destroy(&verrou);
    if(err!=0)
	      error(err,"pthread_mutex_destroy");

	semaphore_destroy(full);

	return (EXIT_SUCCESS);
}
