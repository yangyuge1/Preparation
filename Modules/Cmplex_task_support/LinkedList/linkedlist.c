#include "linkedlist.h"

/**
 * @brief       创建链表节点
 * @param       key    : 键
 *              value  : 值
 * @retval      linkedListNode*     : 生成节点的地址
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
 * @brief       链表节点value赋值
 * @param       value  : 赋值内容
 *              target  : 赋值地址
 * @retval      void
 */
void ListValueAssignment(linkedListDataBox value, linkedListDataBox* target)
{
	if (target != NULL) {
	target->data = value.data;
	}
}
/**
 * @brief       链表节点后插入
 * @param       key    : 键
 *              value  : 值
 *              targetNode  : 目标节点
 * @retval      linkedListNode*     : 生成节点的地址
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
 * @brief       链表节点前插入
 * @param       key    : 键
 *              value  : 值
 *              targetNode  : 目标节点
 * @retval      linkedListNode*     : 生成节点的地址
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