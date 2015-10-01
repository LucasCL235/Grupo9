#include <stdio.h>
#include <stdlib.h>

typedef struct pilha_dinamica PILHA_DINAMICA;

typedef struct Bloco {
	int item;
	struct Bloco *anterior;
} NO;

struct pilha_dinamica {
	NO *topo;
	int tamanho;
};

int vazia (PILHA_DINAMICA *pilha) {
	return (pilha->topo == NULL);
}

int tamanho (PILHA_DINAMICA *pilha) {
	return pilha->tamanho;
}

void criar_pilha(PILHA_DINAMICA *pilha) {
	pilha->tamanho=0;
	pilha->topo=NULL;
}

void push (int entrada,PILHA_DINAMICA *pilha) {

	NO *no_aux = malloc (sizeof(NO));

	no_aux->anterior = pilha->topo;
	pilha->topo = no_aux;

	pilha->topo->item = entrada;
	(pilha->tamanho)++;
}

void pop (int *saida,PILHA_DINAMICA *pilha) {

	*(saida) = pilha->topo->item;

	NO *no_aux = pilha->topo->anterior;
	free(pilha->topo);
	pilha->topo = no_aux;

	free(no_aux);
	(pilha->tamanho)--;
}

void imprimir_pilha (PILHA_DINAMICA *pilha) {
	NO *run = pilha->topo;
	while(run!=NULL) {
		printf("%d ",run->item);
		run=run->anterior;
	}
}

int main( ) {

	PILHA_DINAMICA *pilha = (PILHA_DINAMICA*)malloc(sizeof(PILHA_DINAMICA));
	criar_pilha(pilha);

	char entrada[1];
	int controle = 1, n1=0, n2=0, resultado=0;

	while(entrada[0] != 'q') {

		fgets(entrada,10,stdin);

		if(entrada[0] >= '0' && entrada[0] <= '9') {
			if (pilha->topo==NULL) puts("-");
			pilha->topo->item += ( (controle)*(entrada[0]) );
		}			

		switch(entrada[0]){ 

			case 'E':
				push(0,pilha);
				controle=1;
				break;

			case '+':
				pop(&n1,pilha);
				pop(&n2,pilha);
				resultado = n1+n2;
				push(resultado,pilha);
				controle=1;
				break;
				

			case '-':
				pop(&n1,pilha);
				pop(&n2,pilha);
				resultado = n1-n2;
				push(resultado,pilha);
				controle=1;
				break;

			case '*':
				pop(&n1,pilha);
				pop(&n2,pilha);
				resultado = n1*n2;
				push(resultado,pilha);
				controle=1;
				break;
			case '/':
				pop(&n1,pilha);
				pop(&n2,pilha);
				resultado = n1/n2;
				push(resultado,pilha);
				controle=1;
				break;

		}

		controle *= 10;

		imprimir_pilha(pilha);

	}

	return 0;

}

