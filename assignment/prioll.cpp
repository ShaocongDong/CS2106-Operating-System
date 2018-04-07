#include <stdio.h>
#include <stdlib.h>
#include "prioll.h"

void prioInsertNode(TPrioNode **head, TPrioNode *node)
{
	// If head is NULL, return node
	if(*head == NULL)
		*head = node;
	else
	{
		TPrioNode *trav = *head;
		int prio = node->prio;

		while(trav->next != NULL && trav->prio < prio)
			trav = trav->next;

		// If we are smaller than to trav->prio, we insert before trav,
		// Otherwise we insert after

		if(trav->prio >= prio)
		{
			node->next = trav;
			node->prev = trav->prev;

			if(trav->prev != NULL)
				trav->prev->next = node;
			else
				*head = node;

			trav->prev = node;
		}
		else
		{
			// We insert after trav
			node->next = trav->next;
			trav->next = node;
			node->prev = trav;
		}
	}
}

void prioInsert(TPrioNode **head, int procNum, int period, int prio)
{
	TPrioNode *newNode = (TPrioNode *) malloc(sizeof(TPrioNode));

	newNode->prev=NULL;
	newNode->next=NULL;
	newNode->procNum = procNum;
	newNode->prio = prio;
	newNode->p = period;

	prioInsertNode(head, newNode);
}

TPrioNode *checkReady(TPrioNode *head, int timerTick)
{
	TPrioNode *trav = head;

	while(trav != NULL)
	{
		if((timerTick % trav->prio) == 0)
		{
			return trav;
		}
		else
			trav = trav->next;
	}

	return NULL;
}

TPrioNode *prioRemove(TPrioNode **head)
{
	if(*head == NULL)
	{
		return NULL;
	}

	TPrioNode *tmp = *head;

	if(tmp != NULL)
	{
		*head = tmp->next;
		tmp->next = NULL;
		tmp->prev = NULL;
	}

	if(*head != NULL)
		(*head)->prev = NULL;

	return tmp;
}

TPrioNode *prioRemoveNode(TPrioNode **head, TPrioNode *node)
{
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		*head = node->next;

	if(node->next != NULL)
		node->next->prev = node->prev;

	node->next=NULL;
	node->prev=NULL;

	return node;
}

TPrioNode *peek(TPrioNode *head)
{
	return head;
}

void printList(TPrioNode *head)
{
	TPrioNode *trav = head;

	while(trav)
	{
		printf("id %d prio %d\n", trav->procNum, trav->prio);
		trav=trav->next;
	}
}

int gcd(int a, int b)
{
	if(b==0)
		return a;

	return gcd(b, a%b);
}

// Find the LCM of all the periods in the list
int prioLCM(TPrioNode *head)
{
	if(head == NULL)
		return 0;

	int res=1;

	TPrioNode *trav = head;

	while(trav)
	{
		res = res * trav->p/gcd(res, trav->p);
		trav = trav->next;
	}

	return res;
}

// Delete entire list
void prioDestroy(TPrioNode **head)
{
	TPrioNode *trav = *head;
	*head = NULL;

	while(trav != NULL)
	{
		TPrioNode *tmp = trav;
		trav=trav->next;
		free(tmp);
	}
}

