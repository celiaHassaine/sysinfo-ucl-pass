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

// Producteur
void *producer()
{
	int item;
	int index_write = 0;
	while (true)
	{
		pthread_mutex_lock(&verrou);
		// section critique
		if (number_prod == 1024)
		{
			printf("PROD: \tdans le IF: %d\n", number_prod);
			pthread_mutex_unlock(&verrou);
			semaphore_post(empty);
			break;
		}
		pthread_mutex_unlock(&verrou);
		item = MIN_INT + rand() % (MAX_INT - MIN_INT + 1);
		printf("EMPTY_PROD: \ttesting before wait %d, nbr producteur: %d\n", empty->val, number_prod);
		semaphore_wait(empty); // attente d'une place libre
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
		//printf("CONSUMMER: %d\n", number_cons);
		pthread_mutex_lock(&verrou);
		// section critique
		if (number_cons == 1024)
		{
			printf("CONS: \tdans le IF: %d\n", number_cons);
			pthread_mutex_unlock(&verrou);
			semaphore_post(full);
			break;
		}
		pthread_mutex_unlock(&verrou);
		printf("FULL_cons: \ttesting before wait %d, nbr cons: %d\n", full->val, number_cons);
		semaphore_wait(full); // attente d'une place remplie
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
		semaphore_post(empty); // il y a une place libre en plus
	}
}

int main(int argc, char **argv)
{
	int err_consommateur;
	int err_producteur;
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

	semaphore_init(empty, N); // buffer vide
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

    err = pthread_mutex_destroy(&verrou);
    if(err!=0)
	      error(err,"pthread_mutex_destroy");

	semaphore_destroy(full);
	printf("END\n");

	return (EXIT_SUCCESS);
}
