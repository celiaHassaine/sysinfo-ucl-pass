#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define CYCLES 100000

pthread_t *phil;
pthread_mutex_t *baguette;
int PHILOSOPHES;
__thread int nbCycles = 0;

void error(int err, char *msg) { 
  fprintf(stderr,"%s a retourné %d, message d'erreur : %s\n",msg,err,strerror(errno));
  exit(EXIT_FAILURE);
}


void mange(int id) {
  //philosophe mange
}

void pense(int id){
  //philosophe pense
}

void* philosophe ( void* arg ){
    
    int *id=(int *) arg;
    //Baguette à gauche 
    int left = *id;
    //Baguette à droite
    int right = (left + 1) % PHILOSOPHES;
    if(PHILOSOPHES==1){
      right =1;
    }

    //Boucle avec le nombre de cycle que l'on veut
    while(nbCycles <= 100000){
        pense(*id);
        //Pour que les philosophes considérent les mutex dans le même ordre
        if(left < right){
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }
        else{
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        mange(*id);
        nbCycles++;
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}

int main(int argc, char *argv[]){
    
    PHILOSOPHES = atoi(argv[1]);

    int nbBaguette = PHILOSOPHES;

    if(PHILOSOPHES ==1){
      nbBaguette=2;
    }

    phil = malloc(PHILOSOPHES*sizeof(pthread_t));
    baguette = malloc(sizeof(pthread_mutex_t)*nbBaguette);

    int id[PHILOSOPHES];
    int err;

    for (int i = 0; i < PHILOSOPHES; i++){
      id[i]=i;
    }
    //Initialiser les mutex
    for (int i = 0; i < nbBaguette; i++){
      err = pthread_mutex_init( &baguette[i], NULL);
      if(err !=0){
        error(err,"pthread_mutex_init");
      }
    }
    //Initialiser les threads 
    for (int i = 0; i < PHILOSOPHES; i++){
        err = pthread_create(&phil[i], NULL, philosophe, (void*)&(id[i]));    
        if(err != 0 ){
          error(err,"pthread_create");
        }
    }

    for (int i = 0; i < PHILOSOPHES; i++){
        err = pthread_join(phil[i], NULL);
        if(err != 0 ){
          error(err, "pthread_join");
        }
    }
    //Supprimer les mutex
    for (int i = 0; i < nbBaguette; i++) {
      err = pthread_mutex_destroy(&baguette[i]);
      if(err!=0)
	      error(err,"pthread_mutex_destroy");
    }
    free(baguette);
    free(phil);
    return (EXIT_SUCCESS);
}