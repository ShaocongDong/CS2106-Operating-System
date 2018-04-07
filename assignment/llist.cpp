#include <stdlib.h>
#include <stdio.h>
#include "llist.h"

void insert(TNode **head, int procNum, int quantum)
{
	TNode *node = (TNode *) malloc(sizeof(TNode));
	node->procNum = procNum;
	node->quantum = quantum;
	node->next=NULL;

	if(*head==NULL)
		*head = node;
	else
	{
		// Insert at the end of the list
		TNode *trav=*head;

		while(trav->next != NULL)
			trav = trav->next;

		trav->next = node;
	}
}

int remove(TNode **head)
{
	int ret;
	if(*head == NULL)
		return -1;

	TNode *tmp = *head;
	ret = tmp->procNum;
	*head = tmp->next;

	free(tmp);
	return ret;
}

int totalQuantum(TNode *head)
{
	TNode *trav = head;
	int sum=0;

	while(trav)
	{
		sum += trav->quantum;
		trav = trav->next;
	}

	return sum;
}

void destroy(TNode **head)
{
	TNode *trav = *head;
	*head = NULL;

	while(trav)
	{
		TNode *tmp = trav;
		trav = trav->next;
		free(tmp);
	}
}
