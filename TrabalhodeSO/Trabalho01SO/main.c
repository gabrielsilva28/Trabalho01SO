#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "primos.h"
#include <sys/time.h>

Matriz *matriz;
pthread_mutex_t msub;
Lista *lista_geralseq;
Lista *lista_geralcon;

int modo=2;
int main(void){
	struct timeval inicio_seq,fim_seq;
	struct timeval inicio_con,fim_con;

	lista_geralseq=criarLista();
	lista_geralcon=criarLista();
	matriz = criarMatriz();
	MatrizAleatoria();
	//MostrarMatriz();
	MostrarPrimos();

	if(modo==1){
		gettimeofday(&inicio_seq, 0);
		MatrizPrimoSequencial();
		gettimeofday(&fim_seq, 0);
		double tempo_seq= (fim_seq.tv_sec + fim_seq.tv_usec/1000000.0) - (inicio_seq.tv_sec  + inicio_seq.tv_usec /1000000.0);
		printf("\nSequencial:\nA matriz possui %d Primos",lista_geralseq->contPrimos);
		printf("\nTempo: %.f segundos\n", tempo_seq);
	}

	if(modo==2){
		pthread_mutex_init(&msub, NULL);

		gettimeofday(&inicio_con, 0);
		divideMatriz();
		gettimeofday(&fim_con, 0);
		double tempo_conc = (fim_con.tv_sec + fim_con.tv_usec/1000000.0) - (inicio_con.tv_sec  + inicio_con.tv_usec /1000000.0);
		printf("Concorrente:\nA matriz possui %d Primos",lista_geralcon->contPrimos);
		printf("\nTempo: %.f segundos\n", tempo_conc);
	}
	return 0;
}
