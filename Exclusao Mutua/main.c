#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 1000000000
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer[4] = {0,0,0,0}; /* Transformei em um array*/

const clock_t MAXDELAY = 2000000;

void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}

void *producer(void *ptr) {
    int i;
    int cont = 0; /* criei um contador pra manipular o array*/
    
    /* usado para sinalizacao */
    /* buffer usado entre produtor e consumidor */
    /* dados do produtor */
    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (buffer[cont] != 0){ /*apenas usei o cont para definir a posicao do array buffer*/
            // Pode acabar a fatia de tempo?
            pthread_cond_wait(&condp, &the_mutex);
          }
        printf("Colocando item %d no buffer\n", i);
        buffer[cont] = i; /*coloca item no buffer(usando o cont mais uma vez)*/
        cont+=1;  /* fazendo o contador andar*/
        delay(MAXDELAY);

        pthread_cond_signal(&condc); /* acorda consumidor */
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
        if (cont == 4) /* quando o contador atinge o numero maximo do array, ele e zerado para que o array sempre seja manipulado dentro de seu tamanho*/
           cont  = 0;
    }

    pthread_exit(0);
}

void *consumer(void *ptr) {
    int i;
    int cont = 0; /* criei um contador pra manipular o array*/
  
    /* quantos numeros produzir */
    /* dados do consumidor */
    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (buffer[cont] == 0){ /*apenas usei o cont para definir a posicao do array buffer*/
          pthread_cond_wait(&condc, &the_mutex);
          }
        buffer[cont] = 0; /* retira o item do buffer(mais uma vez usando o cont) */
        cont+=1; /* fazendo o contador andar*/
        printf("Retirando item %d no buffer\n", i);
        delay(MAXDELAY);
        /* acorda o produtor */
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
        if (cont == 4) /* quando o contador atinge o numero maximo do array, ele e zerado para que o array sempre seja manipulado dentro de seu tamanho*/
           cont  = 0;
    }
    pthread_exit(0);

}

int main(int argc, char **argv)
{
    pthread_t pro, con;
    pthread_mutex_init(&the_mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}
