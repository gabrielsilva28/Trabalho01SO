#include "primos.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#define FALSE 0
#define TRUE 1

#define m_aleatoria 29999 //0 a 29999
#define semente 100000    //semente pre determinada
#define num_threads 2	  //numeros de threads

// tamanho matriz
#define tam_lin 9
#define tam_col 9

int tam_mat=tam_lin*tam_col;
//calcula tamanho dos blocos
#define tam_bloco (tam_mat)/num_threads
#define resto (tam_mat)%num_threads

pthread_t threads[num_threads]; 
SubMatrizBloco submat[num_threads]; 

//acha os numeros primos
int numPrimo(int num){
    int raiz;
    if (num <= 3){
        return ! ( num < 2 );
    }
    if ( ( num % 2 ) == 0 || ( num % 3 ) == 0){
        return FALSE;
    }
    raiz=sqrt(num);
    for (int i = 5; i <= raiz; i += 4 )
    {
        if ( ( num % i ) == 0){
            return FALSE;
        }
        i += 2;
        if ( ( num % i ) == 0){
            return FALSE;
        }
    }
    return TRUE;
    
}

//aloca a matriz para armazenar os valores
Matriz *criarMatriz(){
	Matriz *matriz = malloc(sizeof(Matriz));
	matriz->dado= malloc(tam_mat*sizeof(int));
	matriz->lin = tam_lin;
	matriz->col = tam_col;
	return matriz;	
}

//gera a matriz aleatoria
void MatrizAleatoria(){
	srand(semente);
  	for (int i=0; i<matriz->lin; i++ ){
        for (int j=0; j<matriz->col; j++ ) {
          	matriz->dado[i*matriz->col+j]=rand()%(m_aleatoria);
		}
	}	
}

//Sequencial
//Encontrar os primos na matriz no modo sequencial e armazena em uma lista
void *MatrizPrimoSequencial() {
	Nodo *primo_encontradoseq;
	for (int i = 0; i < matriz->lin; i++) {
		for (int j = 0; j < matriz->col; j++) {
			if(numPrimo(matriz->dado[i*matriz->col+j])){
				primo_encontradoseq=malloc(sizeof(Nodo));
				primo_encontradoseq->linha=i;
				primo_encontradoseq->coluna=j;
				primo_encontradoseq->dado=matriz->dado[i*matriz->col+j];
				primo_encontradoseq->proximo=NULL;
				adicionarNodo(lista_geralseq,primo_encontradoseq);   		
			}
		}
	}
}
//Fim da Sequencial

//Concorrente
//Encontrar os primos na matriz no modo concorrente e armazena em uma lista
void *MatrizPrimoConcorrente(void* arg_thread) {
	SubMatrizBloco* submat = (SubMatrizBloco*)arg_thread;      
	Nodo* primo_encontradocon = malloc(sizeof(Nodo));

    for (int i = submat->inicial; i <= submat->final;i++) {
        if (numPrimo(matriz->dado[i])) {
            primo_encontradocon->linha = i/tam_lin;
            primo_encontradocon->coluna = i%tam_col;
            primo_encontradocon->proximo = NULL;
            pthread_mutex_lock(&msub);
            adicionarNodo(lista_geralcon, primo_encontradocon);
            pthread_mutex_unlock(&msub);
        }
    }
    pthread_exit(NULL);
}

//Divide a matriz em submatrizes
void divideMatriz() {
	for (int i = 0; i < num_threads; i++) {
        submat[i].inicial = i*tam_bloco;
        submat[i].final = ((i+1)*tam_bloco)-1;
        pthread_create(&(threads[i]), NULL, &MatrizPrimoConcorrente, (void*)&submat[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    if (resto> 0) {
        submat->inicial = num_threads*tam_bloco;
        submat->final = num_threads*tam_bloco+(resto-1);
        pthread_create(&threads[0], NULL, &MatrizPrimoConcorrente, (void*)&submat);
        pthread_join(threads[0], NULL);
    }
}

//aloca lista para receber os primos
Lista *criarLista(){
	Lista *valor_final = malloc(sizeof(Lista));
    if (valor_final == NULL) {
        return NULL;
    }
    
    valor_final->contPrimos = 0;
    return valor_final;
}

//Recebe a lista, e os primos encontrados
void adicionarNodo(Lista* lista, Nodo* nodo) {
    if (lista == NULL || nodo == NULL) {
        return;
    }

    if (lista->contPrimos == 0) { 
        lista->primeiro = nodo;
        lista->ultimo = nodo;
        lista->contPrimos ++;
        return;
    }

    Nodo* nodo_atual = lista->ultimo;
    nodo_atual->proximo = nodo;
    lista->contPrimos++;
    lista->ultimo = nodo;
    
}

//Fim da Concorrente

//mostra a matriz 
void MostrarMatriz(){
  	for (int i=0; i<matriz->lin; i++ ){
      for (int j=0; j<matriz->col; j++ ) {
      	printf("%d ",matriz->dado[i*matriz->col+j]);
      }
    	printf("\n");
    }
}

//mostra os primos e a posição desses primos
void MostrarPrimos(){
    printf("\nPosicao dos primos:\n"); 
	for (int i = 0; i < matriz->lin; i++) {
		for (int j = 0; j < matriz->col; j++) {
			if(numPrimo(matriz->dado[i*matriz->col+j])){
				printf("Linha[%d] Coluna[%d] - valor[%d]\n",i,j,matriz->dado[i*matriz->col+j]);
			}
		}
	}
}

