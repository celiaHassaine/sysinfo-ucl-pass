//Interface sémaphore
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include "semaphore_ttas.h"

// Functions from test and test and set.c

sem *sema;


void init_lock(int volatile *verrou){
    *verrou = 0;
}

//Fonction lock
void lock(int volatile *verrou){
    int t=1;
    while(t == 1){
        while(*verrou==1){};
        asm volatile ("movl $1, %%eax;"
        "xchgl %%eax, %1;"
        "movl %%eax, %0"
        :"=m" (t)
        :"m" (verrou)   
        :"%eax"
        );
    }
}

//Fonction unlock
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
    //printf("%ls\n", test);
	lock(test);
	--s->val;
	unlock(test);
}


void semaphore_post(sem *s){
    int volatile *test = s->verrou;
    lock(test);
    ++s->val; //Incrémente la valeur du sémaphore
    unlock(test);
}



void semaphore_destroy(sem *s){
    /*int volatile *test = s->verrou;
    free(&test);*/
    free(s);
}

/*
int N; //Nombre de threads

void test_sem(void){
	
	int count = 0;
	
	while(count < 6400/N){
        //printf("début sem value: %d\n", sema->val);
		semaphore_post(sema);
        //printf("1 post sem value: %d\n", sema->val);
		semaphore_wait(sema);
        //printf("1 post + 1 wait sem value: %d\n", sema->val);
		while(rand() > RAND_MAX/10000);
	    count ++;
		semaphore_post(sema);
        //printf("1 post + 1 wait + 1 post sem value: %d\n", sema->val);
		semaphore_wait(sema);
        //printf("1 post + 1 wait + 1 post + 1 wait sem value: %d\n", sema->val);
    }
}

void main(int argc, char *argv[]){
    printf("HERE\n");
	N = atoi(argv[1]);
	pthread_t threads[N]; //ON crée la tableeau de threads
    printf("trying malloc\n");
    sema=malloc(sizeof(sem));
    (*sema).verrou = malloc(sizeof(int volatile));
    semaphore_init(sema, 0);
    printf("%d\n%d\n", sema->val, *(sema->verrou));
	for(int i = 0; i<N; i++){
		pthread_create(&threads[i],NULL,(void *) test_sem,NULL);
		}
	for(int i = 0; i<N; i++){
		pthread_join(threads[i],NULL);
	}
    semaphore_destroy(sema);
}*/
