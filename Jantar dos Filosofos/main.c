#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/types.h"
#include "pthread.h"
#include "semaphore.h"

#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define TRUE   1

typedef sem_t;
int state[N];

sem_t mutex;                    
sem_t s[N]; 


int philosopher(int i)
{

     while (TRUE){
     think(i);
     take_forks(i);
     eat(i);
     put_forks(i);
     }
}

void take_forks(int i)
{
     sem_wait(&mutex);
     state[i] = HUNGRY;
     test(i);
     sem_post(&mutex);
     sem_wait(&s[i]);
}

void put_forks(i)
{
     sem_wait(&mutex);
     state[i] = THINKING;
     test(LEFT);
     test(RIGHT);
     sem_post(&mutex);
}

void test(i)
{
     if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
     {
      state[i] = EATING;
      sem_post(&s[i]);
     }
}

void eat(int i){
    
     printf("O filosofo %d ta comendo \n",i);
     sleep(rand()%5);
}

void think(int i){

     printf("O filoso %d ta pensando \n",i);
     sleep(rand()%10);
   }

int main(int argc, char **argv)
{


int i;

//iniciando o semaforo 
sem_init(&mutex, TRUE, 1);

 for(i = 0; i < N; i++) {
     sem_init(&s[i], TRUE, 1);
     
 }


 pthread_t tphil[5];


//Criando filosofo
   for(i = 0; i < N; i++) {
     pthread_create(&tphil[i], NULL,philosopher, i);
 }

 for(i = 0; i < N; i++) {
     pthread_join(tphil[i], NULL);
 }

 return 0;
}
