//Interface sémaphore
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include "semaphore_tas.h"

// Functions from test and test and set.c

sem *sema;


void init_lock(int volatile *verrou){
    *verrou = 0;
}


void lock(int volatile *verrou){
    int t=1;
    while(t == 1){
        asm volatile ("movl $1, %%eax;"
        "xchgl %%eax, %1;"
        "movl %%eax, %0"
        :"=m" (t)
        :"m" (verrou)
        :"%eax"
        );
    }
}


void unlock(int volatile *verrou){
    int t=0;
    asm ("movl $0, %%eax;"   
        "xchgl %%eax, %1;"
        :"=m" (verrou)
        :"m" (t)
        :"%eax"
    );
}



int semaphore_init(sem *s, int v){
    s->val=v;
    init_lock(s->verrou);
    return 0;
}


void semaphore_wait(sem *s){
    int volatile *test = s->verrou;
    while(s->val <= 0);
	lock(test);
	--s->val;
	unlock(test);
}


void semaphore_post(sem *s){
    int volatile *test = s->verrou;
    lock(test);
    ++s->val; 
    unlock(test);
}



void semaphore_destroy(sem *s){
    int volatile *test = s->verrou;
    free((void *)test);
    free(s);
}