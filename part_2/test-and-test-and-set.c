//test-and-test-and-set
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

int l= 0; //lock ==0 
int N; //Nombre de threads

void init_lock(int volatile *verrou){
    *verrou = 0;
}

//Fonction lock
void lock(int volatile *verrou){
    int t=1;
    while(t == 1)
        while(verrou==1){};
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

int volatile verrou;

//Fonction pour testterle lock et unlock
void test_and_test_and_set(void){
	
	int count = 0;
	
	while(count < 6400/N){
	    lock(&verrou);
	
	    while(rand() > RAND_MAX/10000){}
	    unlock(&verrou);
	    count ++;}
	}

int main(int argc, char *argv[]){
	N = atoi(argv[1]);
	pthread_t threads[N]; //ON crée la tableeau de threads
	for(int i = 0; i<N; i++){
		pthread_create(&threads[i],NULL,(void *) test_and_test_and_set,NULL);
		}
	for(int i = 0; i<N; i++){
		pthread_join(threads[i],NULL);
		}
	}
