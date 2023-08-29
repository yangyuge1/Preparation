#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdint.h>
#include <stdio.h>
#include "memorypool.h"

typedef struct linkedListDataBox {
	int data;
}linkedListDataBox;

typedef struct linkedListNode
{
	int key;
	struct linkedListDataBox value;

	struct linkedListNode* pre;
	struct linkedListNode* next;
}linkedListNode;
void ListValueAssignment(linkedListDataBox value, linkedListDataBox* target);
linkedListNode* CreatLinkedListNode(int key, linkedListDataBox value);
linkedListNode* InsertPre(int key, linkedListDataBox value, linkedListNode* targetNode);
linkedListNode* InsertNext(int key, linkedListDataBox value, linkedListNode* targetNode);
void PrintListFromHereToPre(linkedListNode* node);
void PrintListFromHereToNext(linkedListNode* node);
linkedListNode* SearchListByKey(int key, linkedListNode* node);
#endif
