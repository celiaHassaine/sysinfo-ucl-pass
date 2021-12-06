#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "semaphore_tas.h"

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
int volatile verrou;

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
		lock(&verrou);
		// section critique
		if (number_prod == 1024)
		{
			unlock(&verrou);
			semaphore_post(empty);
			return NULL;
		}

		unlock(&verrou);
		item = MIN_INT + rand() % (MAX_INT - MIN_INT + 1);
		semaphore_wait(empty); // attente d'une place libre
		lock(&verrou);
		// section critique
		if (number_prod == 1024)
		{
			unlock(&verrou);
			semaphore_post(empty);
			return NULL;
		}
		BUFFER[index_write] = item;
		index_write = (index_write + 1) % 8;
		number_prod++;
		unlock(&verrou);
		semaphore_post(full); // il y a une place remplie en plus
	}
}

// Consomateur
void *consumer()
{
	int item;
	int index_read = 0;
	while (true)
	{
		lock(&verrou);
		// section critique
		if (number_cons == 1024)
		{
			unlock(&verrou);
			semaphore_post(full);
			return NULL;
		}
		unlock(&verrou);
		semaphore_wait(full); // attente d'une place remplie
		lock(&verrou);
		// section critique
		if (number_cons == 1024)
		{
			unlock(&verrou);
			semaphore_post(full);
			return NULL;
		}
		while (rand() > RAND_MAX / 10000)
			;
		index_read = (index_read + 1) % 8;
		number_cons++;
		unlock(&verrou);
		semaphore_post(empty); // il y a une place libre en plus
	}
}

int main(int argc, char **argv)
{
	printf("hi\n");
	int err_consommateur;
	int err_producteur;

	int nbr_producteur = atoi(argv[1]);
	int nbr_consommateur = atoi(argv[2]);

	pthread_t threads_producteur[nbr_producteur];
	pthread_t threads_consommateur[nbr_consommateur];

	int arg_producteur[nbr_producteur];
	int arg_consommateur[nbr_consommateur];
	printf("hi\n");

	init_lock(&verrou);

	semaphore_init(empty, 0); // buffer vide
	semaphore_init(full, 0);	// buffer vide

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
		printf("hi from inside\n");

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

	semaphore_destroy(empty);

	semaphore_destroy(full);

	return (EXIT_SUCCESS);*/
}
