#include "LRU_stack.h"

void stk_init(stack& stk){
	stk.head = stk.tail = NULL;
	return;
}

int victim(stack &stk){
	int ret = stk.head->num;
	if (stk.head->next != NULL){
		stk.head->next.prev = NULL;
	}
	stk.head = stk.head->next;
	return ret;
}

void reff(stack &stk, int num){
	sdt *it;
	for (it = stk.headd; it != stk.tail; it++){
		if (it->num == num){
			remove (it);
		}
	}
	if (it->num == num){
		remove (it);
	}
	push_back (stk, num);
}

void remove(sdt *target){
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

void push_back(stack &stk, int num){
	stk.tail->next 	= malloc (sizeof (sdt));
	stk.tail->next->prev 	= stk.tail;
	stk.tail->next->next 	= NULL;
	stk.tail->next->num 	= num;
	stk.tail = stk.tail->next;
	return;
}