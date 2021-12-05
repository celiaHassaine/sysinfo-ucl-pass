//Interface sémaphore
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include "tast_ttas_util.h>"

// Functions from test and test and set.c

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

typedef struct semaphore
{
    int val; //valeur de la sémaphore
    int volatile *verrou; //valeur du verrou pour faire l'attente active
}sem;

sem *sema;


int semaphore_init(sem *s, int v){
    /*
    *s= malloc(sizeof(sem)); //Alloue de la place en mémoire pour notre sémaphore
    if(*s==NULL){
        return -1;
    }
    (*s)->val = v;
    (**s).verrou = malloc(sizeof(int));
    *((**s).verrou) = 0;
    return 0;*/
	s->val = v;
	init_lock(&(s->verrou));
}


void semaphore_wait(sem *s){
    while(s->val <= 0);
	lock(&(s->verrou));
	s->val--;
	unlock(&(s->verrou));
}


void semaphore_post(sem *s){
    lock(&(s->verrou));
    s->val = s->val+1; //Incrémente la valeur du sémaphore
    unlock(&(s->verrou));
}



void semaphore_destroy(sem *s){
    free(&(s->verrou));
    free(s);
}

/*

int N; //Nombre de threads

void test_and_set_sem(void){
	
	int count = 0;
	
	while(count < 6400/N){
        printf("début sem value: %d\n", sema->val);
		semaphore_post(sema);
        printf("1 post sem value: %d\n", sema->val);
		semaphore_wait(sema);
        printf("1 post + 1 wait sem value: %d\n", sema->val);
		while(rand() > RAND_MAX/10000);
	    count ++;
		semaphore_post(sema);
        printf("1 post + 1 wait + 1 post sem value: %d\n", sema->val);
		semaphore_wait(sema);
        printf("1 post + 1 wait + 1 post + 1 wait sem value: %d\n", sema->val);
    }
}

void main(int argc, char *argv[]){
    printf("HERE\n");
	N = atoi(argv[1]);
	pthread_t threads[N]; //ON crée la tableeau de threads
    sema=(sem *)malloc(sizeof(sem));
    printf("%d\n%d\n", sema->val, &(sema->verrou));
    semaphore_init(sema, 0);
	for(int i = 0; i<N; i++){
		pthread_create(&threads[i],NULL,(void *) test_and_set_sem,NULL);
		}
	for(int i = 0; i<N; i++){
		pthread_join(threads[i],NULL);
	}
    semaphore_destroy(sema);
}*/