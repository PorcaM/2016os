#include <stdio.h>
#include <stdlib.h>
#include "LRU_stack.h"

void remove_e (sdt *);
sdt *find_e (stack *, int);
void push_back (stack *, int);

void stk_init(stack *stk){
	stk->head = stk->tail = NULL;
	return;
}

int victim(stack *stk){
	int ret = stk->head->num;
	if (stk->head->next != NULL){
		stk->head->next->prev = NULL;
	}
	stk->head = stk->head->next;
	return ret;
}

void reff(stack *stk, int num){
	sdt *it;
	//printf ("\t\tFUNC: REFF called\n");
	remove_e (find_e (stk, num));
	push_back (stk, num);
	//printf ("\t\tFUNC: REFF return\n");
	return;
}

sdt *find_e (stack *stk, int num){
	sdt *it, *ret = NULL;
	//printf ("\t\t\tFUNC: FIND_E called\n");
	for (it = stk->head; it != NULL; it = it->next){
		if (it->num == num){
			ret = it;
			break;
		}
	}
	//printf ("\t\t\tFUNC: FIND_E return %p\n", ret);
	return ret;
}

void remove_e(sdt *target){
	if (target == NULL) return;
	sdt *pnode, *nnode;
	pnode = target->prev;
	nnode = target->next;
	if (pnode != NULL){
		pnode->next = nnode;
	}
	if (nnode != NULL){
		nnode->prev = pnode;
	}
	free (target);
	return;
}

void push_back(stack *stk, int _num){
	//printf ("\t\t\tFUNC: PUSH_BACK called\n");
	//printf ("\t\t\t\tTAIL: %p\n", stk->tail);
	if (stk->tail) {
		sdt *temp = malloc (sizeof (sdt));
		temp->prev	= stk->tail;
		temp->next	= NULL;
		temp->num	= _num;
		stk->tail->next = temp;
		stk->tail = temp;
	}
	else {
		stk->head = stk->tail = malloc (sizeof (sdt));
		stk->head->next		= stk->head->prev = NULL;
		stk->head->num		= _num;
	}
	//printf ("\t\t\tFUNC: PUSH_BACK return\n");
	return;
}
