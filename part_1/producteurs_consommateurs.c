#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <string.h>

// Initialisation
#define MIN_INT -2147483648
#define MAX_INT 2147483647
#define N 8

#define true 1
#define false 0

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int BUFFER[N];

int number_prod = 0;
int number_cons = 0;

void error(int err, char *msg)
{
	fprintf(stderr, "%s a retourné %d, message d'erreur : %s\n", msg, err, strerror(errno));
	exit(EXIT_FAILURE);
}

// Producteur
void *producer()
{
	int item;
	int index_write = 0;
	while (true)
	{
		pthread_mutex_lock(&mutex);
		// section critique
		if (number_prod == 1024)
		{
			pthread_mutex_unlock(&mutex);
			sem_post(&empty);
			return NULL;
		}

		pthread_mutex_unlock(&mutex);
		item = MIN_INT + rand() % (MAX_INT - MIN_INT + 1);
		sem_wait(&empty); // attente d'une place libre
		pthread_mutex_lock(&mutex);
		// section critique
		if (number_prod == 1024)
		{
			pthread_mutex_unlock(&mutex);
			sem_post(&empty);
			return NULL;
		}
		BUFFER[index_write] = item;
		index_write = (index_write + 1) % 8;
		number_prod++;
		pthread_mutex_unlock(&mutex);
		sem_post(&full); // il y a une place remplie en plus
	}
}

// Consomateur
void *consumer()
{
	int item;
	int index_read = 0;
	while (true)
	{
		pthread_mutex_lock(&mutex);
		// section critique
		if (number_cons == 1024)
		{
			pthread_mutex_unlock(&mutex);
			sem_post(&full);
			return NULL;
		}
		pthread_mutex_unlock(&mutex);
		sem_wait(&full); // attente d'une place remplie
		pthread_mutex_lock(&mutex);
		// section critique
		if (number_cons == 1024)
		{
			pthread_mutex_unlock(&mutex);
			sem_post(&full);
			return NULL;
		}
		printf("\n %d \n", BUFFER[index_read]);
		while (rand() > RAND_MAX / 10000)
			;
		index_read = (index_read + 1) % 8;
		number_cons++;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty); // il y a une place libre en plus
	}
}

int main(int argc, char **argv)
{
	int err_mutex;
	int err_consommateur;
	int err_producteur;

	int nbr_producteur = atoi(argv[1]);
	int nbr_consommateur = atoi(argv[2]);

	pthread_t threads_producteur[nbr_producteur];
	pthread_t threads_consommateur[nbr_consommateur];

	int arg_producteur[nbr_producteur];
	int arg_consommateur[nbr_consommateur];

	pthread_mutex_init(&mutex, NULL);

	sem_init(&empty, 0, N); // buffer vide
	sem_init(&full, 0, 0);	// buffer vide

	err_mutex = pthread_mutex_init(&mutex, NULL);
	if (err_mutex != 0)
	{
		error(err_mutex, "pthread_read_mutex_init");
	}

	//creation des threads consommateurs
	for (long i = 0; i < nbr_consommateur; i++)
	{
		arg_consommateur[i] = i;
		err_consommateur = pthread_create(&(threads_consommateur[i]), NULL, consumer, NULL);
		if (err_consommateur != 0)
			error(err_consommateur, "pthread_create_consom");
	}

	//creation des threads producteurs
	for (long i = 0; i < nbr_producteur; i++)
	{
		arg_producteur[i] = i;
		err_producteur = pthread_create(&(threads_producteur[i]), NULL, producer, NULL);
		if (err_producteur != 0)
			error(err_producteur, "pthread_create_producer");
	}

	// attendre les threads producteurs
	for (int i = 0; i < nbr_producteur; i++)
	{

		err_producteur = pthread_join(threads_producteur[i], NULL);

		if (err_producteur != 0)
			error(err_producteur, "pthread_join");
	}

	// attendre les threads consommateurs
	for (int i = 0; i < nbr_consommateur; i++)
	{

		err_consommateur = pthread_join(threads_consommateur[i], NULL);

		if (err_consommateur != 0)
			error(err_consommateur, "pthread_join");
	}

	err_mutex = pthread_mutex_destroy(&mutex);
	if (err_mutex != 0)
	{
		error(err_mutex, "pthread_mutex_destroy");
	}

	return (EXIT_SUCCESS);
}