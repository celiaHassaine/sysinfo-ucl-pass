//Test-and-set
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

//Fonction lock
void lock(void){
    int t=1;
    while(t == 1){
        printf("t = %d\n", t);
        asm volatile ("movl %1, %%eax;"
        "xchgl %%eax, %0;"
        :"=m" (l)
        :"m" (t)
        :"%eax"
        );
    }
}
//Fonction unlock
void unlock(void){
    int t=0;
    asm ("movl %0, %%eax;"   
        "xchgl %%eax, %1;"
        :"=m" (l)
        :"m" (t)
        :"%eax"
    );
}

//Fonction pour testterle lock et unlock
void test_and_set(void){
    printf("okay\n");
	
	int count = 0;
	
	while(count < 6400/N){
	    lock();
	
	    while(rand() > RAND_MAX/10000){}
	    unlock();
	    count ++;
    }
}

int main(int argc, char *argv[]){
	N = atoi(argv[1]);
	pthread_t threads[N]; //ON crée la tableeau de threads
    printf("argument: %d\n", N);
	for(int i = 0; i<N; i++){
		pthread_create(&threads[i],NULL,(void *) test_and_set,NULL);
	}
	for(int i = 0; i<N; i++){
		pthread_join(threads[i],NULL);
	}
}
