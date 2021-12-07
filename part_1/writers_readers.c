#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

pthread_mutex_t m_reader;
pthread_mutex_t m_writer;
pthread_mutex_t mutex3; //Il permet d'avoir un seul reader qui execute la fonction
pthread_t *writers;
pthread_t *readers;
int WRITERS;
int READERS;
sem_t db_writers;
sem_t db_reader;
int readcount = 0; //nombre de lecteurs occupés
int writecount = 0; // nombre d'écrivains occupés
__thread int writing = 0;
__thread int reading = 0;


void error(int err, char *msg) { 
  fprintf(stderr,"%s a retourné %d, message d'erreur : %s\n",msg,err,strerror(errno));
  exit(EXIT_FAILURE);
}


void write_database(){
  while(rand() > RAND_MAX/10000);
  writing++;
}
void read_database(){
  while(rand() > RAND_MAX/10000);
  reading++;
}

void *writer(){

    while(writing < 640){
        pthread_mutex_lock(&m_writer);
        //section critique - writecount
        writecount=writecount+1;
        if(writecount==1){
            //Premier writer arrive
            sem_wait(&db_reader);
        }
        pthread_mutex_unlock(&m_writer);
        sem_wait(&db_writers);
        //section critique, un seul writer à la fois
        write_database();
        sem_post(&db_writers);
        pthread_mutex_lock(&m_writer);
        //section critique - writecount
        writecount=writecount-1;
        if(writecount==0){
            //départ du dernier writer
            sem_post(&db_reader);
        }
        pthread_mutex_unlock(&m_writer);
        writing=writing+1;
        }
}

void *reader (){
    while(reading < 2560){
        pthread_mutex_lock(&mutex3);
        //exlusion mutuelle, un seul reader en attente
        sem_wait(&db_reader);
        pthread_mutex_lock(&m_reader);
        //exclusion mutuelle, readcount
        readcount = readcount+1;
        if(readcount==1){
            //Arrivée du premier reader
            sem_wait(&db_writers);
        }
        pthread_mutex_unlock(&m_reader);
        sem_post(&db_reader); //libération du prochain reader
        pthread_mutex_unlock(&mutex3);
        read_database();
        pthread_mutex_lock(&m_reader);
        readcount = readcount-1;
        if(readcount==0){
            //départ du dernier reader
            sem_post(&db_writers);
        }
        pthread_mutex_unlock(&m_reader);
        reading = reading +1;
    }
}

int main(int argc, char *argv[]){
    WRITERS = atoi(argv[1]);
    READERS = atoi(argv[2]);
    writers = malloc(WRITERS*sizeof(pthread_t));
    readers = malloc(READERS*sizeof(pthread_t));
    int err;
    int id[WRITERS];
    int idr[READERS];
    for (int i = 0; i < WRITERS ;i++){
      id[i]=i;
      //printf("C'est ok 1");
    }
    for (int i = 0; i < READERS; i++){
      idr[i]=i;
      //printf("C'est ok 2");
    }
    //Initialiser les sémaphores
    sem_init(&db_writers,0,1);
    sem_init(&db_reader,0,1);
    //Initialiser les mutex
    err = pthread_mutex_init( &m_reader, NULL);
      if(err !=0){
        error(err,"pthread_mutex_init");
      }
    err = pthread_mutex_init( &m_writer, NULL);
      if(err !=0){
        error(err,"pthread_mutex_init");}
    err = pthread_mutex_init( &mutex3, NULL);
      if(err !=0){
        error(err,"pthread_mutex_init");
      }
    //Initialiser les threads
    for (int i = 0; i < WRITERS; i++){
        err = pthread_create(&writers[i], NULL, writer, (void*)&(id[i]));    
        if(err != 0 ){
          error(err,"pthread_create");
        }
    }
    for (int i = 0; i < READERS; i++){
        err = pthread_create(&readers[i], NULL, reader, (void*)&(idr[i]));    
        if(err != 0 ){
          error(err,"pthread_create");
        }
    }

    for (int i = 0; i < WRITERS; i++){
        err = pthread_join(writers[i], NULL);
        if(err != 0 ){
          error(err, "pthread_join");
        }
    }
    for (int i = 0; i < READERS; i++){
        err = pthread_join(readers[i], NULL);
        if(err != 0 ){
          error(err, "pthread_join");
        }
    }
    //Supprimer les mutex
    err = pthread_mutex_destroy(&m_reader);
    if(err!=0)
	      error(err,"pthread_mutex_destroy");
    err = pthread_mutex_destroy(&m_writer);
    if(err!=0)
	      error(err,"pthread_mutex_destroy");
    err = pthread_mutex_destroy(&mutex3);
    if(err!=0)
	      error(err,"pthread_mutex_destroy");
    //Supprimer sémaphore
    sem_destroy(&db_reader);
    if(err!=0) {
        error(err,"sem_destroy");
    }
    sem_destroy(&db_writers);
    if(err!=0){
        error(err,"sem_destroy");
    }
    return (EXIT_SUCCESS);
}