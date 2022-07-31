#ifndef PRIMOS_H
#define	PRIMOS_H
#include <pthread.h>

//estrutura da matriz
typedef struct _matrizes_{
	int lin;
	int col;
	int *dado;
} Matriz;

typedef struct _nodo_{
	int linha;
	int coluna; 
	int dado;
	struct _nodo_ *proximo;
} Nodo;

typedef struct _lista_{
	Nodo *primeiro; 
	Nodo *ultimo;
	int  contPrimos;
} Lista;

typedef struct _submatrizes_{
	int inicial;
    int final;
} SubMatrizBloco;

extern Matriz *matriz;
extern pthread_mutex_t msub;
extern Lista *lista_geralseq;
extern Lista *lista_geralcon;

int numPrimo(int n);
Matriz *criarMatriz();
void MatrizAleatoria();
void *MatrizPrimoSequencial();
void *MatrizPrimoConcorrente(void* arg_thread);
void divideMatriz();
Lista *criarLista();
void adicionarNodo(Lista* lista, Nodo* nodo);
void MostrarMatriz();
void MostrarPrimos();

#endif