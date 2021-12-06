#include <math.h>
#ifndef SEMAPHORE_TAS_H
#define SEMAPHORE_TAS_H

void init_lock(int volatile *verrou);
void lock(int volatile *verrou);
void unlock(int volatile *verrou);

typedef struct semaphore
{
    int val; //valeur de la sémaphore
    int volatile *verrou; //valeur du verrou pour faire l'attente active
}sem;

int semaphore_init(sem *s, int v);
void semaphore_wait(sem *s);
void semaphore_post(sem *s);
void semaphore_destroy(sem *s);


#endif