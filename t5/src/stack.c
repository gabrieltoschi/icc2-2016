#include <stdlib.h>
#include <stdio.h>

#include <utils.h>
#include <stack.h>

Stack *stackCreate(int maxSize){
	// alocando nova pilha
	Stack *new = (Stack *) malloc(sizeof(Stack));

	// alocando vetor de dados da pilha
	new->data = (int *) malloc(sizeof(int) * maxSize);

	// atribuindo maximos e inicializando
	new->max = maxSize - 1;
	new->top = -1;

	return new;
}

bool stackEmpty(Stack *s){
	// retorna true se a pilha estiver vazia
	return s->top == -1 ? true : false;
}

bool stackFull(Stack *s){
	// retorna true se a pilha estiver cheia
	return s->top == s->max ? true : false;
}

void stackPush(Stack *s, int d){
	// caso a pilha nao esteja cheia
	if (!stackFull(s)){

		// atualize o topo da pilha
		s->top++;

		// coloque o novo dado lá
		s->data[s->top] = d;
	}
}

int stackPop(Stack *s){
	int r;

	// caso a pilha nao esteja vazia
	if (!stackEmpty(s)){

		// recupere o topo da pilha
		r = s->data[s->top];

		// atualize o topo da pilha
		s->top--;

		return r;
	}

	return -1;
}

int stackView(Stack *s){
	// caso a pilha nao esteja vazia
	if (!stackEmpty(s))
		// recupere o topo da pilha
		return s->data[s->top];

	return -1;
}

void stackDestroy(Stack *s){
	// libera o vetor de dados
	free(s->data);

	// libera a pilha
	free(s);
}

void stackPrint(Stack *s){
	int i;
	for (i = 0; i <= s->top; i++) printf("%d ", s->data[i]+1);
	printf("\n");
}