#include "linkedlist.h"

/**
 * @brief       ��������ڵ�
 * @param       key    : ��
 *              value  : ֵ
 * @retval      linkedListNode*     : ���ɽڵ�ĵ�ַ
 */
linkedListNode * CreatLinkedListNode(int key, linkedListDataBox value)
{
	linkedListNode* node= (linkedListNode *)getMemory(sizeof(linkedListNode));
	if(node!=NULL)
	{ 
		ListValueAssignment(value, &(node->value));
		return node;
	}
	else {
		return NULL;
	}
}
/**
 * @brief       ����ڵ�value��ֵ
 * @param       value  : ��ֵ����
 *              target  : ��ֵ��ַ
 * @retval      void
 */
void ListValueAssignment(linkedListDataBox value, linkedListDataBox* target)
{
	if (target != NULL) {
	target->data = value.data;
	}
}
/**
 * @brief       ����ڵ�����
 * @param       key    : ��
 *              value  : ֵ
 *              targetNode  : Ŀ��ڵ�
 * @retval      linkedListNode*     : ���ɽڵ�ĵ�ַ
 */
linkedListNode* InsertPre(int key, linkedListDataBox value, linkedListNode* targetNode)
{
	if (targetNode != NULL)
	{
		linkedListNode* newNode= (linkedListNode*)getMemory(sizeof(linkedListNode));
		newNode->next = targetNode;
		newNode->key = key;
		ListValueAssignment(value, &(newNode->value));

		if (targetNode->pre != NULL)
		{
			newNode->pre = targetNode->pre;
			targetNode->pre->next = newNode;
			targetNode->pre = newNode;
			return newNode;
		}
		else {
			newNode->pre = NULL;
			targetNode->pre = newNode;
			return newNode;
		}
	}
	else
	{
		return NULL;
	}
}
/**
 * @brief       ����ڵ�ǰ����
 * @param       key    : ��
 *              value  : ֵ
 *              targetNode  : Ŀ��ڵ�
 * @retval      linkedListNode*     : ���ɽڵ�ĵ�ַ
 */
linkedListNode* InsertNext(int key, linkedListDataBox value, linkedListNode* targetNode)
{
	if (targetNode != NULL)
	{
		linkedListNode* newNode = (linkedListNode*)getMemory(sizeof(linkedListNode));
		newNode->pre = targetNode;
		newNode->key = key;
		ListValueAssignment(value, &(newNode->value));

		if (targetNode->next != NULL)
		{
			newNode->next = targetNode->next;
			targetNode->next->pre = newNode;
			targetNode->next = newNode;
			return newNode;
		}
		else {
			newNode->next = NULL;
			targetNode->next = newNode;
			return newNode;
		}
	}
	else
	{
		return NULL;
	}
}

void PrintListFromHereToPre(linkedListNode* node)
{
	linkedListNode* start = node;
	printf("\nPrintListFromHereToPre:\n");
	if (node != NULL)
	{
		do {
			printf("{%d,(%d)}\n", node->key, node->value.data);
			node = node->pre;
		} while (node != NULL && node != start);
	}
	else {
		printf("\nerror\n");
	}
}
void PrintListFromHereToNext(linkedListNode* node)
{
	linkedListNode* start = node;
	printf("\nPrintListFromHereToNext:\n");
	if (node != NULL)
	{
		do {
		printf("{%d,(%d)}\n", node->key, node->value.data);
		node = node->next;
		} while (node != NULL && node != start);
	}
	else {
		printf("\nerror\n");
	}
}
linkedListNode* SearchListByKey(int key, linkedListNode* node)
{
	linkedListNode* start = node;
	if (node != NULL)
	{
		do {
			if (node->key == key)
			{
				return node;
			}
			node = node->next;
		} while (node != NULL && node != start);

		node = start;
		do {
			if (node->key == key)
			{
				return node;
			}
			node = node->pre;
		} while (node != NULL && node != start);
		return NULL;
	}
	else {
		return NULL;
	}
}