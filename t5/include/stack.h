#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack {
	int *data;
	int top;
	int max;
} Stack;

Stack *stackCreate(int maxSize);
bool stackEmpty(Stack *s);
bool stackFull(Stack *s);
void stackPush(Stack *s, int d);
int stackPop(Stack *s);
int stackView(Stack *s);
void stackDestroy(Stack *s);
void stackPrint(Stack *s);

#endif