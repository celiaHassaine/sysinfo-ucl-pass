#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "semaphore_ttas.h"



pthread_t *phil;
int *baguette;
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
    while(nbCycles <= 10000){
        pense(*id);
        //Pour que les philosophes considérent les mutex dans le même ordre
        if(left < right){
            lock(baguette[left]);
            lock(baguette[right]);
        }
        else{
            lock(baguette[right]);
            lock(baguette[left]);
        }
        mange(*id);
        nbCycles++;
        unlock(baguette[left]);
        unlock(baguette[right]);
    }
    return (NULL);
}

int main(int argc, char *argv[]){
    
    PHILOSOPHES = atoi(argv[1]);
    phil = malloc(PHILOSOPHES*sizeof(pthread_t));
    int nbBaguette = PHILOSOPHES;
    if(PHILOSOPHES==1){
        nbBaguette = 2;
    }
    baguette = malloc(sizeof(int) * nbBaguette);
    int id[PHILOSOPHES];
    int err;
    for (int i = 0; i < PHILOSOPHES; i++){
      id[i]=i;
    }
    for (size_t i = 0; i < nbBaguette; i++){
        baguette[i] = 0;
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
    free(baguette);
    return (EXIT_SUCCESS);
}