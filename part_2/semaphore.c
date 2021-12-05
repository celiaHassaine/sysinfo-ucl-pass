//Interface sémaphore
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include "test-and-test.c"

typedef struct semaphore
{
    int val; //valeur de la sémaphore
    int *verrou; //valeur du verrou pour faire l'attente active
}sem;


int semaphore_init(sem** s, int v){
    *s= malloc(sizeof(sem))); //Alloue de la place en mémoire pour notre sémaphore
    if(*s==NULL){
        return -1;
    }
    (*s)->val = v;
    (**s).verrou = malloc(sizeof(int));
    *((**s).verrou) = 0;
    return 0;
}


void semaphore_wait(sem* s){


}


void semaphore_post(sem* s){
    lock(s->verrou);
    s->val = s->val+1; //Incrémente la valeur du sémaphore
    unlock(s->verrou);
}



void semaphore_destroy(sem* s){
    free(s->verrou);
    free(s);
}